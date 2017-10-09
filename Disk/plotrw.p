# Gnuplot script file for plotting data in file "force.dat"
# This file is called   force.p
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtics out nomirror
set ytic auto                         # set ytics automatically
set title "Read Write Performance for Disk"
set xlabel "Threads"
set ylabel "Throughput (MB/sec)"
set xr [0:10]
set yr [0:1500]

plot      "12" using 1:2 title '8KB' with linespoints , \
            "13" using 1:2 title '8MB' with linespoints , \
              "14" using 1:2 title '80MB' with linespoints

set term png
set output "Read_WritePerf.png"
replot
set term x11

#load 'saveplot'
#!mv my-plot.ps force.ps
#!lpr force.ps
