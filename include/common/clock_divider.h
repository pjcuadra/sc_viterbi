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

#ifndef CLOCK_DEVIDER_H_
#define CLOCK_DEVIDER_H_

#include<systemc.h>

/**
 * Shift register Template
 * @param width size of the shift register
 */
template<int ratio>
  SC_MODULE (clock_divider) {
    /** Input CLK */
    sc_in_clk clk_in;
    /** Output CLK */
    sc_out_clk clk_out;
    /** Devider Counter */
    int divider_counter;

    /**
     * Shift register Process
     */
    void prc_clock_divider () {
      if (divider_counter == 2 * ratio) {
        clk_out = true;
        divider_counter = 1;
        return;
      }

      if ((divider_counter >= ratio) && clk_out) {
        clk_out = false;
      }

      divider_counter++;
    }

    /**
     * Constructor
     */
    SC_CTOR (clock_divider) {
      divider_counter = 2 * ratio;
      SC_METHOD (prc_clock_divider);
      sensitive << clk_in;
    }
  };

#endif
