set terminal pngcairo size 1000,650 font "sans,12"
set output "porownanie_n4.png"
set title "Porownanie rozwiazan numerycznych z analitycznym (n = 4)"
set xlabel "x"
set ylabel "U(x)"
set xrange [0:1]
set grid
set key bottom left
# gladka linia analityczna -- 41 punktow z rozwiazanie.dat (n=40)
# markery numeryczne -- 5 punktow z rozwiazanie_n4.dat
plot "rozwiazanie.dat"    u 1:2 w l  lw 2     lc rgb "black"    t "analityczne", \
     "rozwiazanie_n4.dat" u 1:3 w lp pt 7 ps 2.0 lc rgb "#1f77b4" t "MR (n=4)",   \
     ""                   u 1:4 w lp pt 4 ps 2.0 lc rgb "#d62728" t "strzaly (n=4)"
