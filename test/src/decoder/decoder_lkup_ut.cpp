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

#include <sc_test_framework.h>
#include <decoder/viterbi_path.h>
#include <decoder/decoder_lkup.h>
#include <sstream>

static const int output = 2;
static const int input = 1;
static const int memory = 4;
static const int output_buffer_bit_size = 16;
static const int clock_period = 15;
static const double clock_duty = 0.5;
static const int clock_start = 20;
static const double stimulus_start = 200;
// static const int output_size = n * (2* m - k);


SC_TEST(decoder_lkup) {
  // uint current_check_time = stimulus_start;
  // Create system clokc
  // sc_clock sys_clock("sys_clock", clock_period, clock_duty, clock_start, false);

  // Create signals

  sc_signal<sc_uint<memory + input> >lkup_add;
  sc_signal<bool> write_trig;
  sc_signal<bool> read_trig;
  sc_signal<bool> stage_trig;
  sc_signal<bool> shift_trig;
  sc_signal<bool> reset_trig;
  // sc_signal<viterbi_path_s<output_buffer_bit_size> >vp_in_st;
  // sc_signal<viterbi_path_s<output_buffer_bit_size> > vp_out_st;

  // // Create module
  decoder_lkup<output, input, memory, 16> decoder_lkup("ViterbiDecoder_LookUp");

  // connect channels to ports of "ViterbiDecoder_LookUp"
  decoder_lkup.write_lkup(write_trig);
  decoder_lkup.read_lkup(read_trig);
  decoder_lkup.stage_selector(stage_trig);
  decoder_lkup.shift_trigger(shift_trig);
  decoder_lkup.reset(reset_trig);
  decoder_lkup.lkup_address(lkup_add);
  // decoder_lkup.vp_in_state(vp_in_st);
  // decoder_lkup.vp_out_state(vp_out_st);

  // Trace all signals
  SC_TRACE(write_trig, "write_lkup");
  SC_TRACE(read_trig, "read_lkup");
  SC_TRACE(stage_trig, "stage_selector");
  SC_TRACE(shift_trig, "shift_trigger");
  SC_TRACE(reset_trig, "reset");
  SC_TRACE(lkup_add, "lkup_address");
  // SC_TRACE(vp_in_state, "vp_in_state");
  // SC_TRACE(vp_out_state, "vp_out_state");

  // start simulation
  write_trig = false;
  read_trig = true;
  stage_trig = false; //CURR_STAGE
  lkup_add = 4;
  sc_start(100, SC_NS);

  write_trig = false;
  read_trig = true;
  shift_trig = true; // shift data from CURR_STAGE to NEXT_STAGE
  stage_trig = true; //NEXT_STAGE
  lkup_add = 4;
  sc_start(100, SC_NS);

  write_trig = false;
  read_trig = true;
  stage_trig = true; //NEXT_STAGE
  lkup_add = 4;
  reset_trig =true; //Flush lookup
  sc_start(100, SC_NS);


}
