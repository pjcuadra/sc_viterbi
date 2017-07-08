Building System
===============

Our code is completely `IDE` independent. For supporting the compiling and
simulation of the code we had to develop our own building system using
`CMake <https://cmake.org/>`_.

The building system is based on two simple yet powerful `CMake` macros.

.. code::

  # Add source directory
  add_sources_directory(dir1 dir2 ...)

  # Add source file
  add_sources(file1 file2 ...)

The basic idea behind our building system is to specify directories having
source codes and inside every directory with sources specify which files are
source files. For example, `<your-sc_viterbi-src-code-path>/src/CMakeLists.txt`,
specifies `common`, `decoder` and `encoder` as sources directories as follows;

.. code::

  # Add source directory
  add_sources_directory(common/
                        encoder/
                        decoder/)

Furthermore, `<your-sc_viterbi-src-code-path>/src/decoder/CMakeLists.txt`
specifies `decoder.cpp` as a source code file as follows;

.. code::

  # Add sources
  add_sources(decoder.cpp)

.. note::
  * You can specify source code directories and files in the same
    `CMakeLists.txt` file.
  * You can specify as many source code directories and files using the same
    macro call (`add_sources_directory(...)` and `add_sources(...)`
    respectively).
  * Every source code directory added using `add_sources_directory(...)` has to
    contain a `CMakeLists.txt` file.

Our main `CMakeLists.txt` at `<your-sc_viterbi-src-code-path>` does the rest;

* Verifies that SystemC and other dependencies are installed on the system.
* Building all the source code as a library
* Link source code with SystemC and other dependencies libraries
* Building the tests using our testing framework described
  `here <testing_framework.html>`_.
* Link all the tests with the source code and other dependencies
* Enable testing
