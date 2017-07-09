Lookup Tables Utilities
-----------------------

Functions Description
*********************

.. cpp:function:: template <int output, int input, int memory> \
                  inline void create_states_lkup(sc_lv<memory * input> * \
                  next_state_lkp)

  Initialize the next state lookup table.

  :param sc_lv<memory * input> * next_state_lkp: Pointer to the lookup table to
                                                 to be initialized.

.. cpp:function:: template <int output, int input, int memory> \
                  inline void create_output_lkup(sc_lv<memory * input> \
                  polynomials[output], sc_lv<output> * output_lkp)

 Initialize the output/encoded lookup table.

 :param sc_lv<memory * input> polynomials[output]: Polynomials used to encode.
 :param sc_lv<output> * output_lkp: Pointer to the output/encoded lookup table
                                    to be initialized.
