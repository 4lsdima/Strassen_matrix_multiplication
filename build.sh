#!/bin/bash

exe1="Simple"
exe2="Gauss"
exe3="Strassen"

if [ -f $exe1 ] || [ -f $exe2 ] || [ -f $exe3 ];then
	rm -f $exe1
	rm -f $exe2
	rm -f $exe3
fi



g++ simple_test.cpp -std=c++11 -o $exe1 -DNDEBUG -O3 -Ofast

g++ gauss_test.cpp -std=c++11 -o $exe2 -DNDEBUG -O3 -Ofast

g++ strassen_test.cpp -std=c++11 -o $exe3 -lboost_system -lboost_thread -DNDEBUG -O3 -Ofast

if [ -f $exe1 ] && [ -f $exe2 ] && [ -f $exe3 ];then
  echo "Building complete!"
  echo
fi