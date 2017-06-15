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

#ifndef METRIC_CALCULATOR_H_
#define METRIC_CALCULATOR_H_

#include<systemc.h>

/**
 * Metric Calculator Template
 * @param width size of the output
 */
template<int width>
  SC_MODULE (metric_calculator) {
    /** Logic vector of received bits from sampler */
    sc_in<sc_lv<width> > received;
    /** Logic vector of received bits  */
    sc_in<sc_lv<width> > expected_codeword;
    /** Value of predecessor path metric  */
    sc_in<int> predecessor_path_metric;
    /** Output(int) of metric_calculator block */
    sc_out<int> branch_metric;
    /** Output(int) of metric_calculator block */
    sc_out<int> path_metric;
    /** Output(vector) of metric_calculator block--testing purpose */
    sc_out<sc_lv<width> > xor_op;


    /**
     * Calculation Processes for Hamming Metric (SC_METHOD)
     */
    void prc_calculate_hamming_metric_method(){
      /** Temporary variables necessary for port operations */
      sc_lv<width> output;
      sc_lv<width> rx;
      sc_lv<width> cw;
      int count;

      // Read inputs
      rx = received.read();
      cw = expected_codeword.read();

      // Calculation of EX-NOR for hamming matrix
      output = ~( rx ^ cw );
      // Test signal (as integer values of branch and path metrices can't be plotted )
      xor_op = output;

      for(int i = 0; i < width; i++){
        if (output[i]=='1'){
          count++;
        }
        else{
          //do nothing
        }
      }
      // Get branch metric
      branch_metric = count;
    }

    /**
     * Calculation Processes for Path Metric (SC_METHOD)
     */
    void prc_calculate_path_metric_method(){
      /** Temporary variables necessary for port operations */
      int br_met;
      int pp_met;

      // Read inputs
      br_met = branch_metric.read();
      pp_met = predecessor_path_metric.read();

      // Get path metric
      path_metric = br_met + pp_met;
    }

    /**
     * Constructor
     */
    SC_CTOR (metric_calculator) {
      SC_METHOD (prc_calculate_hamming_metric_method);
      dont_initialize();
      sensitive << received;

      SC_METHOD (prc_calculate_path_metric_method);
      dont_initialize();
      sensitive << branch_metric << predecessor_path_metric;
    }

  };

#endif
