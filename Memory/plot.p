# Gnuplot script file for plotting data in file "force.dat"
# This file is called   force.p
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
#set xtic auto                          # set xtics automatically
set xtics 1,2,8
set ytic auto                          # set ytics automatically
set title "Throughput for Different Blocks and threads"
set xlabel "Threads"
set ylabel "Throughput(Mb/s)"
#set key 0.01,100
#set label "Yield Point" at 0.003,260
#set arrow from 0.0028,250 to 0.003,280
set xr [0:10]
set yr [0:5]
plot    "throughput_result" using 2:3 title '8KB' with linespoints , \
         "throughput_result" using 2:3 title '8MB' with linespoints , \
		 "throughput_result" using 2:3 title '80MB' with linespoints
		 

#set term png
#set output "memPerf.png"
#replot
#set term x11

#load 'saveplot'
#!mv my-plot.ps force.ps
#!lpr force.ps