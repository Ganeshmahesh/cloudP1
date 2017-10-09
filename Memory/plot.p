# Gnuplot script file for plotting data in file "force.dat"
# This file is called   force.p
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtics out nomirror
set ytic auto                         # set ytics automatically
set title "Sequential Read and Write"
set xlabel "Number of Threads"
set ylabel "Throughput(MB/sec)"
set xr [0:10]
set yr [0:200]
plot   "12" using 1:2 title '8KB' with linespoints , \
            "13" using 1:2 title '8MB' with linespoints , \
              "14" using 1:2 title '80MB' with linespoints
set term png
set output "Read_WritePerf.png"
replot

plot    "22" using 1:2 title '8KB' with linespoints , \
            "23" using 1:2 title '8MB' with linespoints , \
              "24" using 1:2 title '80MB' with linespoints
set term png
set yr [0:200]
set output "Seq_WritePerf.png"
set title "Sequential Write"
replot

plot    "32" using 1:2 title '8KB' with linespoints , \
            "33" using 1:2 title '8MB' with linespoints , \
              "34" using 1:2 title '80MB' with linespoints	  
set term png
set yr [0:9000]
set title "Random Write"
set output "Rand_WritePerf.png"
replot

plot    "11" using 1:2 title 'Sequential Read/Write' with linespoints , \
			"21" using 1:2 title 'Sequential Write' with linespoints , \
				"31" using 1:2 title 'Random Write' with linespoints
set term png
set yr [0:0.2]
set output "Latency.png"
set ylabel "Latency(us)"
replot

set term x11

#load 'saveplot'
#!mv my-plot.ps force.ps
#!lpr force.ps
