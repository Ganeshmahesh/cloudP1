set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtics out nomirror
set ytic auto                         # set ytics automatically
set title "Sequential Read Performane for Disk"
set xlabel "Threads"
set ylabel "Throughput(MB/sec)"
set xr [0:10]
set yr [0:15000]

plot      "22" using 1:2 title '8KB' with linespoints , \
            "23" using 1:2 title '8MB' with linespoints , \
              "24" using 1:2 title '80MB' with linespoints

          set term png
          set output "Seq_ReadPerf.png"
          replot
set term x11
