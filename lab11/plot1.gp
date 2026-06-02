# ──────────────────────────────────────────────────────────────
# plot1.gp: Zbieżność max|błąd|(T_max) vs h  —  skala log-log
# Oczekiwany rząd: O(h^2) dla obu metod (λ stałe → δt = λh^2/D)
# Schemat stabilności von Neumanna:
#   KMB:  g = 1-4λ sin^2(...), stabilna gdy λ ≤ 0.5
#   ML:   g = 1/(1+4λ sin^2(...)) < 1 zawsze
# ──────────────────────────────────────────────────────────────
set terminal pngcairo size 900,650 font 'Sans,13'
set output 'wykres1_zbieznosc.png'

set logscale xy
set grid xtics ytics mxtics mytics lc rgb '#999999' lw 0.4
set format x "%.3f"
set format y "%.0e"

set xlabel 'Krok przestrzenny  h' font 'Sans,14' offset 0,-0.5
set ylabel 'max|U_{num} − U_{an}|   (w  t = T_{max})' font 'Sans,14'
set title  'Zbieżność błędu końcowego vs h' font 'Sans,15'
set key top left box lw 0.5 spacing 1.3

# Linia referencyjna O(h^2): C = e_KMB(h=0.1) / h^2
C_ref = 7.724826586372191e-05 / 0.10**2
plot \
  'konwergencja.dat' u 1:2 w lp lw 2.2 pt 7 ps 2.0 lc rgb '#4169E1' \
    title 'KMB  (jawna,  λ=0.40)', \
  'konwergencja.dat' u 1:4 w lp lw 2.2 pt 9 ps 2.1 lc rgb '#B22222' \
    title 'Laasonen+Thomas  (λ=1.00)', \
  'konwergencja.dat' u 1:6 w lp lw 2.2 pt 5 ps 2.0 lc rgb '#228B22' \
    title 'Laasonen+Gauss-Seidel  (λ=1.00)', \
  [1e-4:1] C_ref * x**2 w l lw 2.0 dt 3 lc rgb '#555555' \
    title 'O(h²)  — rząd teoret.'
