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
#include <bitset>

#define CURR_STAGE 0
#define NEXT_STAGE 1
#define MAX_STAGES 2

template<int output, int input, int memory>
  struct viterbi_path_s {
    uint metric_value;
    uint path_size;
    uint states_path[output * (2 * memory - input)];
    bool is_alive;
  };

template<int output, int input, int memory>
  SC_MODULE (decoder_viterbi) {
    static const uint input_width = output * (2 * memory - input);
    static const uint states_num = input << (memory - 1);
    const static uint lookup_size = input << memory;
    const static uint output_width = input_width / output;
    sc_in<sc_lv<input_width> > in;
    sc_out<sc_lv<output_width> > out;
    sc_in_clk clk;
    sc_in_clk trigger;
    viterbi_path_s<output, input, memory> trellis_tree_lkup[MAX_STAGES][states_num];
    /** Next state lookup table */
    sc_lv<memory * input> next_state_lkp[lookup_size];
    /** Output lookup table */
    sc_lv<output> output_lkp[lookup_size];
    /** Polynomials input */
    sc_in<sc_lv<memory * input> > polynomials[output];
    sc_event res_ready;

    uint get_metrics(uint val_0, uint val_1) {
      uint distance = 0;

      val_0 ^= val_1;

      val_0 = ((1 << output) - 1) & ~val_0;

      while (val_0) {

        if (val_0 & 0x1) {
          distance++;
        }

        val_0 = val_0 >> 1;
      }

      return distance;

    }

    void prc_decode_viterbi() {
      sc_lv<input_width> in_tmp = in;
      sc_lv<output> in_bus[output_width];
      sc_uint<memory + input> lkup_address;
      viterbi_path_s<output, input, memory> * curr_path = 0;
      uint new_metrics = 0;
      uint next_state_index = 0;

      if (!this->trigger) {
        return;
      }

      // Create independent buses for every output bundle
      for (int i = 0; i < output_width; i++) {
        in_bus[output_width - i - 1] = in_tmp.range(2*i, 2*i +1);
      }

      for (int input_selector = 0; input_selector < input_width; input_selector += 2) {
        cout << " +- Trellis Diagram Stage " << input_selector / output << endl;
        cout << " | +- Selected input: " << std::bitset<output>(in_bus[input_selector / output].to_uint()) << endl;
        for (int state_row = 0; state_row < states_num; state_row++) {
          curr_path = &trellis_tree_lkup[CURR_STAGE][state_row];

          if (!curr_path->is_alive) {
            continue;
          }

          cout << " | +- State: " << state_row << endl;
          cout << " | | +- Possible Trasitions: " << endl;

          // Iterate over all possible inputs
          for (int p_in = 0; p_in < (1 << input); p_in++) {
            lkup_address = (lookup_size - 1) & ((state_row << input) | p_in);
            next_state_index = ((lookup_size >> 1) - 1) & next_state_lkp[lkup_address].to_uint();
            viterbi_path_s<output, input, memory> * next_state = &trellis_tree_lkup[NEXT_STAGE][next_state_index];

            new_metrics = curr_path->metric_value;
            new_metrics += get_metrics(in_bus[input_selector / output].to_uint(), output_lkp[lkup_address].to_uint());

            cout << " | | | +- Input: " << std::bitset<output>(output_lkp[lkup_address].to_uint()) << endl;
            cout << " | | | | +- Next state: " << next_state_index << endl;
            cout << " | | | | +- Input Distance: " << get_metrics(in_bus[input_selector / output].to_uint(), output_lkp[lkup_address].to_uint()) << endl;
            cout << " | | | | +- Next state metric: " << new_metrics << endl;

            if (next_state->metric_value > new_metrics) {
              continue;
            }

            memcpy(next_state, curr_path, sizeof(viterbi_path_s<output, input, memory>));

            next_state->metric_value = new_metrics;
            // Push the state to the list (array)
            next_state->states_path[next_state->path_size++] = next_state_index;
          }


          cout << " | | +- Metrics: " << curr_path->metric_value << endl;

          if (curr_path->path_size > 0) {
            cout << " | | +- Path: " << endl;
            for (int i = 0; i < curr_path->path_size; i++) {
              cout << " | | | +- Node: " << curr_path->states_path[i] << endl;
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
      sc_lv<output_width> output_bus;
      viterbi_path_s<output, input, memory> curr_path = trellis_tree_lkup[CURR_STAGE][5];

      for (int state_row = 0; state_row < states_num; state_row++) {
        if (!trellis_tree_lkup[CURR_STAGE][state_row].is_alive) {
          continue;
        }

        if (trellis_tree_lkup[CURR_STAGE][state_row].metric_value > curr_path.metric_value) {
          curr_path = trellis_tree_lkup[CURR_STAGE][state_row];
        }
      }

      if (curr_path.path_size > 0) {
        cout << " +- Selected Path: " << endl;
        for (int i = 0; i < curr_path.path_size; i++) {
          cout << " | +- Node: " << curr_path.states_path[i] << endl;
        }
      }

      for (int path_item_index = 0; path_item_index < output_width; path_item_index++) {
        output_bus[output_width - path_item_index - 1] = (curr_path.states_path[path_item_index + 1] & 0x1) ? sc_logic('1') : sc_logic('0');
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
      trellis_tree_lkup[0][0].states_path[0] = 0;
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
