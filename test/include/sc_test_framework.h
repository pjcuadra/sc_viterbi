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

#ifndef SC_TEST_FRAMEWORK_H_
#define SC_TEST_FRAMEWORK_H_

#include<systemc.h>
#include<string>
#include<cstdio>

#ifdef TRACE_PATH
  #define _SC_TRACE_PATH TRACE_PATH
#else
  #define _SC_TRACE_PATH "/tmp/"
#endif

#define SC_TRACE(signal, name) sc_trace(this->tf, signal, name)
#define SC_STRACE(signal) sc_trace(this->tf, signal, #signal)

#define SC_TEST(test_name) static sc_unit_test * test = new sc_unit_test(#test_name); \
  int sc_main(int argc, char* argv[]) { \
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING); \
    test->run(); \
    delete(test); \
    return 0; \
  } \
  void sc_unit_test::run()

class sc_unit_test {
public:
  sc_core::sc_trace_file * tf;

  sc_unit_test(std::string test_name) {
    this->_test_name = test_name;
    _filename = _SC_TRACE_PATH;
    _filename.append(this->_test_name);

    cout << _filename.c_str();

    tf = sc_create_vcd_trace_file(_filename.c_str());
  }

  ~sc_unit_test() {
    sc_close_vcd_trace_file(tf);
  }

  void run();

  std::string get_name() {
    return this->_test_name;
  }

private:
  std::string _test_name;
  std::string _filename;

};

#endif
