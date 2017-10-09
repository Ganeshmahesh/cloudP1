set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtics out nomirror
set ytic auto                        # set ytics automatically
set title "Processor Performance by IOPS and FLOPS"
set xlabel "Threads"
set ylabel "Latency(ms)"
set xr [0:10]
set yr [0:0.0005]

plot    "11" using 1:2 title 'read_write' with linespoints , \
          "21" using 1:2 title 'seq_read' with linespoints , \
          "31" using 1:2 title 'rand_read' with linespoints

          set term png
          set output "Lat.png"
          replot
          set term x11
                                                                                              
                       
