#!/bin/bash

exe="M"

if [ -f $exe ];then
	rm $exe
fi

g++ "main.cpp" -o $exe -std=c++11 -lboost_system -lboost_thread -DNDEBUG -O3 -Ofast

if [ -f $exe ];then
  echo "Building complete!"
  echo "object is:" $exe
  echo
fi