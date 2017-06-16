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

#ifndef DECODER_LKUP_H_
#define DECODER_LKUP_H_

#include<systemc.h>
#include<common/util.h>
#include <decoder/viterbi_path.h>

#define CURR_STAGE 0
#define NEXT_STAGE 1
#define MAX_STAGES 2

template<int output, int input, int memory, int output_buffer_bit_size>
  SC_MODULE (decoder_lkup) {

    // Constants
    /** Lookup table size */
    const static uint lookup_size = input << memory;
    /** Number of states per trellis diagram stage */
    static const uint states_num = input << (memory - 1);

    /** Trellis State Table */
   viterbi_path_s<output_buffer_bit_size> trellis_tree_lkup[MAX_STAGES][states_num]; //8

    // Inputs
    sc_in<viterbi_path_s<output_buffer_bit_size> > vp_in_state;
    sc_in<sc_uint<memory + input> > lkup_address; //5
    sc_in<bool> write_lkup;
    sc_in<bool> read_lkup;
    sc_in<bool> stage_selector;
    sc_in<bool> shift_trigger;
    sc_in<bool> reset;

    // Outputs
    /** Serial output */
    sc_out<viterbi_path_s<output_buffer_bit_size> > vp_out_state;

    /**
     * Read data from Look-Up table depending upon stage_selector
     */
    void prc_read_state() {
      //viterbi_path_s<output_buffer_bit_size> tmp = trellis_tree_lkup[NEXT_STAGE][lkup_address.read( ).to_uint()];
      if (stage_selector){
        //vp_out_state.write(tmp);
        vp_out_state.write(trellis_tree_lkup[NEXT_STAGE][lkup_address.read( ).to_uint()]);
      }
      else{
        //vp_out_state.write(tmp);
        vp_out_state.write(trellis_tree_lkup[CURR_STAGE][lkup_address.read( ).to_uint()]);
      }
    }

    /**
     * Write data from input to the Look-Up table depending upon stage_selector
     */
    void prc_write_state() {
      // viterbi_path_s<output_buffer_bit_size> tmp1 = trellis_tree_lkup[NEXT_STAGE][lkup_address.read( ).to_uint()];
      viterbi_path_s<output_buffer_bit_size> tmp = vp_in_state.read();
      if (stage_selector){
        // trellis_tree_lkup[NEXT_STAGE][lkup_address.write(tmp).to_uint()];
        trellis_tree_lkup[NEXT_STAGE][lkup_address.read( ).to_uint()] = tmp;
      }
      else{
      //   trellis_tree_lkup[CURR_STAGE][lkup_address] = vp_in_state.read();
        trellis_tree_lkup[CURR_STAGE][lkup_address.read( ).to_uint()] = tmp;
      }
    }

    /**
     * Transit data from one state to the other on shift_trigger
     */
    void prc_shift() {
      for (uint state_row = 0; state_row < states_num; state_row++) {
        trellis_tree_lkup[NEXT_STAGE][state_row] = trellis_tree_lkup[CURR_STAGE][state_row];
      }
    }


    /**
     * Flush out data of lookup table
     */
    void prc_flush_lookup() {
      for (int stage = 0; stage < MAX_STAGES; stage++) {
        for (int state_row = 0; state_row < states_num; state_row++) {
          trellis_tree_lkup[stage][state_row].path_size = 0;
          trellis_tree_lkup[stage][state_row].metric_value = 0;
          trellis_tree_lkup[stage][state_row].path_output = 0;
          trellis_tree_lkup[stage][state_row].is_alive = false;
        }
      }
    }


    /**
     * Constructor
     */
    SC_CTOR (decoder_lkup) {
    /** Methods with Sensitivity list */
      SC_METHOD(prc_read_state);
      dont_initialize();
      sensitive << read_lkup << stage_selector;

      SC_METHOD(prc_write_state);
      dont_initialize();
      sensitive << write_lkup << stage_selector;

      SC_METHOD(prc_shift);
      dont_initialize();
      sensitive << shift_trigger;

      SC_METHOD(prc_flush_lookup);
      dont_initialize();
      sensitive << reset;

    }
  };

#endif
