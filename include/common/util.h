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

#ifndef UTIL_H_
#define UTIL_H_

#include<systemc.h>

/**
 * Invert the bits order
 */
template <int size>
  inline sc_uint<size> swap_uint(sc_uint<size> in) {
    sc_uint<size> swap_in;

    for (int i = 0; i < size; i++) {
      swap_in[i] = in[size - i - 1];
    }

    return swap_in;
  }


  /**
   * Build the states lookup table
   */
template <int output, int input, int memory>
  inline void create_states_lkup(sc_lv<memory * input> * next_state_lkp) {

    const static uint lookup_size = input << memory;

    // Initialize the lookup table
    for (uint i = 0; i < lookup_size; i++) {
      uint in_tmp = i & ((1 << input) - 1);
      uint state = i >> input;
      next_state_lkp[i] = (state << input) | in_tmp;
    }
  }

  /**
   * Build the output lookup table
   */
template <int output, int input, int memory>
  inline void create_output_lkup(sc_lv<memory * input> polynomials[output], sc_lv<output> * output_lkp) {

    const static uint lookup_size = input << memory;

    // Interate over all entries of the lookup table
    for (uint mem = 0; mem < lookup_size; mem++) {
      sc_lv<output> tmp_out;

      for (uint o = 0; o < output; o++) {
        sc_lv<memory * input> sout;
        sc_uint<memory * input> p;
        sc_uint<memory * input> p_inv;

        // Cast and swap bit order
        p = static_cast< sc_uint<memory * input> > (polynomials[output - o - 1]);
        p_inv = swap_uint<memory * input>(p);

        // And both signals
        sout = (p_inv & mem);

        // Xor reduce
        tmp_out[o] = sout.xor_reduce();
      }

      // Set the lkup table entry
      output_lkp[mem] = tmp_out;
    }

  }


#endif
