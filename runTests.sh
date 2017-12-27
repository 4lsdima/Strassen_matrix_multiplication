#!/bin/bash
log1="log_simple.txt"
log2="log_gauss.txt"
log3="log_strassen.txt"



if [ -f $log1 ];then
	rm -f $log1
elif [ -f $log2 ];then
	rm -f $log2
elif [ -f $log3 ];then
	rm -f $log3
fi


exe1="Simple"
exe2="Gauss"
exe3="Strassen"

echo "generate matrix:"
python genMatrix.py 10 15 100 i # matrix A[10x15] , B[15x10], maxElement=100,(minElement=-100), dtype=int 
echo "#########################################"
echo "Simple test... : "
if [ -f $exe1 ]; then
	./$exe1 | tee $log1
fi

echo "generate matrix:"
python genMatrix.py 10 10 100 f # matrix A[10x10] , B[10x10], maxElement=100,(minElement=-100), dtype=int 
echo "#########################################"
echo "test Gauss... : "

if [ -f $exe2 ]; then
	./$exe2 Ag.dat Bg.dat | tee $log2
	# ./$exe2 Ag.dat Bg.dat >> $log2
	./$exe2 | tee -a $log2
fi

echo "generate matrix:"
python genMatrix.py 1000 1000 100 f
echo "#########################################"
echo "test Strassen... : "

if [ -f $exe3 ]; then
	./$exe3 | tee $log3
fi

