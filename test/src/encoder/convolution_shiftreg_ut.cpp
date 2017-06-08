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

#include <sc_test_framework.h>
#include <encoder/convolution.h>
#include <common/shift_register.h>

//for testing-consistency between shift_Reg and convolution
static const int reg_width = 3;

static const int clock_period = 15;
static const double clock_duty = 0.5;
static const int clock_start = 20;

SC_TEST(convolution_shiftreg) {
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

  // create channels
  sc_signal<sc_lv<reg_width> > shift_reg; //logic vector for shift register
  sc_signal<sc_lv<reg_width> > generator_polynomial; //logic vector for shift register
  sc_signal<sc_logic> conv_output; //logic output of output of convolution
  sc_signal<sc_logic> data_in;

  shift_register<reg_width> shift_register("Reg");
  shift_register.clk(sys_clock);
  shift_register.data_in(data_in);
  shift_register.q (shift_reg);

  // create module
  convolution<reg_width> convolution("Convolution");

  // connect channels to ports of "convolution"
  convolution.input(shift_reg);
  convolution.polynomial(generator_polynomial);
  convolution.y(conv_output);

  cout << "convolution Block(2,1,3)" << endl;

  // create trace of signals
  SC_TRACE(shift_reg, "shift_Reg");
  SC_TRACE(conv_output, "conv_output");
  SC_TRACE(data_in, "data_in");

  data_in = sc_logic('0');
  generator_polynomial = "101";
  sc_start(100, SC_NS);

  data_in = sc_logic('1');
  sc_start(clock_period, SC_NS);

  data_in = sc_logic('0');
  sc_start(4*clock_period, SC_NS);



}
