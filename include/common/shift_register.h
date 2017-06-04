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

#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#include<systemc.h>

/**
 * Shift register Template
 * @param width size of the shift register
 */
template<int width>
  SC_MODULE (shift_register) {
    /** Input CLK */
    sc_in_clk clk;
    /** Data in */
    sc_in<sc_logic> data_in;
    /** Stored value output */
    sc_out<sc_lv<width> > q;
    /** Stored value state */
    sc_lv<width> q_state;

    /**
     * Shift register Process
     */
    void prc_shift_register () {
      // Shift value
      q_state.range(1, width - 1) = q_state.range(0, width - 2);
      // Take data into the register's state
      q_state[0] = data_in;
      // Link state to the output
      q = q_state;
    }

    /**
     * Constructor
     */
    SC_CTOR (shift_register) {
      for (int i = 0; i < width; i++) {
        q_state[i] = sc_logic('0');
      }
      SC_METHOD (prc_shift_register);
      sensitive_pos << clk;
    }
  };

#endif
