#!/bin/bash

echo  "---Building App---"

if [ ! -d "build" ]
then
    mkdir build
fi
cd build
cmake ..
cmake --build .

echo  "---Running App---"
./ps
cd ..
