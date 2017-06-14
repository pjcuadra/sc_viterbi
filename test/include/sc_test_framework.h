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
#include<list>

#ifdef TRACE_PATH
  #define _SC_TRACE_PATH TRACE_PATH
#else
  #define _SC_TRACE_PATH "/tmp/"
#endif

#define SC_TRACE(signal, name) sc_trace(this->tf, signal, name)
#define SC_STRACE(signal) sc_trace(this->tf, signal, #signal)

#define SC_TEST(test_name) static sc_unit_test * test = new sc_unit_test(#test_name); \
  static uint _expect_at_counter = 0; \
  std::list<void *> list_of; \
  static bool failed = false; \
  int sc_main(int argc, char* argv[]) { \
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING); \
    test->run(); \
    delete(test); \
    list_of.clear(); \
    if (failed) { \
        return 1; \
    } \
    return 0; \
  } \
  void sc_unit_test::run()

#define SSTR( x ) static_cast< std::ostringstream & >( \
          ( std::ostringstream() << std::dec << x ) ).str().c_str()

#define SC_EXPECT_AT_NAME(var, count) expect_at ## count

#define SC_EXPECT_AT(expected, actual_val, time_val, time_unit) \
  create_expect_check(expected, time_val, time_unit, _expect_at_counter, list_of, &failed)->actual(actual_val);\


template <class T>
SC_MODULE (sc_expect_check) {
  /** Parallel input */
  sc_in<T> actual;
  T expected;
  sc_event check;
  sc_time check_time;
  sc_time_unit step_time_unit;
  uint file_line;
  bool *failed;

  /**
   * Check the value at given time
   */
  void prc_check_value () {
    if (sc_time_stamp() == this->check_time) {
      // If value doesn't match throw an exception
      if (actual.read() != this->expected) {
        cout << "\033[1;31m[FAILED]\033[0m Missmatch Exception: expected " << expected << ", actual " << actual  << " (@ time " << sc_time_stamp() << ")" << endl;
        *failed |= true;
      }

      // Don't keep checking after time has passed
      return;
    }

    next_trigger(1, step_time_unit);
  }

  SC_HAS_PROCESS(sc_expect_check);

  /**
   * Constructor
   */
  sc_expect_check(sc_module_name name, double check_time, sc_time_unit time_unit, T expected, bool *failed) {
   this->check_time = sc_time(check_time, time_unit);
   this->expected = expected;
   this->step_time_unit = time_unit;
   this->failed = failed;

   SC_METHOD (prc_check_value)
 }
};


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


template <class T>
  sc_expect_check<T> * create_expect_check (const T expected, double time_val, sc_time_unit time_unit, uint &counter, std::list<void *> &list_of, bool * failed) {
    sc_expect_check<T> * _expect_at;
    std::stringstream expect_name;

    expect_name.str("expect_check_");
    expect_name << counter;
    _expect_at = new sc_expect_check<T>(expect_name.str().c_str(), time_val, time_unit, expected, failed);
    // _expect_at->actual(actual);

    list_of.push_front((void *)_expect_at);

    counter++;

    return _expect_at;
  }

#endif
