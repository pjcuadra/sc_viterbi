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

/**
 * Convolution Template
 * @param width size of the shift register
 * @param generator polynomial
 */
template<int width>
  SC_MODULE (convolution) {
    /** Logic vector of memory element */
    sc_in<sc_lv<width> > input;
    /** Output(logic) of convolution block */
    sc_out<sc_logic> y;
    /** Logic vector of memory element */
    sc_in<sc_lv<width> > polynomial;


    /**
     * Convolution Process (SC_METHOD)
     */
    void prc_calculate_conv(){
      /** Temporary variables necessary for port operations */
      sc_lv<width> output;
      sc_lv<width> ip;
      sc_lv<width> p;

      ip = input.read();
      p = polynomial.read();
      for(int i = 0; i < width; i++){
        output[i]=ip[i] & p[i];
      }
      y=output.xor_reduce();
    }


    /**
     * Constructor
     */
    SC_CTOR (convolution) {
      SC_METHOD (prc_calculate_conv);
      dont_initialize();
      sensitive << input;
    }

  };

#endif
