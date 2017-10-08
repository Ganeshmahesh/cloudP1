# Gnuplot script file for plotting data in file "force.dat"
# This file is called   force.p
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
#set xtic auto                          # set xtics automatically
#set xtics 0,2,10
set xtics out nomirror
set ytic auto                        # set ytics automatically
set title "Processor Performance by IOPS and FLOPS"
set xlabel "Threads"
set ylabel "GIOPS / GFLOPS"
#set key 0.01,100
#set label "Yield Point" at 0.003,260
#set arrow from 0.0028,250 to 0.003,280
#set xr [0:10]
set yr [0:1000]

plot    "11" using 1:2 title '8B' with linespoints , \
 "12" using 1:2 title '8KB' with linespoints , \
  "13" using 1:2 title '8MB' with linespoints , \
   "14" using 1:2 title '80MB' with linespoints

        set term png
        set output "Read_WritePerf.png"
        replot
        set term x11

