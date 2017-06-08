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
    /** Serializing trigger */
    sc_in<bool> ser_trig;
    /** Serial output */
    sc_out<sc_logic> ser_out;
    /** Output selector */
    uint output_selector;
    /** Serializing flag to serialize just once per trigger */
    bool serializing;

    /**
     * Serializer Process
     */
    void prc_serializer () {
      sc_lv<inputs> latch;

      latch = par_in;

      ser_out = serializing & latch[output_selector];

      if (output_selector == 0) {
        serializing =  false;
      }

      output_selector--;
    }

    /**
     * Serializer Process
     */
    void prc_serializer_trig_catch () {
      if (ser_trig) {
        output_selector = inputs - 1;
        serializing = true;
      }
    }

    /**
     * Constructor
     */
    SC_CTOR (serializer) {
      output_selector = 0;
      serializing = false;

      SC_METHOD (prc_serializer_trig_catch);
      sensitive << ser_trig;

      SC_METHOD (prc_serializer);
      sensitive << clk_in.pos();



    }
  };

#endif
