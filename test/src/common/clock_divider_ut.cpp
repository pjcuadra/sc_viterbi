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
#include <common/clock_divider.h>

static const int clock_period = 14;
static const double clock_duty = 0.5;
static const int clock_start = 20;

SC_TEST(clock_divider) {
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);
  sc_signal<bool> div_clock_3;
  sc_signal<bool> div_clock_4;

  SC_TRACE(sys_clock, "sys_clock");
  SC_TRACE(div_clock_3, "div_clock_3");
  SC_TRACE(div_clock_4, "div_clock_4");

  clock_divider<3> div_3 ("DivideBy3");
  clock_divider<4> div_4 ("DivideBy4");

  div_3.clk_in (sys_clock);
  div_4.clk_in (sys_clock);
  div_3.clk_out (div_clock_3);
  div_4.clk_out (div_clock_4);

  sc_start(350, SC_NS);

}
