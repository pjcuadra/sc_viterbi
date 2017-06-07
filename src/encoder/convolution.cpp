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
  if (reset == '1')
  {
    //y ="00";
    op[0]='0'; //set Y1 output 0
    op[1]='0'; //set Y2 output 0

  }
  else
  {
    op[0]= (ip[0] ^ ip[1]) ^ ip[2]; // Generator Polynomial (1,1,1)
    op[1]= ip[0] ^ ip[2]; // Generator Polynomial (1,0,1)
  }
  y.write(op);
}
