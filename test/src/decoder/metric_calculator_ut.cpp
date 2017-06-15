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
#include <decoder/metric_calculator.h>

//for testing-consistency between shift_Reg and convolution
static const int input_bit_width = 2;

/*SC_MODULE(testbench) {

  sc_signal<sc_logic> reset;
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);


  SC_CTOR(testbench) {
    SC_THREAD (stimuli);
    SC_THREAD (clk_gen);
  }

  void stimuli() {
    while(true) {
      reset.write(true);
      wait(10, SC_NS);
      reset.write(false);
      wait(250, SC_NS);
    }
  }
  void clk_gen() {
    while(true) {
      clk.write(true);
      wait(10, SC_NS);
      clk.write(false);
      wait(10, SC_NS);
}
}

};
*/


// SC_MODULE(scope) {
//   sc_in<bool> m_a, m_b, m_cin, m_sum, m_cout;
//
//   void prc_monitor() {
//     cout << "At time " << sc_time_stamp() << "::";
//     cout << " (a, b, carry_in): ";
//     cout << m_a << m_b << m_cin;
//     cout << " (sum, carry_out): ";
//     cout << m_sum << m_cout << endl;
//   }
//
//   SC_CTOR(scope) {
//     SC_METHOD(prc_monitor);
//     sensitive << m_a << m_b << m_cin << m_sum << m_cout;
//   }
// };

SC_TEST(metric_calculator) {

  // create channels
  sc_signal<sc_lv<input_bit_width> > received_bits;
  sc_signal<sc_lv<input_bit_width> > expected_bits;
  sc_signal<int> branch;
  sc_signal<int> path;
  sc_signal<int> predecessor_path;
  sc_signal<sc_lv<input_bit_width> > xor_output;//for testing only

  // create module
  metric_calculator<input_bit_width> metric_calculator("Metric Calculator");

  // connect channels to ports of "convolution"
  metric_calculator.received(received_bits);
  metric_calculator.expected_codeword(expected_bits);
  metric_calculator.branch_metric(branch);
  metric_calculator.predecessor_path_metric(predecessor_path);
  metric_calculator.path_metric(path);
  metric_calculator.xor_op(xor_output);//for testing only

  cout << "Metric Calculation" << endl;

  // create trace of signals
  SC_TRACE(received_bits, "received_bits");
  SC_TRACE(expected_bits, "expected_bits");
  SC_TRACE(xor_output, "hamming_metric");
  //SC_TRACE(branch, "branch metric");
  //SC_TRACE(predecessor_path, "predecessor path metric");
  //SC_TRACE(path, "path metric");

  // start simulation
  received_bits = "10";
  expected_bits = "11";
  predecessor_path = 4;
  sc_start(100, SC_NS);
  cout << "calculated path" << path << "calculated branch" << branch << endl;

  received_bits = "11";
  expected_bits = "10";
  predecessor_path = 5;
  sc_start(100, SC_NS);
  cout << "calculated path" << path << "calculated branch" << branch << endl;

  received_bits = "00";
  expected_bits = "10";
  predecessor_path = 4;
  sc_start(100, SC_NS);
  cout << "calculated path" << path << "calculated branch" << branch << endl;
}
