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
#include <common/shift_register.h>

static const int clock_period = 14;
static const double clock_duty = 0.5;
static const int clock_start = 20;
static const int reg_width = 5;

SC_TEST(shift_register) {
  sc_signal<sc_logic> data_in;
  sc_signal<sc_lv<reg_width> > register_state;
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

  SC_TRACE(sys_clock, "sys_clock");
  SC_TRACE(register_state, "register_state");
  SC_TRACE(data_in, "data_in");

  shift_register<reg_width> sregister ("FullAdder");
  sregister.data_in (data_in);
  sregister.clk (sys_clock);
  sregister.q (register_state);

  data_in = sc_logic('0');
  sc_start(50, SC_NS);
  data_in = sc_logic('1');
  sc_start(100, SC_NS);
  data_in = sc_logic('0');
  sc_start(100, SC_NS);
}
