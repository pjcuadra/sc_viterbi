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

# Define project
project(sc_viterbi)

# Define required cmake version
cmake_minimum_required(VERSION 2.8)

# Include macros
include(cmake/macros.cmake)

# Locate the packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Enable testing
enable_testing()

# Parse all sources
add_subdirectory(src/)

# Find SystemC Library
find_package(SystemC REQUIRED)

# Add SystemC headers
include_directories(include/ ${SystemC_INCLUDE_DIRS})

# Create library out of code
add_library(libsc_viterbi_run STATIC ${sc_viterbi_SOURCES})

# Add compile test target
set(CTEST_OUTPUT_ON_FAILURE 1)

# Add compilation flag to select trace files path
add_definitions(-DTRACE_PATH="${CMAKE_CURRENT_BINARY_DIR}/trace/")

# Parse all test sources
include_directories(include/ test/include/)
add_subdirectory(test/src/)

# Compile all tests
add_all_tests()

# Create placeholder for wave traces
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/trace/")
message("-- Placeholder for trace files created at: ${CMAKE_CURRENT_BINARY_DIR}/trace/")
