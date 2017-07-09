#!/bin/bash

echo "Start" > plantuml.log

rm -rf plantuml.tmp
while read a
do
  echo "$a" >> plantuml.tmp
done
echo "$a" >> plantuml.tmp

plantweb -v --format png --engine plantuml plantuml.tmp >> plantuml.log
cat plantuml.png

rm -rf plantuml.tmp
rm -rf plantuml.png
rm -rf plantuml.log
