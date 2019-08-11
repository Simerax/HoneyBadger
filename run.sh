#!/bin/bash
./build/bin/HoneyBadgerCompiler first.hb
clang++ first.hb.o -Lbuild -lHoneyBadger -o first
echo
echo binary created...running now
echo
LD_LIBRARY_PATH=./build  ./first