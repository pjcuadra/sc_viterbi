/* Copyright 2017 Pedro Cuadra <pjcuadra@gmail.com> & Meghadoot Gardi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#ifndef ENCODER_H_
#define ENCODER_H_

#include<systemc.h>
#include<common/shift_register.h>
#include<common/clock_divider.h>
#include<encoder/convolution.h>
#include<common/serializer.h>

template<int output, int input, int memory>
  SC_MODULE (encoder) {
    // Inputs
    /** Input CLK */
    sc_in_clk clk;
    /** Parallel input */
    sc_in<sc_lv<input> > in;
    /** Polynomials input */
    sc_in<sc_lv<memory * input> > polynomials[output];
    // Outputs
    /** Serial output */
    sc_out<sc_logic > out;
    /** Arranged for convolution memory bus */
    sc_signal<sc_lv<memory * input> > mem_bus_conv;
    // Internal Signals
    /** Divided clk signal */
    sc_signal<bool> clk_div;
    /** Data in driver (distribute to shift registers) */
    sc_signal<sc_logic> data_in_drv[memory];
    /** Convolution outputs array */
    sc_signal<sc_logic> conv_outs[output];
    /** Parallel convolution outputs bus */
    sc_signal<sc_lv<output> > conv_par_bus;
    /** Shift registers' memory buses */
    sc_signal<sc_lv<memory> > mem_bus[input];
    // Sub modules
    /** Convolution Block*/
    convolution<memory * input> * conv_block[output];
    /** Shift Registers */
    shift_register<memory> * register_bank[input];
    /** Clock divider */
    clock_divider<output> * clk_divider;
    /** Output serializer */
    serializer<output> * serial;

    /**
     * Split parallel input into all shift registers
     */
    void prc_split_input() {
      sc_lv<input> in_tmp = in.read();

      for (int i = 0; i < input; i++) {
        data_in_drv[i] = in_tmp[i];
      }

    }

    /**
     * Update the convolutions output parallel bus out of all convolution
     * blocks' outputs
     */
    void prc_update_conv_par_bus() {
      sc_lv<output> ser_in_tmp;

      for (int o = 0; o < output; o++) {
        ser_in_tmp[o] = conv_outs[o];
      }

      conv_par_bus = ser_in_tmp;

    }

    /**
     * Arrange memory bus out of all parallel shift registers
     */
    void prc_arrange_memory_bus() {
      int p_index = 0;
      sc_lv<memory * input> mem_bus_conv_tmp;

      for (int j = 0; j < memory; j++) {
        for (int i = 0; i < input; i++) {
          sc_lv<memory> mem_bus_tmp = mem_bus[i];

          mem_bus_conv_tmp[p_index] = mem_bus_tmp[j];
          p_index++;
        }
      }

      mem_bus_conv = mem_bus_conv_tmp;

    }

    /**
     * Constructor
     */
    SC_CTOR (encoder) {

      SC_METHOD(prc_arrange_memory_bus)
      // Create shift registers and connect their inputs/outputs
      for (int i = 0; i < input; i++) {
        sensitive << mem_bus[i];
        register_bank[i] =  new shift_register<memory> ("reg_" + i);
        register_bank[i]->data_in(data_in_drv[i]);
        register_bank[i]->clk(clk_div);
        register_bank[i]->q(mem_bus[i]);

        // Initialize to zero
        data_in_drv[i] = sc_logic('0');
      }

      // Create clock divider and connect its inputs/outputs
      clk_divider = new clock_divider<output>("clk_div");
      clk_divider->clk_in(clk);
      clk_divider->clk_out(clk_div);

      SC_METHOD(prc_update_conv_par_bus)

      // Create convolution blocks and connect their inputs/outputs
      for (int o = 0; o < output; o++) {
        sensitive << conv_outs[o];
        conv_block[o] = new convolution<memory * input>("conv_" + o);
        conv_block[o]->input(mem_bus_conv);
        conv_block[o]->y(conv_outs[o]);
        conv_block[o]->polynomial(polynomials[output - o - 1]);
      }

      // Initialize to zeros
      conv_par_bus = sc_lv<output> (sc_logic_0);

      // Create output serializer and connect its inputs/outputs
      serial = new serializer<output>("serializer");
      serial->clk_in(clk);
      serial->par_in(conv_par_bus);
      serial->ser_out(out);
      serial->ser_trig(clk_div);

      SC_METHOD(prc_split_input)
      sensitive << in;

    }
  };

#endif
