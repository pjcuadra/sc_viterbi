Simulation Results
******************

The code of the test case of the viterbi_encoder_lkup is shown below;

.. code-block:: cpp
  :linenos:

  ...

  static const int n = 2;
  static const int k = 1;
  static const int m = 4;

  ...

  static const int output_size = n * (2* m - k);

  SC_TEST(encoder) {
    ...

    // Create signals
    sc_signal<sc_lv<k> > in; //logic vector for shift register
    sc_signal<sc_logic> out_0; //logic output of output of convolution
    sc_signal<sc_logic> out_1; //logic output of output of convolution

    sc_signal<sc_lv<m> > mem_bus[k]; //logic vector for shift register
    sc_signal<sc_lv<m * k> > mem_bus_conv; //logic vector for shift register
    sc_signal<sc_logic> serial_in_drv[n];
    sc_signal<sc_lv<m> > polynomials[n];
    sc_lv<output_size> expected_out = "11110111010111";
    sc_lv<m> input_out = "1011";

    // Create module
    encoder<n, k, m> vencoder("ViterbiEncoder");
    encoder_lkup<n, k, m> vencoder_lkup("ViterbiEncoderLKUP");

    // Assign polynomials
    polynomials[0] = "1111";
    polynomials[1] = "1101";

    ...

    vencoder.clk(sys_clock);
    vencoder.in(in);
    vencoder.out(out_0);

    vencoder_lkup.clk(sys_clock);
    vencoder_lkup.in(in);
    vencoder_lkup.out(out_1);

    // Output verification (11110111010111)
    current_check_time = 220;
    SC_EXPECT_AT(sc_logic('0'), out_0, current_check_time, SC_NS);
    SC_EXPECT_AT(sc_logic('0'), out_1, current_check_time, SC_NS);
    current_check_time += clock_period / 2;

    for (int i = 0; i < output_size; i++) {
      SC_EXPECT_AT(sc_logic(expected_out.get_bit(output_size - i -1)), out_0, current_check_time, SC_NS);
      SC_EXPECT_AT(sc_logic(expected_out.get_bit(output_size - i -1)), out_1, current_check_time, SC_NS);
      current_check_time += clock_period;
    }

    ...

    // Set the serial input to encode
    for (int i = 0; i < m; i++) {
      in = sc_lv<k>(sc_logic(input_out.get_bit(m - i -1)));
      sc_start(2*clock_period, SC_NS);
    }

    in = "0";
    sc_start(200, SC_NS);

  }

.. note::
  * Both implementation of Viterbi encoder are being tested the same way.
  * Both encoders have the same input.
  * The input is :math:`b1011` and the expected encoded value
    :math:`b11110111010111`
  * The output is being verified with the :c:macro:`SC_EXPECT_AT`

:numref:`encoder_sim_wave` shows the result of the simulation.

.. _encoder_sim_wave:
.. figure:: ../_static/encoder_simulation.png
  :align: center

  Encoder Simulation Wave Result

.. note::

  * At :math:`200ns` the input starts to be `in` encoded. Both encoders have
    the same input.
  * Just :math:`output` cycles after the encoding starts.
  * The encoded value's MSb is transmitted first.
  * Every `in` state has to be stable for :math:`output` cycles.
  * `out_0` and `out_1` have the same baudrate as the `sys_clock`
  * `out_0` and `out_1` present the same behavior as expected
  * `out_0` and `out_1` are set back to `sc_logic('0')` after encoding is done
    (:math:`430ns`).
