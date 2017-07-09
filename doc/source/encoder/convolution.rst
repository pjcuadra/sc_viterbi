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

The structure of this module comprises an array of :math:`input * memory`
and-gates to apply a bit-wise and between the polynomials and the input port.
After applying the bit-wise and a xor reduce of all bits have to be applied.
This implied :math:`(input * memory) - 1` xor-gates applying a xor operation
to every bit of the result of the bit-wise and the result of the xor gate of the
previous 2 bits (for the first 2 bits the xor-gate is simply apply to both).

Simulation Results
******************

The code of the test case of the convolution is shown below;

.. code-block:: cpp
  :linenos:

  ...

  static const int reg_width = 3;

  ...

  SC_TEST(convolution) {

    // create channels
    sc_signal<sc_lv<reg_width> > shift_reg; //logic vector for shift register
    sc_signal<sc_lv<reg_width> > generator_polynomial; //logic vector for shift register
    sc_signal<sc_logic> conv_output; //logic output of output of convolution

    // create module
    convolution<reg_width> convolution("Convolution");

    ...

    generator_polynomial = "101";

    // start simulation
    shift_reg = "100";
    sc_start(100, SC_NS);

    shift_reg = "110";
    sc_start(100, SC_NS);

  }

.. note::
  * `generator_polynomial` is constant with value :math:`b101`
  * `shift_reg` starts with value :math:`b100` and changes to :math:`b110` at
    :math:`100ns`.

:numref:`convolution_sim_wave` shows the result of the simulation.

.. _convolution_sim_wave:
.. figure:: ../_static/convolution_simulation.png
  :align: center

  Convolution Simulation Wave Result

.. note::

  * Since the convolution is purely combinational no clock is needed.
