# Gnuplot script file for plotting data in file "force.dat"
# This file is called   force.p
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
#set xtic auto                          # set xtics automatically
set xtics 0,2,10
set ytic auto                          # set ytics automatically
set title "Processor Performance by IOPS and FLOPS"
set xlabel "Threads"
set ylabel "GIOPS / GFLOPS"
#set key 0.01,100
#set label "Yield Point" at 0.003,260
#set arrow from 0.0028,250 to 0.003,280
set xr [0:10]
set yr [0:5]
plot    "IOPS" using 1:2 title 'IOPS' with linespoints , \
          "FLOPS" using 1:2 title 'FLOPS' with points

set term png
set output "cpuPerf.png"
replot
set term x11

load 'saveplot'
!mv my-plot.ps force.ps
!lpr force.ps


