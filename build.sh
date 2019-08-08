#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Debug -Bbuild -H.
cd build
make
cd ..