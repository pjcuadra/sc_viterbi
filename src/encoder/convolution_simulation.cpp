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

#include<encoder/convolution.h>
#include<systemc.h>

int sc_main(int argc, char **argv)
{
  // create channels
  sc_signal<sc_bit> shift_Reg_0;
  sc_signal<sc_bit> shift_Reg_1;
  sc_signal<sc_bit> shift_Reg_2;

  sc_signal<sc_bit> conv_output_1;
  sc_signal<sc_bit> conv_output_2;

  // create modules
  convolution convolution("convolution");

  // connect channels to ports
  convolution.mem_1(shift_Reg_0);
  convolution.mem_2(shift_Reg_1);
  convolution.mem_3(shift_Reg_2);

  convolution.y1(conv_output_1);
  convolution.y2(conv_output_2);

  // start simulation
  sc_start(1000, SC_NS);

  return 0;
}
