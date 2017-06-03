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

void convolution::calculate_conv_method()
{
  if (reset)
  {
    y1 = sc_bit('0'); //set Y1 output 0
    y2 = sc_bit('0'); //set Y2 output 0
  }
  else
  {
    y1= (mem_1 ^ mem_2) ^ mem_3; // Generator Polynomial (1,1,1)
    y2= mem_1 ^ mem_3; // Generator Polynomial (1,0,1)
  }
}
