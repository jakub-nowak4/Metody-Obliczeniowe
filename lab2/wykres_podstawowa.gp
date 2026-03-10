set encoding utf8
set terminal pngcairo enhanced font "Verdana,10"
set output 'wykres_podstawowa.png'

set grid
set xlabel "log_{10}(x)"
set ylabel "log_{10}(|błąd względny|)"
set title "Zależność błędu względnego od x (metoda podstawowa)"

set xrange [-10:3]
set yrange [-20:2]

plot "bledy_podstawowa.txt" using 1:2 with lines lc rgb "purple" title "Błąd algorytmu f(x)"
