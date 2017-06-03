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
#include <common/full_adder.h>

SC_MODULE(stimulus) {
  sc_out<bool> d_a, d_b, d_cin;

  void prc_stimulus() {
    sc_uint<3> pattern;
    pattern = 0;

    while(true) {
      d_a = pattern[0];
      d_b = pattern[1];
      d_cin = pattern[2];
      wait(5, SC_NS);
      pattern++;
    }
  }

  SC_CTOR(stimulus) {
    SC_THREAD (prc_stimulus);
  }
  
};

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

SC_TEST(full_adder) {
  sc_signal<bool> t_a, t_b, t_cin, t_sum, t_cout;

  cout << "HELLO FULL ADDER" << endl;

  SC_TRACE(t_a, "t_a");
  SC_TRACE(t_b, "t_b");
  SC_TRACE(t_cin, "t_cin");
  SC_TRACE(t_sum, "t_sum");
  SC_TRACE(t_cout, "t_cout");

  full_adder f1 ("FullAdder");
  f1 << t_a << t_b << t_cin << t_sum << t_cout;

  stimulus s1 ("Stimulus");
  s1 << t_a << t_b << t_cin;

  sc_start(100, SC_NS);
}
