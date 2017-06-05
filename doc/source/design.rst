.. SystemC Viterbi documentation master file, created by
   sphinx-quickstart on Tue May 30 18:33:15 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

System's Design
===============

Viterbi Encoder
---------------

The encoder implementation is as specified in the following diagram.

.. graphviz::
  :caption: System

  digraph G {
    graph [rankdir=LR];
    node [shape=polygon];

    subgraph cluster_0 {

      subgraph cluster_1 {
        shift_reg_0 [label="Shift Register 0"];
        shift_reg_1 [label="Shift Register 1"];
        shift_reg_m [shape=plaintext, label="..."];
        shift_reg_k [label="Shift Register k"];
        parallel [label="Parallelizer"];
        shift_reg_0 -> parallel [label="m"];
        shift_reg_1 -> parallel [label="m"];
        shift_reg_k -> parallel [label="m"];
        label = "Shift Registers Bank";
        color=blue
      }

      subgraph cluster_2 {
        conv_0 [label="Convolution 0"];
        conv_1 [label="Convolution 1"];
        conv_o [shape=plaintext, label="..."];
        conv_n [label="Convolution n"];
        conv_0 -> serial;
        conv_1 -> serial;
        conv_n -> serial;
        serial [label="Serializer"];
        label = "Convolution Block";
        color=blue
      }

      parallel -> conv_0 [label="m*k"];
      parallel -> conv_1 [label="m*k"];
      parallel -> conv_n [label="m*k"];

      label = "Viterbi Encoder";

    }

    in_0 [shape=plaintext, label="in[0]"]
    in_0 -> shift_reg_0;
    in_1 [shape=plaintext, label="in[1]"]
    in_1 -> shift_reg_1;
    in_k [shape=plaintext, label="in[k]"]
    in_k -> shift_reg_k;

    out [shape=plaintext, label="out"]
    serial -> out;
  }

Please note that the encoder has also a clock input that is propagated to the
shift registers only.

.. warning::
  In this case convolution is purely combinational.

The Shift Register internal structure is shown in the following diagram;

.. graphviz::
  :caption: Shift register

  digraph G {
    graph [rankdir=LR];
    node [shape=polygon];

    subgraph cluster_0 {

      bit_store_t [shape=plaintext, label="..."];

      bit_store_0 -> bit_store_1 [label="q"];
      bit_store_0 -> bit_store_1 [label="clk"];
      bit_store_1 -> bit_store_t [label="q"];
      bit_store_1 -> bit_store_t [label="clk"];
      bit_store_t -> bit_store_m [label="q"];
      bit_store_t -> bit_store_m [label="clk"];

      label = "Shift Register";

    }

    data_in [shape=plaintext, label="Data In"]
    data_in -> bit_store_0;
    clk [shape=plaintext, label="Clock"]
    clk -> bit_store_0;

  }


Viterbi Decoder
---------------
