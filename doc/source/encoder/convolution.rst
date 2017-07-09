Convolution
-----------

Class Diagram
*************

The UML diagram of :numref:`convolution_class` shows the overview of the class.

.. _convolution_class:
.. uml::
  :align: center
  :caption: Convolution Block Class Diagram

  @startuml
    class sc_core::sc_module

    class convolution<int width> {
      .. Inputs ..
      sc_in<sc_lv<width> > input
      sc_in<sc_lv<width> > polynomial

      .. Outputs ..
      sc_out<sc_logic> y

      __ Processes __
      prc_calculate_conv()

    }
    convolution -up-|> sc_core::sc_module
  @enduml


Class Description
*****************

.. cpp:class:: template<int width> convolution

  Convolution block

  .. cpp:var:: sc_in<sc_lv<width> > input

    Parallel input to be convolved

  .. cpp:var:: sc_in<sc_lv<width> > polynomial

    Polynomials to convolve with

  .. cpp:var:: sc_out<sc_logic> y

    Convolved output

  .. cpp:function:: void prc_calculate_conv(void)

    Convolution Process. Logic and between input and polynomial and xor reduce
    the result.

    .. cpp:var:: list sensitivity

      input


Structure
*********

TODO

Simulation Results
******************

TODO
