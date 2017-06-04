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

//for testing-consistency between shift_Reg and convolution
static const int clock_period = 14;
static const double clock_duty = 0.5;
static const int clock_start = 20;

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

SC_TEST(convolution) {

  // create channels
  sc_signal<sc_lv<_M_PARAM> > shift_Reg; //logic vector for shift register
  sc_signal<sc_lv<_OUTPUT_PARAM> > conv_output; //logic output of output of convolution
  sc_signal<sc_logic> reset_sig;
  sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

  // create modules
  convolution convolution("convolution");
  //testbench testbench("testbench");

  // connect channels to ports of "convolution"
  convolution.mem(shift_Reg);
  convolution.y(conv_output);
  convolution.clk(sys_clock);
  convolution.reset(reset_sig);

  // connect channels to ports of "testbench"
  //testbench.clk(clk_sig);
  //testbench.reset(reset_sig);


  cout << "convolution Block(2,1,3)" << endl;

  // create trace of signals
  SC_TRACE(shift_Reg, "shift_Reg");
  SC_TRACE(conv_output, "conv_output");
  SC_TRACE(sys_clock, "sys_clock");
  SC_TRACE(reset_sig, "reset_sig");

  // start simulation
  shift_Reg = "000";
  sc_start(50, SC_NS);
  cout << "Shift and Convolved" << conv_output << endl;
  shift_Reg = "100";
  sc_start(100, SC_NS);
  cout << "Shift and Convolved" << conv_output << endl;
  shift_Reg = "110";
  sc_start(100, SC_NS);
}
