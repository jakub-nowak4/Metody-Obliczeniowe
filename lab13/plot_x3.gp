set terminal pngcairo size 900,650
set output 'wykres_erf_x3.png'
set logscale xy
set grid
set xlabel "n (liczba podprzedzialow)"
set ylabel "|I_num - erf(x)|"
set title "Zbieznosc kwadratury: erf(3.0)"
set key top right
C1 = 0.07; C2 = 0.003; C4 = 0.001
plot 'bledy_x3.dat' u 1:2 w lp lw 2 pt 7  title 'prosto. lewa  O(h^1)',  \
     'bledy_x3.dat' u 1:3 w lp lw 2 pt 9  title 'prosto. prawa O(h^1)',  \
     'bledy_x3.dat' u 1:4 w lp lw 2 pt 5  title 'prosto. srodek O(h^2)', \
     'bledy_x3.dat' u 1:5 w lp lw 2 pt 13 title 'trapezy O(h^2)',        \
     'bledy_x3.dat' u 1:6 w lp lw 2 pt 11 title 'Simpson O(h^4)',        \
     C1/x      w l lw 1 dt 2 lc 'gray' title 'ref O(1/n)',   \
     C2/x**2   w l lw 1 dt 3 lc 'gray' title 'ref O(1/n^2)', \
     C4/x**4   w l lw 1 dt 4 lc 'gray' title 'ref O(1/n^4)'
