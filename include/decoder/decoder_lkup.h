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

template<int output, int input, int memory>
  SC_MODULE (decoder_lkup) {
    // Inputs
    /** Input CLK */
    //sc_in_clk clk;
    /** Parallel input */
    //sc_in<sc_lv<input> > in;
    /** Polynomials input */
    //sc_in<sc_lv<memory * input> > polynomials[output];
    // Outputs
    /** Serial output */
    //sc_out<sc_logic > out;
    // Internal Signals
    /** Current State */
    //sc_lv<memory * input> curr_state;
    /** Next state lookup table */
    //sc_lv<memory * input> next_state_lkp[lookup_size];
    /** Output lookup table */
    //sc_lv<output> output_lkp[lookup_size];
    /** Clock divider counter */
    //uint div_counter;

    /** Lookup table size */
    const static uint lookup_size = input << memory;

    // Inputs
    sc_in<sc_uint<32>> vp_in_state;
    sc_in<sc_uint<memory + input>> lkup_address;
    sc_in<bool> write;
    sc_in<bool> read;
    sc_in<bool> input_selector;

    // Outputs
    /** Serial output */
    sc_in<sc_uint<32>> vp_out_state;


    /**
     * Transit from one state to the other depending on input and current state
     */
    // void prc_state_trasition() {
    //   sc_uint<memory * input> curr_state_int = static_cast< sc_uint<memory * input> > (curr_state);
    //   sc_uint<memory + input> lkup_address;
    //   sc_lv<input> in_tmp;
    //
    //   // Serialize the output
    //   out = output_lkp[curr_state.to_uint()][div_counter];
    //
    //   // Divide the input clock by number of outputs
    //   if (div_counter < (output - 1)) {
    //     div_counter++;
    //     return;
    //   }
    //
    //   // Temp store the input
    //   in_tmp = in.read();
    //
    //   // Build the lookup address
    //   lkup_address = (lookup_size - 1) & ((curr_state_int << input) | in_tmp.to_uint());
    //
    //   // Get the next state
    //   curr_state = next_state_lkp[lkup_address];
    //
    //   // Reset clock divider counter
    //   div_counter = 0;
    //
    // }
    //
    // /**
    //  * Build the output lookup table based on polynimials
    //  */
    // void prc_update_output_lkup() {
    //   sc_lv<memory * input> polynomials_tmp[output];
    //
    //   // Invert the order of bits
    //   for (int o = 0; o < output; o++) {
    //     polynomials_tmp[o] = polynomials[output - o -1];
    //   }
    //
    //   create_output_lkup<output, input, memory>(polynomials_tmp, output_lkp);
    // }

void prc_read_state() {

}

void prc_write_state() {

}

    /**
     * Constructor
     */
    SC_CTOR (decoder_lkup) {
      // Initialize the lookup table
      // create_states_lkup<output, input, memory>(next_state_lkp);

      // div_counter = 0;
      // curr_state = sc_lv<memory * input> (sc_logic_0);
      //
      // SC_METHOD(prc_state_trasition)
      // sensitive << clk.pos();
      //
      // SC_METHOD(prc_update_output_lkup)
      // for (int i = 0; i < output; i++) {
      //   sensitive << polynomials[i];
      // }
      SC_METHOD(prc_read_state)
      sensitive << read;

      SC_METHOD(prc_write_state)
      sensitive << write;
      //
    }
  };

#endif
