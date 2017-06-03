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

#ifndef DECODER_H_
#define DECODER_H_

#include<systemc.h>

SC_MODULE (decoder) {
  sc_in<bool> a, b, carry_in;
  sc_out<bool> sum, carry_out;

  void prc_decoder();

  SC_CTOR (decoder) {
    SC_METHOD (prc_decoder);
    sensitive << a << b << carry_in;
  }
};

#endif
