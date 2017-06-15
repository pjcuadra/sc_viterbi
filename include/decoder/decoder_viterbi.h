/* Copyright 2017 Pedro Cuadra <pjcuadra@gmail.com> & Meghadoot Gardi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#ifndef DECODER_VITERBI_H_
#define DECODER_VITERBI_H_

#include <systemc.h>
#include <common/util.h>
#include <decoder/viterbi_path.h>
#include <common/shift_register.h>
#include <common/clock_divider.h>
#include <bitset>

#define CURR_STAGE 0
#define NEXT_STAGE 1
#define MAX_STAGES 2

template<int output, int input, int memory, int output_buffer_bit_size>
  SC_MODULE (decoder_viterbi) {
    // Constants
    /** With of the input */
    static const uint input_width = output * (2 * memory - input);
    /** Number of states per trellis diagram stage */
    static const uint states_num = input << (memory - 1);
    /** Lookup table size */
    static const uint lookup_size = input << memory;
    /** Output width */
    static const uint output_width = input_width / output;
    // Inputs
    /** Parallel Input */
    sc_in<sc_logic> in;
    /** Input clock */
    sc_in_clk clk;
    /** Decoding trigger */
    sc_in_clk trigger;
    /** Polynomials input */
    sc_in<sc_lv<memory * input> > polynomials[output];
    // Outputs
    /** Parallel Output */
    sc_out<sc_logic> out;
    /** Trellis State Table */
    viterbi_path_s<output_buffer_bit_size> trellis_tree_lkup[MAX_STAGES][states_num];
    /** Next state lookup table */
    sc_lv<memory * input> next_state_lkp[lookup_size];
    /** Output lookup table */
    sc_lv<output> output_lkp[lookup_size];
    // Sub modules
    /** Clock divider */
    clock_divider<output> * clk_divider;
    /** Shift Registers */
    shift_register<output> * shift_reg;
    // Internal Signal
    /** Divided clk signal */
    sc_signal<bool> clk_div;
    /** Parallel Input */
    sc_signal<sc_lv<output> > par_in;
    /** Current Trellis Stage */
    uint curr_trellis_stage;
    /** Conter for serializing  */
    uint serializer_count;
    /** Decoding state flag */
    bool decoding;
    /** Serializing state flag */
    bool serializing;
    /** Shift stage */
    sc_event shift_stage;


    /**
     * Calculate the metrics given 2 values
     * @param val_0 first value
     * @param val_1 second value
     * @return metric value
     */
    uint get_metrics(uint val_0, uint val_1) {
      uint distance = 0;

      val_0 ^= val_1;

      val_0 = ((1 << output) - 1) & ~val_0;

      // Count the ones
      while (val_0) {

        if (val_0 & 0x1) {
          distance++;
        }

        val_0 = val_0 >> 1;
      }

      return distance;

    }

    /**
     * Decode a parallel input using Viterbi algorithm
     */
    void prc_decode_viterbi() {
      sc_uint<memory + input> lkup_address;
      viterbi_path_s<output_buffer_bit_size> curr_path;
      viterbi_path_s<output_buffer_bit_size> next_state;
      uint new_metrics = 0;
      sc_uint<states_num> next_state_index = 0;

      if (!decoding) {
        return;
      }

      curr_trellis_stage++;

      if (curr_trellis_stage > output_buffer_bit_size) {
        return;
      }

      for (int state_row = 0; state_row < states_num; state_row++) {
        curr_path = trellis_tree_lkup[CURR_STAGE][state_row];

        if (!curr_path.is_alive) {
          continue;
        }

        // Iterate over all possible inputs
        for (int p_in = 0; p_in < (1 << input); p_in++) {
          lkup_address = (lookup_size - 1) & ((state_row << input) | p_in);
          next_state_index = (states_num - 1) & next_state_lkp[lkup_address].to_uint();
          next_state = trellis_tree_lkup[NEXT_STAGE][next_state_index];

          new_metrics = curr_path.metric_value;
          new_metrics += get_metrics(par_in.read().to_uint(), output_lkp[lkup_address].to_uint());

          if (next_state.metric_value > new_metrics) {
            continue;
          }

          next_state = curr_path;

          next_state.metric_value = new_metrics;
          // Push the state to the list (array)
          next_state.path_output[next_state.path_size++] = next_state_index[0];

          trellis_tree_lkup[NEXT_STAGE][next_state_index] = next_state;
        }

        if (curr_path.path_size > 0) {
          for (int i = 0; i < curr_path.path_size; i++) {
          }
        }

      }

      // Change next state to current
      shift_stage.notify();

    }

    /**
     * Shift to the next trellis stage
     */
    void prc_shift_stage() {

      next_trigger(shift_stage);

      // Change next state to current
      for (int i = 0; i < states_num; i++) {
        trellis_tree_lkup[CURR_STAGE][i] = trellis_tree_lkup[NEXT_STAGE][i];
      }
    }

    /**
     * Build the output lookup table based on polynimials
     */
    void prc_update_output_lkup() {
      sc_lv<memory * input> polynomials_tmp[output];

      // Invert the order of bits
      for (int o = 0; o < output; o++) {
        polynomials_tmp[o] = polynomials[output - o -1];
      }

      create_output_lkup<output, input, memory>(polynomials_tmp, output_lkp);
    }

    /**
     * Serialize the outpu
     */
    void prc_serialize_output() {
      sc_uint<1> output_bus;
      viterbi_path_s<output_buffer_bit_size> curr_path = trellis_tree_lkup[CURR_STAGE][5];

      out = sc_logic('0');

      if (!serializing) {
        return;
      }

      for (int state_row = 0; state_row < states_num; state_row++) {
        if (!trellis_tree_lkup[CURR_STAGE][state_row].is_alive) {
          continue;
        }

        if (trellis_tree_lkup[CURR_STAGE][state_row].metric_value > curr_path.metric_value) {
          curr_path = trellis_tree_lkup[CURR_STAGE][state_row];
        }
      }


      out = sc_logic(curr_path.path_output[serializer_count].to_bool());

      serializer_count++;

      if (serializer_count > (output_buffer_bit_size - 1)) {
        serializing = false;
      }
    }

    void prc_decode_catch_trigger() {
      // Initialize the Trellis Diagram
      // Mark all as not alive and the highest metric value possible
      for (int stage = 0; stage < MAX_STAGES; stage++) {
        for (int state_row = 0; state_row < states_num; state_row++) {
          trellis_tree_lkup[stage][state_row].path_size = 0;
          trellis_tree_lkup[stage][state_row].metric_value = 0;
          trellis_tree_lkup[stage][state_row].path_output = 0;
          trellis_tree_lkup[stage][state_row].is_alive = false;
        }
      }

      // Initlialize the first trellis diagram item
      trellis_tree_lkup[0][0].path_size = 0;
      trellis_tree_lkup[0][0].metric_value = 0;
      trellis_tree_lkup[0][0].path_output = 0;
      trellis_tree_lkup[0][0].is_alive = true;

      decoding = true;
      serializing = false;
      curr_trellis_stage = 0;

    }

    void prc_decode_start_serializing() {
      serializing = true;
      serializer_count = 0;
      decoding = false;
    }

    SC_CTOR (decoder_viterbi) {

      // Initialize the lookup table
      create_states_lkup<output, input, memory>(next_state_lkp);

      // Create clock divider and connect its inputs/outputs
      clk_divider = new clock_divider<output>("clk_div");
      clk_divider->clk_in(clk);
      clk_divider->clk_out(clk_div);

      // Add the shift register to parallelize the input
      shift_reg =  new shift_register<output> ("shift_reg");
      shift_reg->data_in(in);
      shift_reg->clk(clk);
      shift_reg->q(par_in);

      decoding = false;
      serializing = false;

      SC_METHOD (prc_decode_viterbi);
      sensitive_pos << clk_div;

      SC_METHOD (prc_decode_catch_trigger);
      sensitive << trigger.pos();

      SC_METHOD (prc_decode_start_serializing);
      sensitive << trigger.neg();

      SC_METHOD(prc_update_output_lkup)
      for (int i = 0; i < output; i++) {
        sensitive << polynomials[i];
      }

      SC_METHOD (prc_serialize_output);
      sensitive << clk.pos();

      SC_METHOD(prc_shift_stage)

    }
  };

#endif
