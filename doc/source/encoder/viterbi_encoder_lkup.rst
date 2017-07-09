Viterbi Encoder (with Lookup Tables)
------------------------------------

Class Diagram
*************

The UML diagram of :numref:`viterbi_encoder_lkup_table_class` shows the
overview of the class.

.. _viterbi_encoder_lkup_table_class:
.. uml::
  :align: center
  :caption: Viterbi Encoder with Lookup Tables Class Diagram

  @startuml
    class sc_core::sc_module

    class encoder_lkup<int output, int input, int memory> {

      .. Constants ..
      uint lookup_size = input << memory

      .. Inputs ..
      sc_in_clk clk
      sc_in<sc_lv<input> > in
      sc_in<sc_lv<memory * input> > polynomials[output]

      .. Outputs ..
      sc_out<sc_logic > out

      .. Variables ..
      sc_lv<memory * input> curr_state
      sc_lv<memory * input> next_state_lkp[lookup_size]
      sc_lv<output> output_lkp[lookup_size]
      uint div_counter

      __ Processes __
      prc_state_trasition()
      prc_update_output_lkup()

    }
    encoder_lkup -up-|> sc_core::sc_module
  @enduml

Class Description
*****************

.. cpp:class:: template<int output, int input, int memory> encoder_lkup

  Viterbi Encoder with lookup tables

  .. cpp:var:: sc_core::sc_in_clk clk

    Input clock

  .. cpp:var:: sc_in<sc_lv<input> > in

    Parallel input to be encoded

  .. cpp:var:: sc_in<sc_lv<memory * input> > polynomials[output]

    Polynomials to convolve with

  .. cpp:var:: sc_out<sc_logic > out

    Serialized encoded output

  .. cpp:var:: sc_lv<memory * input> curr_state

    Current State value holder

  .. cpp:var:: sc_lv<memory * input> next_state_lkp[lookup_size]

    Next state lookup table. This is filled in by create_states_lkup.

  .. cpp:var:: sc_lv<output> output_lkp[lookup_size]

    Output lookup table

  .. cpp:var:: uint div_counter

    Clock divider counter

  .. cpp:function:: void prc_state_trasition(void)

    Transit from one state to the other depending on input and current state.
    Uses the lookup table to determine the next step.

    .. cpp:var:: list sensitivity

      clk.pos()

  .. cpp:function:: void prc_update_output_lkup(void)

    Updates the output lookup table if any change occurs in the polynomials.

    .. cpp:var:: list sensitivity

      polynomials

.. note::
    `next_state_lkp[lookup_size]` and `output_lkp[lookup_size]` are filled using `create_states_lkup` and
    `create_output_lkup` respectively.


Structure
*********

:numref:`encoder_lkup_circuit` shows the structure of the our Viterbi encoder
implementation using lookup tables.

.. _encoder_lkup_circuit:
.. figure:: ../_static/encoder_lkup_circuit.png
  :align: center

  Viterbi Encoder with Lookup Table Circuit

.. include:: encoder_simulation_results.rst
