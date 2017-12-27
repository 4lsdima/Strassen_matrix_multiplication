#!/bin/bash

exe="Gauss_"

if [ -f $exe ];then
	rm -f $exe
fi

g++ "gauss_test.cpp" -std=c++11 -o $exe -DNDEBUG -O3 -Ofast

if [ -f $exe ];then
  echo "Building complete!"
  echo
fi