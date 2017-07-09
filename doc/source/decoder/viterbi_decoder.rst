Viterbi Decoder
---------------

Class Diagram
*************

The UML diagram of :numref:`viterbit_decoder_class` shows the overview of the
class.

.. _viterbit_decoder_class:
.. uml::
  :align: center
  :scale: 70 %
  :caption: Viterbi Decoder Class Diagram

  @startuml
    class sc_core::sc_module

    class decoder_viterbi<int output, int input, int memory> {
      .. Constants ..
      const uint input_width = output * (2 * memory - input)
      const uint states_num = input << (memory - 1)
      const uint lookup_size = input << memory
      const uint output_width = input_width / output
      const uint MAX_STAGES = 2

      .. Inputs ..
      sc_in_clk clk
      sc_in<sc_logic> in
      sc_in_clk trigger
      sc_in<sc_lv<memory * input> > polynomials[output]

      .. Outputs ..
      sc_out<sc_logic > out

      .. Variables ..
      viterbi_path_s<output_buffer_bit_size> trellis_tree_lkup[MAX_STAGES][states_num]
      sc_lv<memory * input> next_state_lkp[lookup_size]
      sc_lv<output> output_lkp[lookup_size]
      uint curr_trellis_stage
      uint serializer_count
      bool decoding
      bool serializing

      .. Signals ..
      sc_signal<bool> clk_div
      sc_signal<sc_lv<output> > par_in

      .. Events ..
      sc_event shift_stage

      .. Sub-modules ..
      clock_divider<output> * clk_divider
      shift_register<output> * shift_reg


      __ Processes __
      prc_decode_viterbi()
      prc_shift_stage()
      prc_update_output_lkup()
      prc_serialize_output()
      prc_decode_catch_trigger()
      prc_decode_start_serializing()

      -- Helpers --
      uint get_metrics(uint val_0, uint val_1)

    }
    decoder_viterbi -up-|> sc_core::sc_module
  @enduml


Class Description
*****************

.. cpp:class:: template<int output, int input, int memory> decoder_viterbi

  Viterbi Decoder

  .. cpp:var:: sc_core::sc_in_clk clk

    Input clock

  .. cpp:var:: sc_in<sc_lv<input> > in

    Serial input to be decoded

  .. cpp:var:: sc_in_clk trigger

    Decoding Trigger

  .. cpp:var:: sc_in<sc_lv<memory * input> > polynomials[output]

    Polynomials used for encoding

  .. cpp:var:: sc_out<sc_logic> out

    Decoded serial output

  .. cpp:var:: viterbi_path_s<output_buffer_bit_size> trellis_tree_lkup[MAX_STAGES][states_num]

    Trellis diagram lookup table. It only stores current stage and next stage.
    These 2 stages are the only ones needed at every calculation point in time.

  .. cpp:var:: sc_lv<memory * input> next_state_lkp[lookup_size]

    Next stage lookup table

  .. cpp:var:: sc_lv<output> output_lkp[lookup_size]

    Output lookup table

  .. cpp:var:: uint curr_trellis_stage

    Overall current stage of the trellis diagram. It doesn't only consider
    current and next stage. For every stage of the trellis diagram this value
    get incremented by 1.

  .. cpp:var:: uint serializer_count

    Bit selector counter for output serialization

  .. cpp:var:: bool decoding

    Flags that the Viterbi decoder is in decoding state

  .. cpp:var:: bool serializing

    Flags that the Viterbi decoder is in serializing state

  .. cpp:var:: sc_event shift_stage

    This event is trigger every time a stage of the Trellis diagram is
    completely calculated. Needed to switch to the next state, meaning assigning
    next state values to current state.

  .. cpp:var:: sc_signal<bool> clk_div

    Divided clock signal

  .. cpp:var:: sc_signal<sc_lv<output> > par_in

    Parallelized input

  .. cpp:var:: clock_divider<output> * clk_divider

    Clock divider

  .. cpp:var:: shift_register<output> * shift_reg

    Shift register to parallelize the input

  .. cpp:function:: void prc_decode_viterbi(void)

    Decode a parallel input using Viterbi algorithm. This process calculate the
    Viterbi path for one stage of the Trellis diagram. The entire decoding is
    done when all the needed stages are calculated. The stage calculation is
    done every time the needed input bits are available, this increases the
    throughput of the Viterbi decoder, because it doesn't have to have the
    entire input ready to start decoding.

    .. cpp:var:: list sensitivity

      clk_div.pos()

  .. cpp:function:: void prc_shift_stage(void)

    Moves the NEXT_STAGE values of the trellis_tree_lkup to the CURR_STAGE slot.

    .. cpp:var:: list sensitivity

      Dynamic sensitivity with shift_stage event

  .. cpp:function:: void prc_update_output_lkup(void)

    Build the output lookup table based on polynomials

    .. cpp:var:: list sensitivity

      polynomials

  .. cpp:function:: void prc_serialize_output(void)

    Serialize the output

    .. cpp:var:: list sensitivity

      clk.pos()

  .. cpp:function:: void prc_decode_catch_trigger(void)

    Catch the trigger for decoding. Initialize all needed structures for running
    the Viterbi decoding algorithm. Flags the decoding state after
    initialization.

    .. cpp:var:: list sensitivity

      trigger.pos()

  .. cpp:function:: void prc_decode_start_serializing(void)

    Catches the trigger for starting serialization. Flags serializing state
    and unflags the decoding state.

    .. cpp:var:: list sensitivity

      trigger.neg()

  .. cpp:function:: uint get_metrics(uint val_0, uint val_1)

    Calculate the metrics between two values.

    :param unit val_0: First value
    :param unit val_1: Second value
    :return: Metric value



Structure
*********

TODO

Simulation Results
******************

TODO
