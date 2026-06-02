set terminal pngcairo size 1000,700 font "sans,12"
set output "bme_stabilne.png"
set title  "BME, dt = 0.010 -- STABILNIE  (max |g_k| = 0.45)"
set xlabel "t"
set ylabel "y(t)"
set xrange [0:1]
set grid
set key top right
plot "bme_stabilne.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \
     ""                u 1:3 w p  pt 7 ps 1.2 lc rgb "#d62728" t "BME"
