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

#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

#include<systemc.h>

SC_MODULE (convolution) {
  sc_in<bool> clk,reset;
  sc_in<sc_bit> mem_1, mem_2, mem_3;
  sc_out<sc_bit> y1, y2;

  void calculate_conv_method();

  SC_CTOR (convolution) {
    SC_METHOD (calculate_conv_method);
    dont_initialize();
    sensitive << clk.pos()<<reset;
  }

};

#endif
