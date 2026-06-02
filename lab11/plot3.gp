# ──────────────────────────────────────────────────────────────
# plot3.gp: max|błąd|(t) vs t  przez cały przedział [0, T_max]
#
# Oczekiwane zachowanie:
#   Lokalny błąd obcięcia LTE ∝ ∂⁴U/∂x⁴ ∝ (τ+t)^{-5/2}
#   — przy t=0: wąski Gauss → wielka wartość ∂⁴U/∂x⁴ → duże LTE
#   — z czasem Gauss się rozszerza → ∂⁴U/∂x⁴ maleje → LTE maleje
#   — błąd nie jest monotoniczny: rośnie od 0, osiąga maksimum
#     ok. t ≈ 2τ/3 ≈ 0.067, po czym maleje
# ──────────────────────────────────────────────────────────────
set terminal pngcairo size 1000,680 font 'Sans,13'
set output 'wykres3_blad_vs_t.png'

set logscale y
set grid xtics ytics mxtics mytics lc rgb '#aaaaaa' lw 0.4
set format y "%.0e"

set xlabel 't' font 'Sans,14'
set ylabel 'max|U_{num}(t) − U_{an}(t)|' font 'Sans,14'
set title  'Ewolucja błędu w czasie: h=0.05' font 'Sans,15'
set key top right box lw 0.5 spacing 1.3

plot \
  'blad_t_kmb.dat' u 1:2 w l lw 2.5 lc rgb '#4169E1' \
    title 'KMB  (jawna,  λ=0.40,  δt=0.001)',  \
  'blad_t_ml.dat'  u 1:2 w l lw 2.5 lc rgb '#B22222' dt 2 \
    title 'Laasonen+Thomas  (λ=1.00,  δt=0.0025)', \
  'blad_t_ml_gs.dat' u 1:2 w l lw 2.5 lc rgb '#228B22' dt 3 \
    title 'Laasonen+Gauss-Seidel  (λ=1.00,  δt=0.0025)'
