set terminal pngcairo size 1000,700
set output 'wykres1_rownoodlegle.png'
set title "Zjawisko Rungego -- wezly rownoodlegle, a=25"
set xlabel "x"; set ylabel "y"
set yrange [-0.6:0.6]
set key top left
plot 'funkcja.dat'           u 1:2 w l lw 3 lc 'black' title 'f(x)',\
     'interp_unif_n5.dat'    u 1:2 w l lw 1.5 dt 2 title 'n=5',\
     'interp_unif_n10.dat'   u 1:2 w l lw 1.5 dt 3 title 'n=10',\
     'interp_unif_n15.dat'   u 1:2 w l lw 1.5 dt 4 title 'n=15',\
     'interp_unif_n20.dat'   u 1:2 w l lw 1.5       title 'n=20'
