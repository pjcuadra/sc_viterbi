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
#include <bitset>

#define CURR_STAGE 0
#define NEXT_STAGE 1
#define MAX_STAGES 2

template<int output, int input, int memory>
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
    sc_in<sc_lv<input_width> > in;
    /** Input clock */
    sc_in_clk clk;
    /** Decoding trigger */
    sc_in_clk trigger;
    /** Polynomials input */
    sc_in<sc_lv<memory * input> > polynomials[output];
    // Outputs
    /** Parallel Output */
    sc_out<sc_lv<output_width> > out;
    /** Trellis State Table */
    viterbi_path_s<output, input, memory> trellis_tree_lkup[MAX_STAGES][states_num];
    /** Next state lookup table */
    sc_lv<memory * input> next_state_lkp[lookup_size];
    /** Output lookup table */
    sc_lv<output> output_lkp[lookup_size];
    // Events
    /** Result ready event */
    sc_event res_ready;

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
      sc_lv<input_width> in_tmp = in;
      sc_lv<output> in_bus[output_width];
      sc_uint<memory + input> lkup_address;
      viterbi_path_s<output, input, memory> curr_path;
      viterbi_path_s<output, input, memory> next_state;
      uint new_metrics = 0;
      sc_uint<states_num> next_state_index = 0;

      if (!this->trigger) {
        return;
      }

      // Create independent buses for every output bundle
      for (int i = 0; i < output_width; i++) {
        in_bus[output_width - i - 1] = in_tmp.range(2*i, 2*i +1);
      }

      // Iterate over the entire input
      for (int input_selector = 0; input_selector < input_width; input_selector += 2) {
        cout << "Stage: " << input_selector / output << endl;
        cout << "  Sel. Input: " << in_bus[input_selector / output].to_uint() << endl;
        for (int state_row = 0; state_row < states_num; state_row++) {
          curr_path = trellis_tree_lkup[CURR_STAGE][state_row];

          if (!curr_path.is_alive) {
            continue;
          }

          cout << "  State " << state_row << endl;

          // Iterate over all possible inputs
          for (int p_in = 0; p_in < (1 << input); p_in++) {
            lkup_address = (lookup_size - 1) & ((state_row << input) | p_in);
            next_state_index = (states_num - 1) & next_state_lkp[lkup_address].to_uint();
            next_state = trellis_tree_lkup[NEXT_STAGE][next_state_index];

            new_metrics = curr_path.metric_value;
            new_metrics += get_metrics(in_bus[input_selector / output].to_uint(), output_lkp[lkup_address].to_uint());

            cout << "    Input " << in_bus[input_selector / output].to_uint() << endl;
            cout << "    Metric " << new_metrics << endl;
            cout << "    Next State " << next_state_index << endl;

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
        memcpy(&trellis_tree_lkup[CURR_STAGE], &trellis_tree_lkup[NEXT_STAGE], states_num * sizeof(viterbi_path_s<output, input, memory>));

      }

      res_ready.notify();

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
     * Build the output lookup table based on polynimials
     */
    void prc_update_output() {
      sc_uint<output_width> output_bus;
      viterbi_path_s<output, input, memory> curr_path = trellis_tree_lkup[CURR_STAGE][5];

      for (int state_row = 0; state_row < states_num; state_row++) {
        if (!trellis_tree_lkup[CURR_STAGE][state_row].is_alive) {
          continue;
        }

        if (trellis_tree_lkup[CURR_STAGE][state_row].metric_value > curr_path.metric_value) {
          curr_path = trellis_tree_lkup[CURR_STAGE][state_row];
        }
      }

      for (int path_item_index = 0; path_item_index < output_width; path_item_index++) {
        output_bus[output_width - path_item_index - 1] = curr_path.path_output[path_item_index + 1];
      }

      out = output_bus;
    }

    SC_CTOR (decoder_viterbi) {

      // Initialize the lookup table
      create_states_lkup<output, input, memory>(next_state_lkp);

      // Initialize the Trellis Diagram
      // Mark all as not alive and the highest metric value possible
      for (int stage = 0; stage < MAX_STAGES; stage++) {
        for (int state_row = 0; state_row < states_num; state_row++) {
          trellis_tree_lkup[stage][state_row].path_size = 0;
          trellis_tree_lkup[stage][state_row].metric_value = 0;
          trellis_tree_lkup[stage][state_row].is_alive = false;
        }
      }

      // Initlialize the first trellis diagram item
      trellis_tree_lkup[0][0].path_size = 1;
      trellis_tree_lkup[0][0].metric_value = 0;
      trellis_tree_lkup[0][0].path_output = 0;
      trellis_tree_lkup[0][0].is_alive = true;

      SC_METHOD (prc_decode_viterbi);
      sensitive_pos << trigger;

      SC_METHOD(prc_update_output_lkup)
      for (int i = 0; i < output; i++) {
        sensitive << polynomials[i];
      }

      SC_METHOD (prc_update_output);
      sensitive << res_ready;

    }
  };

#endif
