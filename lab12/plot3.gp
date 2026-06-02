set terminal pngcairo size 1000,700
set output 'wykres3_bledy.png'
set logscale y
set xlabel "Stopien wielomianu n"
set ylabel "max|p(x)-f(x)|"
set title "Zbieznosc interpolacji"
set grid
plot 'bledy.dat' u 1:2 w lp title 'rownoodlegle',\
     'bledy.dat' u 1:3 w lp title 'Czebyszew'
