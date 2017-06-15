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
#include <decoder/decoder_viterbi.h>
#include <sstream>

static const int n = 2;
static const int k = 1;
static const int m = 4;
static const int clock_period = 15;
static const int out_buff = 16;
static const double clock_duty = 0.5;
static const int clock_start = 20;
static const double stimulus_start = 200;
static const int output_size = n * (2* m - k);


SC_TEST(decoder) {
  // uint current_check_time = stimulus_start;
  // Create system clokc
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

  // Create signals
  sc_signal<sc_logic> in;
  sc_signal<sc_logic> out;
  sc_lv<4> expected_out;
  sc_signal<sc_lv<m> > polynomials[n];
  sc_lv<output_size> in_bus;
  sc_signal<bool> trigger;
  uint current_check_time;

  in_bus = "11110111010111";

  expected_out = "1011";

  // Create module
  decoder_viterbi<n, k, m, out_buff> vdecoder("ViterbiDecoder");

  // Assign polynomials
  polynomials[0] = "1111";
  polynomials[1] = "1101";

  // Trace all signals
  SC_STRACE(vdecoder.clk);
  SC_STRACE(vdecoder.in);
  SC_STRACE(vdecoder.out);
  SC_STRACE(vdecoder.trigger);
  SC_STRACE(vdecoder.par_in);
  SC_STRACE(vdecoder.clk_div);
  SC_STRACE(vdecoder.serializing);
  SC_STRACE(vdecoder.decoding);


  for (int i = 0; i < n; i++) {
    std::stringstream pol_name;

    pol_name << "polynimial(" << i << ")";

    SC_TRACE(polynomials[i], pol_name.str());
    vdecoder.polynomials[i](polynomials[i]);
  }

  vdecoder.clk(sys_clock);
  vdecoder.in(in);
  vdecoder.out(out);
  vdecoder.trigger(trigger);

  for (int i = 0; i < 8; i++) {
    std::stringstream trellis_node;

    trellis_node << "trellis.state(" << i << ")";

    SC_TRACE(vdecoder.trellis_tree_lkup[0][i], trellis_node.str().c_str());
  }

  // Output verification (1011)
  current_check_time = 312;
  SC_EXPECT_AT(sc_logic('0'), out, current_check_time, SC_NS);
  current_check_time += clock_period;
  for (int i = 0; i < m; i++) {
    SC_EXPECT_AT(sc_logic(expected_out.get_bit(m - i -1)), out, current_check_time, SC_NS);
    current_check_time += clock_period;
  }

  current_check_time = 1010;
  SC_EXPECT_AT(sc_logic('0'), out, current_check_time, SC_NS);
  current_check_time += clock_period;
  for (int i = 0; i < m; i++) {
    SC_EXPECT_AT(sc_logic(expected_out.get_bit(m - i -1)), out, current_check_time, SC_NS);
    current_check_time += clock_period;
  }


  trigger = false;
  in = sc_logic('0');
  sc_start(50, SC_NS);
  trigger = true;
  for (int i = 0; i < output_size; i++) {
    in = in_bus[output_size - i - 1];
    sc_start(clock_period, SC_NS);
  }

  in = sc_logic('0');

  sc_start(50, SC_NS);
  trigger = false;


  sc_start(490, SC_NS);

  in_bus = "01100111010110";

  trigger = true;
  for (int i = 0; i < output_size; i++) {
    in = in_bus[output_size - i - 1];
    sc_start(clock_period, SC_NS);
  }
  in = sc_logic('0');

  sc_start(5, SC_NS);
  trigger = false;

  sc_start(500, SC_NS);

}
