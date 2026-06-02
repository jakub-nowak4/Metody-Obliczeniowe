set terminal pngcairo size 1000,700 font "sans,12"
set output "bme_niestabilne.png"
set title  "BME, dt = 1/12 = 0.083 -- NIESTABILNIE  (max |g_k| = 3.58, y wybucha)"
set xlabel "t"
set ylabel "|y(t)|"
set xrange [0:1]
set logscale y
set format y "10^{%L}"
set grid
set key top left
plot "bme_niestabilne.dat" u 1:2          w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \
     ""                   u 1:(abs($3)) w lp pt 7 ps 1.2 lc rgb "#d62728" t "|y_{BME}|"
