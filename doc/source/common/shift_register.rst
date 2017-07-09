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

TODO

Simulation Results
******************

TODO
