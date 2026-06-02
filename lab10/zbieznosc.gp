set terminal pngcairo size 900,600 font "sans,12"
set output "zbieznosc.png"
set title  "Maksymalny blad bezwzgledny w funkcji dt"
set xlabel "dt"
set ylabel "|err|_{max}"
set logscale xy
set format x "10^{%L}"
set format y "10^{%L}"
set grid
set key bottom right
plot "bledy.dat" u 1:2 w lp pt 7 ps 0.8 lc rgb "#d62728"    t "BME", \
     ""          u 1:3 w lp pt 5 ps 0.8 lc rgb "#1f77b4"    t "PME", \
     ""          u 1:4 w lp pt 9 ps 0.9 lc rgb "dark-green" t "PMT", \
     1e0*x       w l dt 2 lw 2 lc rgb "gray40" t "O(dt)",          \
     1e1*x**2    w l dt 3 lw 2 lc rgb "gray20" t "O(dt^2)"
