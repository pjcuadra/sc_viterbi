Viterbi Path Structure
----------------------

Class Diagram
*************

The UML diagram of :numref:`viterbi_path_class` shows the
overview of the class.

.. _viterbi_path_class:
.. uml::
  :align: center
  :caption: Viterbi Path Structure

  @startuml
    class sc_core::sc_signal

    class viterbi_path_s<int output_buffer_bit_size> {
      sc_uint<MAX_METRIC_WIDTH> metric_value
      sc_uint<MAX_PATH_WIDTH> path_size
      sc_uint<output_buffer_bit_size> path_output
      bool is_alive

      viterbi_path_s<output_buffer_bit_size>& operator= (const viterbi_path_s<output_buffer_bit_size>& obj)
      bool operator== (const viterbi_path_s<output_buffer_bit_size>& obj) const
      void sc_trace(sc_trace_file * tf, const viterbi_path_s<output_buffer_bit_size>& obj, const std::string& name)

    }

    viterbi_path_s -up-|> sc_core::sc_signal

  @enduml


Class Description
*****************

.. cpp:class:: template<int output_buffer_bit_size> viterbi_path_s

  Viterbi Path Structure

  .. cpp:var:: sc_uint<MAX_METRIC_WIDTH> metric_value

    Metric Value

  .. cpp:var:: sc_uint<MAX_PATH_WIDTH> path_size

    Current size of the path

  .. cpp:var:: sc_uint<output_buffer_bit_size> path_output

    Ordered array of states comprising the Viterbi path.

  .. cpp:var:: bool is_alive

    Flags the validity of the information stored in the structure.

  .. cpp:function:: inline viterbi_path_s<output_buffer_bit_size>& operator= \
                    (const viterbi_path_s<output_buffer_bit_size>& obj)

    Overload the = operator

    :param const viterbi_path_s<output_buffer_bit_size>& obj: Object to be
                                                              assign to this
                                                              instance.

  .. cpp:function:: inline bool operator== \
                    (const viterbi_path_s<output_buffer_bit_size>& obj) const

    Overload the == operation

    :param const viterbi_path_s<output_buffer_bit_size>& obj: Object to be
                                                              equal to this
                                                              instance.

  .. cpp:function:: inline friend void sc_trace(sc_trace_file * tf, \
                    const viterbi_path_s<output_buffer_bit_size>& obj, \
                    const std::string& name)

    Enables the tracing of the signal of type
    viterbi_path_s<output_buffer_bit_size>

    :param sc_trace_file * tf: Trace file pointer
    :param const viterbi_path_s<output_buffer_bit_size>& obj: Object instance
                                                              to trace.
    :param const string& name: Signal name



Structure
*********

The structure of the clock divider is trivial since it has only one process.
