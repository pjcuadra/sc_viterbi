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

# Add sources macro
macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND sc_viterbi_SOURCES "${_relPath}/${_src}")
        else()
            list (APPEND sc_viterbi_SOURCES "${_src}")
        endif()
    endforeach()

    if (_relPath)
        # propagate SRCS to parent directory
        set (sc_viterbi_SOURCES ${sc_viterbi_SOURCES} PARENT_SCOPE)
    endif()
endmacro()

# Add source directory macro
macro (add_sources_directory)
  file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
  foreach (_src ${ARGN})
      add_subdirectory(${_src})
      # propagate SRCS to parent directory
      set (sc_viterbi_SOURCES ${sc_viterbi_SOURCES} PARENT_SCOPE)
  endforeach()
endmacro()

function(add_single_test_source FILE)
  get_filename_component(EXEC_NAME ${FILE} NAME_WE)
  add_executable(${EXEC_NAME} EXCLUDE_FROM_ALL ${FILE})
  target_link_libraries(${EXEC_NAME} libsc_viterbi_run ${SystemC_LIBRARIES})
  add_test(${EXEC_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME})
  add_dependencies(compile_tests ${EXEC_NAME})
endfunction()

function(add_all_tests)
  add_custom_target(compile_tests)
  foreach (_test ${sc_viterbi_test_SOURCES})
    add_single_test_source(${_test})
  endforeach()
endfunction()

# Add testing source macro
macro (add_test_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
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
  file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
  foreach (_src ${ARGN})
      add_subdirectory(${_src})
      # propagate SRCS to parent directory
      set (sc_viterbi_test_SOURCES ${sc_viterbi_test_SOURCES} PARENT_SCOPE)
  endforeach()
endmacro()
