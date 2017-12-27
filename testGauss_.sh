#!/bin/bash

rm -f "A.dat"
rm -f "B.dat"
rm -f "A1.dat"
rm -f "B1.dat"

log="log_gauss_standalone.txt"



if [ -f $log ];then
	rm -f $log
fi

exe="Gauss_"


echo "generate matrix:"
python genMatrix.py 10 10 100 f # matrix A[10x10] , B[10x10], maxElement=100,(minElement=-100), dtype=int 
if [ -f $exe ]; then
	echo "#########################################"
	echo " test1:"
	echo
	if [[ -f 'Ag.dat' && -f 'Bg.dat' ]];then
		./$exe Ag.dat Bg.dat | tee $log
	fi
	echo "#########################################"
	echo " test2:"
	echo
	if [[ -f "Ac.dat" && -f "Bc.dat" ]];then
		./$exe Ac.dat Bc.dat | tee -a $log
	fi
	echo "#########################################"
	echo " test3:"
	echo
	if [[ -f "A.dat" && -f "B.dat" ]];then
		./$exe A.dat B.dat | tee -a $log
	fi
	echo "#########################################"
	echo " test4:"
	echo
	./$exe | tee -a $log # generate random matrix[5x5]
fi

