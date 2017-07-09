Shift Register
--------------

Class Diagram
*************

The UML diagram of :numref:`shift_register_class` shows the overview of the
class.

.. _shift_register_class:
.. uml::
  :align: center
  :caption: Shift Register Class Diagram

  @startuml
    class sc_core::sc_module

    class shift_register<int width> {
      .. Inputs ..
      sc_in_clk clk
      sc_in<sc_logic> data_in

      .. Outputs ..
      sc_out<sc_lv<width> > q

      .. Variables ..
      sc_lv<width> q_state

      __ Processes __
      prc_shift_register()

    }
    shift_register -up-|> sc_core::sc_module
  @enduml


Class Description
*****************

.. cpp:class:: template<int width> shift_register

  Clock Divider module with clock division ratio

  .. cpp:var:: sc_core::sc_in_clk clk

    Input clock

  .. cpp:var:: sc_core::sc_in<sc_core::sc_logic> data_in

    Input data to be shifted in

  .. cpp:var:: sc_core::sc_out<sc_core::sc_lv<width> > q

    Stored value of the shift register

  .. cpp:var:: sc_core::sc_lv<width> q_state

    Internal stored value

  .. cpp:function:: void prc_shift_register(void)

    Shift register main process

    Shifts the value one bit to the left and assign data_in to the MSb.

    .. cpp:var:: list sensitivity

      clk.pos()

Structure
*********

:numref:`shift_register_circuit` shows the structure of the generic shift
register. Our implementation doesn't differ from the generic shift register.

.. _shift_register_circuit:
.. figure:: ../_static/shift_register_circuit.png
  :align: center

  Shift Register Circuit

Simulation Results
******************

The code of the test case of the shift_register is shown below;

.. code-block:: cpp
  :linenos:

  ...

  static const int reg_width = 5;

  SC_TEST(shift_register) {
    sc_signal<sc_logic> data_in;
    sc_signal<sc_lv<reg_width> > register_state;
    sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

    ...

    shift_register<reg_width> sregister ("ShiftRegister");

    ...

    data_in = sc_logic('0');
    sc_start(50, SC_NS);
    data_in = sc_logic('1');
    sc_start(100, SC_NS);
    data_in = sc_logic('0');
    sc_start(100, SC_NS);
  }

.. note::
  * `data_in` starts at `sc_logic('0')`
  * `data_in` toggles to `sc_logic('1')` at :math:`50ns`
  * `data_in` toggles back to `sc_logic('0')` at :math:`100ns`
  * Shift Register width is :math:`5`

:numref:`shift_register_sim_wave` shows the result of the simulation.

.. _shift_register_sim_wave:
.. figure:: ../_static/shift_register_simulation.png
  :align: center

  Shift Register Simulation Wave Result

.. note::

  * At :math:`50ns` the shifting of `sc_logic('1')` starts. The `sc_logic('1')`
    is injected at the MSb.
  * Between :math:`120ns` and :math:`150ns` the value of `register_state[4:0]`
    stays constant at :math:`0x1F` because of the shift register's width is set
    to :math:`5`.
  * After :math:`150ns` the shifting of `sc_logic('0')` starts
  * 5 clock cycles after :math:`150ns` the `register_state[4:0]` is back to
    :math:`0x0`.
