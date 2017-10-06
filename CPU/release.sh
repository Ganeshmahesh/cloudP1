#!/bin/sh
#clean
make clean
rm IOPS
rm FLOPS
rm cpuPerf.png

#build
make

#IOPS
#thread 1
./test 1 2 1

#thread 2
./test 1 2 2

#thread 4
./test 1 2 4

#thread 8
./test 1 2 8

#FLOPS
#thread 1
./test 2 2 1

#thread 2
./test 2 2 2

#thread 4
./test 2 2 4

#thread 8
./test 2 2 8

gnuplot 'plot.p'
