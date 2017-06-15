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

#ifndef VITERBI_PATH_H_
#define VITERBI_PATH_H_

#include <systemc.h>
#include <common/util.h>
#include <sstream>
#include <bitset>

#define CURR_STAGE 0
#define NEXT_STAGE 1
#define MAX_STAGES 2

#define MAX_PATH_LENGTH 100
#define MAX_PATH_WIDTH 32
#define MAX_METRIC_WIDTH 32

template<int output, int input, int memory>
  struct viterbi_path_s {
    /** Metric Value */
    sc_uint<MAX_METRIC_WIDTH> metric_value;
    /** Size of the path */
    sc_uint<MAX_PATH_WIDTH> path_size;
    /** Path */
    sc_uint<input << memory> states_path[MAX_PATH_LENGTH];
    /** Valid flag */
    bool is_alive;

    /**
     * Overload the = operation
     */
    inline viterbi_path_s& operator= (const viterbi_path_s& obj) {
      metric_value = obj.metric_value;
      path_size = obj.path_size;
      for (int i = 0; i < MAX_PATH_LENGTH; i++) {
        states_path[i] = obj.states_path[i];
      }
      is_alive = obj.is_alive;
    }

    /**
     * Overload the == operation
     */
    inline bool operator== (const viterbi_path_s& obj) const {

      bool ret_val = true;

      ret_val &= (metric_value == obj.metric_value);
      ret_val &= (path_size == obj.path_size);
      ret_val &= (is_alive == obj.is_alive);

      for (int i = 0; i < MAX_PATH_LENGTH; i++) {
        ret_val &= (states_path[i] == obj.states_path[i]);
      }

      return ret_val;
    }

  };
#endif
