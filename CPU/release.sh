#!/bin/sh
#clean
make clean
rm IOPS
rm FLOPS
rm cpuPerf.png

#build
make

clear
#IOPS
echo "running IOPS tests\n"
echo "test with 1 thread is executing"
#thread 1
./test 1 2 1

echo "test with 2 thread is executing"
#thread 2
./test 1 2 2

echo "test with 4 thread is executing"
#thread 4
./test 1 2 4

echo "test with 8 thread is executing"
#thread 8
./test 1 2 8

#FLOPS
echo "running FLOPS tests"
#thread 1
echo "test with 1 thread is executing"
./test 2 2 1

#thread 2
echo "test with 2 thread is executing"
./test 2 2 2

#thread 4
echo "test with 4 thread is executing"
./test 2 2 4

#thread 8
echo "test with 1 thread is executing"
./test 2 2 8

echo "generating IOPS and FLOPS plot"
gnuplot 'plot.p'

#perform linpack test
echo "executing linpack benchmark"
cd linpack/
./runme_xeon64

