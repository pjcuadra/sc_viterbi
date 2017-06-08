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

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include<systemc.h>

/**
 * Shift register Template
 * @param width size of the shift register
 */
template<int inputs>
  SC_MODULE (serializer) {
    /** Input CLK */
    sc_in_clk clk_in;
    /** Parallel input */
    sc_in<sc_lv<inputs> > par_in;
    /** Serial output */
    sc_out<sc_logic> ser_out;
    /** Output selector */
    int output_selector;

    /**
     * Serializer Process
     */
    void prc_serializer () {
      sc_lv<inputs> latch;

      latch = par_in;

      ser_out = latch[output_selector];

      output_selector--;

      // If selector overflows reset the counter
      if (output_selector < 0) {
        output_selector = inputs - 1;
      }
    }

    /**
     * Constructor
     */
    SC_CTOR (serializer) {

      output_selector = inputs - 1;

      SC_METHOD (prc_serializer);
      sensitive << clk_in.pos();

    }
  };

#endif
