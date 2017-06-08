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
#include <encoder/encoder.h>
#include <sstream>

static const int n = 2;
static const int k = 1;
static const int m = 4;
static const int clock_period = 15;
static const double clock_duty = 0.5;
static const int clock_start = 20;


SC_TEST(encoder) {
  // Create system clokc
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

  // Create signals
  sc_signal<sc_lv<k> > in; //logic vector for shift register
  sc_signal<sc_logic> out; //logic output of output of convolution
  sc_signal<sc_lv<m> > mem_bus[k]; //logic vector for shift register
  sc_signal<sc_lv<m * k> > mem_bus_conv; //logic vector for shift register
  sc_signal<sc_logic> serial_in_drv[n];
  sc_signal<sc_lv<m> > polynomials[n];

  // Create module
  encoder<n, k, m> vencoder("ViterbiEncoder");

  // Assign polynomials
  polynomials[0] = "1111";
  polynomials[1] = "1101";

  SC_TRACE(sys_clock, "sys_clock");
  SC_TRACE(in, "in");
  SC_TRACE(out, "out");
  SC_TRACE(mem_bus_conv, "mem_bus_conv");

  for (int i = 0; i < n; i++) {
    std::stringstream pol_name;
    pol_name << "polynimial(" << i << ")";

    SC_TRACE(polynomials[i], pol_name.str());
    vencoder.polynomials[i](polynomials[i]);
  }

  vencoder.clk(sys_clock);
  vencoder.in(in);
  vencoder.out(out);
  vencoder.mem_bus_conv(mem_bus_conv);

  // Impulse Response
  in = "0";
  sc_start(200, SC_NS);

  in = "1";
  sc_start(2*clock_period, SC_NS);

  in = "0";
  sc_start(2*clock_period, SC_NS);

  in = "1";
  sc_start(2*clock_period, SC_NS);

  in = "1";
  sc_start(2*clock_period, SC_NS);

  in = "0";
  sc_start(200, SC_NS);


}
