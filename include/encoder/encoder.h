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

#ifndef ENCODER_H_
#define ENCODER_H_

#include<systemc.h>
#include<common/shift_register.h>

template<int output, int input, int memory>
  SC_MODULE (encoder) {
    /** Input CLK */
    sc_in_clk clk;
    /** Parallel input */
    sc_in<sc_lv<input> > in;
    /** Serial output */
    sc_out<sc_logic > out;
    /** Shift Registers */
    shift_register<memory> register_bank[input];

    SC_CTOR (encoder) {

      // Connect the input and clock of each shift register
      for (int i = 0; i < input; i++) {
        register_bank[i].data_in = in[i];
        register_bank[i].clk = clk;
      }
    }
  };

#endif
