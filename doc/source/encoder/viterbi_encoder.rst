Viterbi Encoder
---------------

Class Diagram
*************

The UML diagram of :numref:`viterbi_encoder_class` shows the
overview of the class.

.. _viterbi_encoder_class:
.. uml::
  :align: center
  :caption: Viterbi Encoder Class Diagram

  @startuml
    class sc_core::sc_module

    class encoder<int output, int input, int memory> {
      .. Inputs ..
      sc_in_clk clk
      sc_in<sc_lv<input> > in
      sc_in<sc_lv<memory * input> > polynomials[output]

      .. Outputs ..
      sc_out<sc_logic > out

      .. Signals ..
      sc_signal<sc_lv<memory * input> > mem_bus_conv
      sc_signal<bool> clk_div
      sc_signal<sc_logic> data_in_drv[memory]
      sc_signal<sc_logic> conv_outs[output]
      sc_signal<sc_lv<output> > conv_par_bus
      sc_signal<sc_lv<memory> > mem_bus[input]


      .. Sub-modules ..
      convolution<memory * input> * conv_block[output]
      shift_register<memory> * register_bank[input]
      clock_divider<output> * clk_divider
      serializer<output> * serial

      __ Processes __
      prc_split_input()
      prc_update_conv_par_bus()
      prc_arrange_memory_bus()

    }
    encoder -up-|> sc_core::sc_module
  @enduml


Class Description
*****************

.. cpp:class:: template<int output, int input, int memory> encoder

  Viterbi Encoder

  .. cpp:var:: sc_core::sc_in_clk clk

    Input clock

  .. cpp:var:: sc_in<sc_lv<input> > in

    Parallel input to be encoded

  .. cpp:var:: sc_in<sc_lv<memory * input> > polynomials[output]

    Polynomials to convolve with

  .. cpp:var:: sc_out<sc_logic > out

    Serialized encoded output

  .. cpp:var:: sc_signal<sc_lv<memory * input> > mem_bus_conv

    Arranged for convolution memory bus

  .. cpp:var:: sc_signal<bool> clk_div

    Divided clk signal

  .. cpp:var:: sc_signal<sc_logic> data_in_drv[memory]

    Data in driver (distribute to shift registers)

  .. cpp:var:: sc_signal<sc_logic> conv_outs[output]

    Convolution outputs array

  .. cpp:var:: sc_signal<sc_lv<output> > conv_par_bus

    Parallel convolution outputs bus

  .. cpp:var:: sc_signal<sc_lv<memory> > mem_bus[input]

    Shift registers' memory buses

  .. cpp:var:: convolution<memory * input> * conv_block[output]

    Convolution Modules

  .. cpp:var:: shift_register<memory> * register_bank[input]

    Shift Registers

  .. cpp:var:: clock_divider<output> * clk_divider

    Clock divider

  .. cpp:var:: serializer<output> * serial

    Serializer for the output

  .. cpp:function:: void prc_split_input(void)

    Split the in every input to connect with each shift register of the bank.
    There are input shift registers.

    .. cpp:var:: list sensitivity

      in

  .. cpp:function:: void prc_update_conv_par_bus(void)

    Merge all convolution blocks outputs into a single parallel bus.

    .. cpp:var:: list sensitivity

      conv_outs

  .. cpp:function:: void prc_arrange_memory_bus(void)

    Create a single parallel bus merging all parallel buses of all shift
    registers.

    .. cpp:var:: list sensitivity

      mem_bus

Structure
*********

:numref:`encoder_circuit` shows the structure of the our Viterbi encoder
implementation without using lookup tables.

.. _encoder_circuit:
.. figure:: ../_static/encoder_circuit.png
  :align: center

  Viterbi Encoder Circuit

.. include:: encoder_simulation_results.rst
