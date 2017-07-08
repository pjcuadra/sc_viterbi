# Copyright 2017 Pedro Cuadra <pjcuadra@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

function(add_single_test_source FILE)

  # Get executable name
  get_filename_component(EXEC_NAME ${FILE} NAME_WE)
  get_filename_component(TRACE_DIR ${FILE} DIRECTORY)

  message("-- Test source found: ${FILE}")
  message("----       Test name: ${EXEC_NAME}")
  message("----      Trace path: trace/${TRACE_DIR}/")

  # Create executable
  add_executable(${EXEC_NAME} EXCLUDE_FROM_ALL ${TEST_SRC_PATH}/${FILE})
  target_link_libraries(${EXEC_NAME} libsc_viterbi_run ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

  # Add the test
  add_test(${EXEC_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME})

  # Add it as dependency of the compile_tests target
  add_dependencies(compile_tests ${EXEC_NAME})

  # Pass the directory to dump the trace
  target_compile_definitions(${EXEC_NAME} PRIVATE -DTRACE_PATH="${CMAKE_CURRENT_BINARY_DIR}/trace/${TRACE_DIR}/")

  # Create directory path
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/trace/${TRACE_DIR}")

endfunction()

function(add_all_tests TEST_SRC)

  set(TEST_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SRC}")
  message("-- Test Source Path: ${TEST_SRC_PATH}")

  add_subdirectory("${TEST_SRC}")

  # Add custom target
  add_custom_target(compile_tests)

  # Iterate over all sources
  foreach (_test ${sc_viterbi_test_SOURCES})

    # Add a single test
    add_single_test_source(${_test})

  endforeach()

endfunction()

# Add testing source macro
macro (add_test_sources)
    file (RELATIVE_PATH _relPath "${TEST_SRC_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND sc_viterbi_test_SOURCES "${_relPath}/${_src}")
        else()
            list (APPEND sc_viterbi_test_SOURCES "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (sc_viterbi_test_SOURCES ${sc_viterbi_test_SOURCES} PARENT_SCOPE)
    endif()
endmacro()

# Add testing source directory macro
macro (add_test_sources_directory)
  foreach (_src ${ARGN})
      add_subdirectory(${_src})
      # propagate SRCS to parent directory
      set (sc_viterbi_test_SOURCES ${sc_viterbi_test_SOURCES} PARENT_SCOPE)
  endforeach()
endmacro()
