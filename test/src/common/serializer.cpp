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
#include <common/serializer.h>

static const int clock_period = 14;
static const double clock_duty = 0.5;
static const int clock_start = 20;
static const int inputs = 8;

SC_TEST(serializer) {
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);
  sc_signal<sc_logic> ser_out;
  sc_signal<sc_lv<inputs> > par_in;

  par_in = "10010010";

  SC_TRACE(sys_clock, "sys_clock");
  SC_TRACE(ser_out, "ser_out");
  SC_TRACE(par_in, "par_in");

  serializer<inputs> serializer ("Serializer");
  serializer.ser_out(ser_out);
  serializer.clk_in(sys_clock);
  serializer.par_in(par_in);

  sc_start(350, SC_NS);

}
