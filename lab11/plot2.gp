# ──────────────────────────────────────────────────────────────
# plot2.gp: Profile U(x, t_i) dla wybranych chwil czasu
#   — linia ciągła = rozwiązanie analityczne (kol. 3)
#   — punkty       = rozwiązanie numeryczne  (kol. 2)
# Format pliku: bloki gnuplot (index 0..5), podwójne puste linie
# t_snap = { 0.00, 0.05, 0.20, 0.50, 1.00, 2.00 }
# ──────────────────────────────────────────────────────────────
set terminal pngcairo size 1100,750 font 'Sans,13'

# ── Wykres dla KMB ─────────────────────────────────────────────
set output 'wykres2a_profil_kmb.png'
set xlabel 'x' font 'Sans,14'
set ylabel 'U(x, t)' font 'Sans,14'
set title 'Profil U(x,t) — KMB: h=0.05, λ=0.40' font 'Sans,15'
set key top right box lw 0.5 spacing 1.2
set grid lc rgb '#cccccc' lw 0.4
set yrange [-0.04:0.95]
set xrange [-5:5]    # ogony poza [-5,5] sa ~0 -- skupiamy sie na dynamice Gaussa
set label 1 'linia: analityczne; punkty: numeryczne' at graph 0.02,0.94 \
    font 'Sans,11' tc rgb '#333333'

plot \
  'profil_kmb.dat' index 0 u 1:3 w l  lw 2   lc 'black'        title 't=0.00 (an.)', \
  'profil_kmb.dat' index 0 u 1:2 every 4 w p pt 7 ps 0.9 lc 'black' notitle, \
  'profil_kmb.dat' index 1 u 1:3 w l  lw 2   lc 'blue'         title 't=0.05 (an.)', \
  'profil_kmb.dat' index 1 u 1:2 every 4 w p pt 7 ps 0.9 lc 'blue' notitle, \
  'profil_kmb.dat' index 2 u 1:3 w l  lw 2   lc rgb '#228B22' title 't=0.20 (an.)', \
  'profil_kmb.dat' index 2 u 1:2 every 4 w p pt 7 ps 0.9 lc rgb '#228B22' notitle, \
  'profil_kmb.dat' index 3 u 1:3 w l  lw 2   lc 'orange'       title 't=0.50 (an.)', \
  'profil_kmb.dat' index 3 u 1:2 every 4 w p pt 7 ps 0.9 lc 'orange' notitle, \
  'profil_kmb.dat' index 4 u 1:3 w l  lw 2   lc 'red'          title 't=1.00 (an.)', \
  'profil_kmb.dat' index 4 u 1:2 every 4 w p pt 7 ps 1.1 lc 'red' notitle, \
  'profil_kmb.dat' index 5 u 1:3 w l  lw 2   lc 'purple'       title 't=2.00 (an.)', \
  'profil_kmb.dat' index 5 u 1:2 every 4 w p pt 9 ps 1.2 lc 'purple' notitle

# ── Wykres dla ML+Thomas ────────────────────────────────────────
set output 'wykres2b_profil_ml.png'
set title 'Profil U(x,t) — Laasonen+Thomas: h=0.05, λ=1.00' font 'Sans,15'

plot \
  'profil_ml.dat' index 0 u 1:3 w l  lw 2   lc 'black'        title 't=0.00 (an.)', \
  'profil_ml.dat' index 0 u 1:2 every 4 w p pt 7 ps 0.9 lc 'black' notitle, \
  'profil_ml.dat' index 1 u 1:3 w l  lw 2   lc 'blue'         title 't=0.05 (an.)', \
  'profil_ml.dat' index 1 u 1:2 every 4 w p pt 7 ps 0.9 lc 'blue' notitle, \
  'profil_ml.dat' index 2 u 1:3 w l  lw 2   lc rgb '#228B22' title 't=0.20 (an.)', \
  'profil_ml.dat' index 2 u 1:2 every 4 w p pt 7 ps 0.9 lc rgb '#228B22' notitle, \
  'profil_ml.dat' index 3 u 1:3 w l  lw 2   lc 'orange'       title 't=0.50 (an.)', \
  'profil_ml.dat' index 3 u 1:2 every 4 w p pt 7 ps 0.9 lc 'orange' notitle, \
  'profil_ml.dat' index 4 u 1:3 w l  lw 2   lc 'red'          title 't=1.00 (an.)', \
  'profil_ml.dat' index 4 u 1:2 every 4 w p pt 7 ps 1.1 lc 'red' notitle, \
  'profil_ml.dat' index 5 u 1:3 w l  lw 2   lc 'purple'       title 't=2.00 (an.)', \
  'profil_ml.dat' index 5 u 1:2 every 4 w p pt 9 ps 1.2 lc 'purple' notitle

# ── Wykres dla ML+Gauss-Seidel ─────────────────────────────────
set output 'wykres2c_profil_ml_gs.png'
set title 'Profil U(x,t) — Laasonen+Gauss-Seidel: h=0.05, λ=1.00' font 'Sans,15'

plot \
  'profil_ml_gs.dat' index 0 u 1:3 w l  lw 2   lc 'black'        title 't=0.00 (an.)', \
  'profil_ml_gs.dat' index 0 u 1:2 every 4 w p pt 7 ps 0.9 lc 'black' notitle, \
  'profil_ml_gs.dat' index 1 u 1:3 w l  lw 2   lc 'blue'         title 't=0.05 (an.)', \
  'profil_ml_gs.dat' index 1 u 1:2 every 4 w p pt 7 ps 0.9 lc 'blue' notitle, \
  'profil_ml_gs.dat' index 2 u 1:3 w l  lw 2   lc rgb '#228B22' title 't=0.20 (an.)', \
  'profil_ml_gs.dat' index 2 u 1:2 every 4 w p pt 7 ps 0.9 lc rgb '#228B22' notitle, \
  'profil_ml_gs.dat' index 3 u 1:3 w l  lw 2   lc 'orange'       title 't=0.50 (an.)', \
  'profil_ml_gs.dat' index 3 u 1:2 every 4 w p pt 7 ps 0.9 lc 'orange' notitle, \
  'profil_ml_gs.dat' index 4 u 1:3 w l  lw 2   lc 'red'          title 't=1.00 (an.)', \
  'profil_ml_gs.dat' index 4 u 1:2 every 4 w p pt 7 ps 1.1 lc 'red' notitle, \
  'profil_ml_gs.dat' index 5 u 1:3 w l  lw 2   lc 'purple'       title 't=2.00 (an.)', \
  'profil_ml_gs.dat' index 5 u 1:2 every 4 w p pt 9 ps 1.2 lc 'purple' notitle
