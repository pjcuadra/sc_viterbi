Serializer
----------

Class Diagram
*************

The UML diagram of :numref:`serializer_class` shows the overview of the class.

.. _serializer_class:
.. uml::
  :align: center
  :caption: Serializer Class Diagram

  @startuml
    class sc_core::sc_module

    class serializer<int inputs> {
      .. Inputs ..
      sc_in_clk clk_in
      sc_in<sc_lv<inputs> > par_in
      sc_in_clk ser_trig

      .. Outputs ..
      sc_out<sc_logic> ser_out

      .. Variables ..
      int output_selector

      __ Processes __
      prc_serializer()
      prc_serializer_counter_update()
      prc_serializer_counter_reset()

    }

    serializer -up-|> sc_core::sc_module

  @enduml


Class Description
*****************

.. cpp:class:: template<int inputs> serializer

  Clock Divider module with clock division ratio

  .. cpp:var:: sc_core::sc_in_clk clk_in

    Input clock

  .. cpp:var:: sc_core::sc_in<sc_core::sc_lv<inputs> > par_in

    Parallel input

  .. cpp:var:: sc_core::sc_in_clk ser_trig

    Trigger the serialization

  .. cpp:var:: sc_core::sc_out<sc_core::sc_logic> ser_out

    Serialized output

  .. cpp:var:: int output_selector

    Internal counter for select the bit index to forward to the output

  .. cpp:function:: void prc_serializer(void)

    Serializer main Process

    Forwards the current bit to the ser_out. The current bit is calculated
    using output_selector. The serialization process starts with the ser_trig
    and ends when the last bit has been forwarded to the output. After
    forwarding the last bit the output is set to sc_logic('0').

    .. cpp:var:: list sensitivity

      clk_in.pos(), ser_trig.pos()

  .. cpp:function:: void prc_serializer_counter_update(void)

    Updates the internal bit selector counter with each positive edge of the
    clock.

    .. cpp:var:: list sensitivity

      clk_in.pos()

  .. cpp:function:: void prc_serializer_counter_reset(void)

    Reset the internal bit selector counter after a ser_trig positive edge.

    .. cpp:var:: list sensitivity

      ser_trig.pos()


Structure
*********

TODO

Simulation Results
******************

TODO
