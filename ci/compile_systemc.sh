#!/bin/bash

echo "SystemC Prefix: ${SYSTEMC_PREFIX}"
ls ${SYSTEMC_PREFIX}

mkdir tmp
cd tmp
wget https://github.com/systemc/systemc-2.3/archive/master.zip
unzip master.zip
cd systemc-2.3-master/
mkdir objdir
cd objdir
../configure
make
make install
