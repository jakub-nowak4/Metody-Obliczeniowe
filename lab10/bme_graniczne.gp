set terminal pngcairo size 1000,700 font "sans,12"
set output "bme_graniczne.png"
set title  "BME, dt = 1/27 = 0.037 -- PROG STABILNOSCI  (max |g_k| = 1.04)"
set xlabel "t"
set ylabel "y(t)"
set xrange [0:1]
set grid
set key top right
plot "bme_graniczne.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \
     ""                 u 1:3 w p  pt 7 ps 1.2 lc rgb "#d62728" t "BME"
