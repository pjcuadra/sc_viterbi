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
    sc_in_clk ser_trig;
    /** Serial output */
    sc_out<sc_logic> ser_out;
    /** Output selector */
    int output_selector;

    /**
     * Serializer Process
     */
    void prc_serializer () {
      sc_lv<inputs> latch;

      // Wait for the parallel input to stabilize
      next_trigger(1, SC_NS);

      latch = par_in.read();

      ser_out.write(latch[output_selector]);

      if (output_selector == -1) {
        ser_out = sc_logic('0');
        return;
      }

    }

    /**
     * Update the counter
     */
    void prc_serializer_counter_update () {
      if (output_selector == -1) {
        return;
      }

      output_selector--;
    }

    /**
     * Reset the counter
     */
    void prc_serializer_counter_reset () {
      output_selector = inputs - 1;
    }

    /**
     * Constructor
     */
    SC_CTOR (serializer) {
      output_selector = inputs;

      SC_METHOD (prc_serializer)
      sensitive << clk_in.pos() << ser_trig.pos();

      SC_METHOD (prc_serializer_counter_update)
      sensitive << clk_in.pos();

      SC_METHOD (prc_serializer_counter_reset)
      sensitive << ser_trig.pos();

    }
  };

#endif
