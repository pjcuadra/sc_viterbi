Testing Framework
=================

Since there is no robust and reliable unit testing framework for SystemC code,
we decide to implement our own testing framework. The goals for the design of
the testing framework were;

* Similar to other existing unit testing frameworks like
  `googletest <https://github.com/google/googletest>`_ and
  `Junit <http://junit.org/junit4/>`_.
* Intuitive timing specifications checking APIs
* Centralized mechanism to run all unit tests

This goals helped us to overcome the following disadvantages inherit from
SystemC traditional testing methodologies;

* `sc_start()` and `sc_stop()` calls affect the entire process.
* There's no API to reset simulation time
* Every unit test has to be written inside a `sc_main(...)`. Meaning a separate
  executable per test.
* An entire `SC_MODULE` has to be developed for introducing stimuli to the
  `Module Under Test` and an other for verifying it's output.

Based on this goals we developed a unit testing framework with the following
features;

* Centralized method to run all tests through `CTest`. Review
  `ctest <https://cmake.org/cmake/help/v3.0/manual/ctest.1.html>`_ for a full
  set of features and capabilities inherited from `ctest`.
* Easy, Intuitive, and Powerful building system.
* Intuitive timing specification checking.
* Simplified test writing structure.
* Simplified tracing APIs and automatic `VCD` files creation.
* Easy to integrate C++ headers library.

Testing Building System
-----------------------

The our building system for testing is based on our project's
`Building System <building_system.html>`_. The entire tests building system is
based on the following `CMake` macros;

.. code::

  # Add test sources directory
  add_test_sources_directory(dir1 dir2 ...)

  # Add test source file
  add_test_sources(file1 file2 ...)

The usage of both macros is analog to the usage of `add_sources_directory(...)`
and `add_sources(...)`, respectively. Review
`Building System <building_system.html>`_ for more information.

.. note::

  All tests source code have to be located at
  `<your-sc_viterbi-src-code-path>/test/src`.

Furthermore, `add_test_sources(...)` also registers every test source file as a test
in `CTest`. This makes it easier for test running through `ctest` command or
`make test`.

APIs
----

.. c:macro:: SC_TEST(test_name)

  Define a test with a given test name

  :param string test_name: The name or label of the test

.. c:macro:: SC_EXPECT_AT(expected, actual, time_val, time_unit)

  Check that actual equals expected at a given moment

  :param expected: Expected value
  :param actual: Actual value
  :param double time_val: Time value
  :param sc_time_unit time_unit: Time unit


Tracing
*******

.. c:macro:: SC_TRACE(signal, name)

   Add a signal to the test's trace file with the provided name

   :param sc_signal signal: The signal to be added to the trace
   :param string name: The given name for the signal

.. c:macro:: SC_STRACE(signal)

  Add a signal to the test's trace file

  :param sc_signal signal: The signal to be added to the trace (the name of the
                           signal will be name of the variable)


Test Example
------------

The following code was taken from our clock divider's test. It is a good
example to show how our testing framework works.

.. code-block:: cpp
  :linenos:

  #include <sc_test_framework.h>

  ...

  SC_TEST(clock_divider) {
    sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);
    sc_signal<bool> div_clock_4;

    # Trace signals
    SC_TRACE(sys_clock, "sys_clk");
    SC_STRACE(div_clock_4);

    # Create clock divider
    clock_divider<4> div_4 ("DivideBy4");

    # Connect Signals
    div_4.clk_in (sys_clock);
    div_4.clk_out (div_clock_4);

    # Verify output at a certain point in time
    SC_EXPECT_AT(sc_logic(0), div_clock_4, 200, SC_NS);
    SC_EXPECT_AT(sc_logic(1), div_clock_4, 220, SC_NS);

    sc_start(350, SC_NS);

  }

Here is a short explanation on what the code does;

* In `line 1` the framework headers library is included.
* In `line 5` the test is defined with the name `clock_divider`.
* In `line 10` the signal sys_clock is added to the trace file with a given name
  that differs from the `sc_signal` variable name.
* In `line 11` the signal `div_clock_4` is added to the trace file using with
  with the name `div_clock_4`.
* In `line 21` and `22` the time specification for the signal `div_clock_4` is
  done. If the signal `div_clock_4` differs from '0' at 200ns or from '1' at
  220ns the test will fail.


Future Work
-----------

This testing framework is planned to be developed as a separate project because
of its usability and scope. It was incubated in within this project.

APIs
****

.. c:macro:: SC_EXPECT_AFTER(expected, actual, time_val, time_unit)

  Check that actual equals expected after a specific time interval

  :param expected: Expected value
  :param actual: Actual value
  :param double time_val: Time value
  :param sc_time_unit time_unit: Time unit

.. c:macro:: SC_SIGNAL_SET(signal, value, time_val, time_unit)

  Set the signal to a specific value at a specific point in time

  :param signal: The signal to be set
  :param value: The value to be taken
  :param double time_val: Time value
  :param sc_time_unit time_unit: Time unit

.. c:macro:: SC_SETUP()

  Set up everything before the test run

.. c:macro:: SC_TEARDOWN()

  Clean everything up after the test run
