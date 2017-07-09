Clock Divider
-------------

Class Diagram
*************

The UML diagram of :numref:`clock_divider_class` shows the overview of the
class.

.. _clock_divider_class:
.. uml::
  :align: center
  :caption: Clock Divider Class Diagram

  @startuml
    class sc_core::sc_module

    class clock_divider<int ratio> {
      .. Inputs ..
      sc_in_clk clk_in

      .. Outputs ..
      sc_out_clk clk_out

      .. Variables ..
      int divider_counter

      __ Processes __
      prc_clock_divider()

    }
    clock_divider -up-|> sc_core::sc_module
  @enduml


Class Description
*****************

.. cpp:class:: template<int ratio> clock_divider

  Clock Divider module with clock division ratio

  .. cpp:var:: sc_core::sc_in_clk clk_in

    Input clock at normal rate

  .. cpp:var:: sc_core::sc_out_clk clk_out

    Output clock at rate of the input clock divided by ratio with the same duty
    cycle as the input clock.

  .. cpp:var:: int divider_counter

    Internal edges counter

  .. cpp:function:: void prc_clock_divider(void)

    Clock divider main Process

    The process counts the positive and negative edges and toggles to
    sc_logic('0') and sc_logic('1') when the edges are twice the ratio and
    exact the ratio, respectively.

    .. cpp:var:: list sensitivity

      clk_in.pos(), clk_in.neg()

Structure
*********

The structure of the clock divider is trivial since it has only one process.

Simulation Results
******************

The simulation of the clock divider is just create 2 clock divider one with
ratio 3 and other with ratio 4. :numref:`clock_divider_sim_wave` shows the
result of the simulation.


.. _clock_divider_sim_wave:
.. figure:: ../_static/clock_divider_simulation.png
  :align: center

  Clock Divider Simulation Wave Result

.. note::

  * `div_clock_3` has :math:`\frac{1}{3}` of the frequency of `sys_clock`
  * `div_clock_4` has :math:`\frac{1}{4}` of the frequency of `sys_clock`
  * `div_clock_3` and `div_clock_4` conserve :math:`50\%` duty cycle
