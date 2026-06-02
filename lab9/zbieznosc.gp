set terminal pngcairo size 900,600 font "sans,12"
set output "zbieznosc.png"
set title  "Maksymalny blad bezwzgledny w funkcji h"
set xlabel "h"
set ylabel "|err|_{max}"
set logscale xy
set format x "10^{%L}"
set format y "10^{%L}"
set grid
set key bottom right
plot "bledy.dat" u 1:2 w lp pt 7 ps 0.8 t "MR",      \
     ""          u 1:3 w lp pt 6 ps 0.8 t "strzaly", \
     1e-1*x**2 w l dt 2 lw 2 t "O(h^2)"
