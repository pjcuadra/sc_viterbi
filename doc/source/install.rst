.. SystemC Viterbi documentation master file, created by
   sphinx-quickstart on Tue May 30 18:33:15 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Development Environment Setup
=============================

Install SystemC Library
-----------------------

On Linux
********

To install **SystemC** on Ubuntu 16.04, first you have to install some
dependencies, by running;

.. code::

  sudo apt-get install build-essential

Download **SystemC 2.3** from https://github.com/systemc/systemc-2.3. After
downloading, extract it. Now we need to compile it by running;

.. code::

  cd <extracted-dir>
  ./configure --prefix=/usr/local/
  make
  sudo make install


No you can build the code by running;

.. code::

  cd <your-sc_viterbi-src-code-path>
  mkdir build
  cd build
  cmake ..
  make


Using Docker
************

Windows
+++++++

First install Docker by following
https://docs.docker.com/docker-for-windows/install/.

Now you should open a shell (cmd.exe, PowerShell, or other) and you'll download
a Docker image with SystemC preinstalled, for doing so type;

.. code::

  docker pull pjcuadra/sc_viterbi

After it's downloaded you have to start the container by running;

.. code::

  docker run -d --rm --name systemc -v <your-sc_viterbi-src-code-path>:/root/ \
  -p 1122:22 pjcuadra/sc_viterbi:latest

.. warning::

  Don't forget to update the *<your-sc_viterbi-src-code-path>* placeholder.

Linux
+++++

First install Docker by following
https://docs.docker.com/engine/installation/linux/ubuntu/#prerequisites

Now you should open a shell and you'll download a Docker image with SystemC
preinstalled, for doing so type;

.. code::

  docker pull pjcuadra/sc_viterbi

After it's downloaded you have to start the container by running;

.. code::

  docker run -d --rm --name systemc -v <your-sc_viterbi-src-code-path>:/root/ \
  -p 1122:22 pjcuadra/sc_viterbi:latest

.. warning::

  Don't forget to update the *<your-sc_viterbi-src-code-path>* placeholder.


Set up Netbeans
+++++++++++++++

TBD
