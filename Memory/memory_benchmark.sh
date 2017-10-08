#!/bin/sh
#clean
make clean
#remove old files
#rm result.csv

#build
make

cd stream
gcc -fopenmp -D_OPENMP stream.c -o stream 
export OMP_NUM_THREADS=8
./stream

cd ..
for paramspace in {1,2,3}
do
	for no_threads in {1,2,4,8}
	do
		for block_size in {1,2,3,4} #8kb,8mb,80mb case, througput
		do
			echo "Parmeter space :$paramspace" , "Num of threads: $no_threads" , "Block Size: $block_size"
			./memory_benchmark $paramspace $no_threads $block_size
			echo "#################################################################################"
		done
	done	
done

gnuplot 'plot.p'
