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
#include <common/comparator.h>

//for testing-consistency between shift_Reg and convolution
//static const int input_bit_width = 2;

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

SC_TEST(comparator) {

  // create channels
  sc_signal<int> first_entity;
  sc_signal<int> second_entity;
  sc_signal<int> max;

  // create module
  comparator<int,int> comparator("comparator");

  // connect channels to ports of "convolution"
  comparator.first_entity(first_entity);
  comparator.second_entity(second_entity);
  comparator.maximum(max);

  cout << "Comparison" << endl;

  // create trace of signals
  SC_TRACE(first_entity, "first_entity");
  SC_TRACE(second_entity, "second_entity");
  SC_TRACE(max, "maximum_entity");

  // start simulation
  first_entity = 5;
  second_entity = 4;
  sc_start(100, SC_NS);
  cout << "Compared and maximum is:" << max << endl;

  first_entity = 3;
  second_entity = 4;
  sc_start(100, SC_NS);
  cout << "Compared and maximum is:" << max << endl;

}
