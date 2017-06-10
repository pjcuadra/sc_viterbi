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

#ifndef COMPARATOR_H_
#define COMPARATOR_H_

#include<systemc.h>

/**
 * Comparator Template
 * @param width size of the output
 */
template<class T1, class T2>
  SC_MODULE (comparator) {
    /** First entity to compare from decoder */
    sc_in<T1> first_entity;
    /** Second entity to compare from decoder  */
    sc_in<T2> second_entity;
    /**Output: Maxximum entity  */
    sc_out<T1> maximum;


    /**
     * Calculation Processes for Hamming Metric (SC_METHOD)
     */
    void prc_comparision_method(){

    /** Temporary variables necessary for port operations */
    T1 output;
    output = (first_entity < second_entity) ? second_entity : first_entity;
    maximum = output;
    }

    /**
     * Constructor
     */
    SC_CTOR (comparator) {
      SC_METHOD (prc_comparision_method);
      dont_initialize();
      sensitive << first_entity << second_entity;
    }

  };

#endif
