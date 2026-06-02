set terminal pngcairo size 1000,650 font "sans,12"
set output "porownanie.png"
set title "Porownanie rozwiazan numerycznych z analitycznym (n = 40)"
set xlabel "x"
set ylabel "U(x)"
set xrange [0:1]
set grid
set key bottom left
plot "rozwiazanie.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \
     ""               u 1:3 w p  pt 7 ps 1.6 lc rgb "#1f77b4" t "MR",          \
     ""               u 1:4 w p  pt 4 ps 1.6 lc rgb "#d62728" t "strzaly"
