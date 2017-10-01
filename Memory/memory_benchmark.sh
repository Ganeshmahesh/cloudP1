#!/bin/sh
#clean
make clean
#remove old files
#rm result.csv

#build
make

for paramspace in {1,2,3}
do
	for no_threads in {1,2,4,8}
	do
		for block_size in {2,3,4} #8kb,8mb,80mb case, througput
		do
			echo "Parmeter space :$paramspace" , "Num of threads: $no_threads" , "Block Size: $block_size"
			./memory_benchmark $paramspace $no_threads $block_size
			echo "#################################################################################"
		done
	done	
done

for paramspace in {1,2,3}
do
	for no_threads in {1,2,4,8}
	do
		for block_size in {1} #8b case and latency
		do
			echo "Parmeter space :$paramspace" , "Num of threads: $no_threads" , "Block Size: $block_size"
			./memory_benchmark $paramspace $no_threads $block_size
			echo "#################################################################################"
		done
	done	
done
	
#gnuplot 'plot.p'
