# =============================================================
#  Makefile dla zadania:
#    Liniowe RR zwyczajne 2-go rzedu (MR + metoda strzalow)
#
#  Cele:
#    make            -> kompilacja programu
#    make run        -> kompilacja + uruchomienie (generuje *.dat)
#    make plots      -> wykresy w gnuplocie (porownanie.png, porownanie_n4.png, zbieznosc.png)
#    make all        -> wszystko: build + run + plots
#    make clean      -> usuniecie plikow wynikowych i binarki
# =============================================================

# --- kompilator i flagi ---
CXX      := g++
CXXFLAGS := -O2 -Wall -Wextra
LDFLAGS  :=

# --- pliki ---
SRC      := main.cpp
BIN      := lab9
DATA     := rozwiazanie.dat rozwiazanie_n4.dat bledy.dat
PLOTS    := porownanie.png porownanie_n4.png zbieznosc.png
GP_FILES := porownanie.gp porownanie_n4.gp zbieznosc.gp

# --- domyslny cel ---
.PHONY: all
all: $(BIN) run plots

# --- kompilacja ---
$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

# --- uruchomienie programu ---
.PHONY: run
run: $(BIN)
	./$(BIN)

# --- wykresy w gnuplocie ---
.PHONY: plots
plots: $(PLOTS)

porownanie.png: rozwiazanie.dat porownanie.gp
	gnuplot porownanie.gp

porownanie_n4.png: rozwiazanie_n4.dat rozwiazanie.dat porownanie_n4.gp
	gnuplot porownanie_n4.gp

zbieznosc.png: bledy.dat zbieznosc.gp
	gnuplot zbieznosc.gp

# --- skrypty gnuplot generowane automatycznie, jesli nie istnieja ---
porownanie.gp:
	@echo 'set terminal pngcairo size 1000,650 font "sans,12"'        >  $@
	@echo 'set output "porownanie.png"'                               >> $@
	@echo 'set title "Porownanie rozwiazan numerycznych z analitycznym (n = 40)"' >> $@
	@echo 'set xlabel "x"'                                            >> $@
	@echo 'set ylabel "U(x)"'                                         >> $@
	@echo 'set xrange [0:1]'                                          >> $@
	@echo 'set grid'                                                  >> $@
	@echo 'set key bottom left'                                       >> $@
	@echo 'plot "rozwiazanie.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \' >> $@
	@echo '     ""               u 1:3 w p  pt 7 ps 1.6 lc rgb "#1f77b4" t "MR",          \' >> $@
	@echo '     ""               u 1:4 w p  pt 4 ps 1.6 lc rgb "#d62728" t "strzaly"'  >> $@

porownanie_n4.gp:
	@echo 'set terminal pngcairo size 1000,650 font "sans,12"'        >  $@
	@echo 'set output "porownanie_n4.png"'                            >> $@
	@echo 'set title "Porownanie rozwiazan numerycznych z analitycznym (n = 4)"' >> $@
	@echo 'set xlabel "x"'                                            >> $@
	@echo 'set ylabel "U(x)"'                                         >> $@
	@echo 'set xrange [0:1]'                                          >> $@
	@echo 'set grid'                                                  >> $@
	@echo 'set key bottom left'                                       >> $@
	@echo '# gladka linia analityczna -- 41 punktow z rozwiazanie.dat (n=40)' >> $@
	@echo '# markery numeryczne -- 5 punktow z rozwiazanie_n4.dat'    >> $@
	@echo 'plot "rozwiazanie.dat"    u 1:2 w l  lw 2     lc rgb "black"    t "analityczne", \' >> $@
	@echo '     "rozwiazanie_n4.dat" u 1:3 w lp pt 7 ps 2.0 lc rgb "#1f77b4" t "MR (n=4)",   \' >> $@
	@echo '     ""                   u 1:4 w lp pt 4 ps 2.0 lc rgb "#d62728" t "strzaly (n=4)"' >> $@

zbieznosc.gp:
	@echo 'set terminal pngcairo size 900,600 font "sans,12"'        >  $@
	@echo 'set output "zbieznosc.png"'                               >> $@
	@echo 'set title  "Maksymalny blad bezwzgledny w funkcji h"'     >> $@
	@echo 'set xlabel "h"'                                           >> $@
	@echo 'set ylabel "|err|_{max}"'                                 >> $@
	@echo 'set logscale xy'                                          >> $@
	@echo 'set format x "10^{%L}"'                                   >> $@
	@echo 'set format y "10^{%L}"'                                   >> $@
	@echo 'set grid'                                                 >> $@
	@echo 'set key bottom right'                                     >> $@
	@echo 'plot "bledy.dat" u 1:2 w lp pt 7 ps 0.8 t "MR",      \'   >> $@
	@echo '     ""          u 1:3 w lp pt 6 ps 0.8 t "strzaly", \'   >> $@
	@echo '     1e-1*x**2 w l dt 2 lw 2 t "O(h^2)"'                  >> $@

# --- sprzatanie ---
.PHONY: clean
clean:
	rm -f $(BIN) $(DATA) $(PLOTS) $(GP_FILES)

# --- pomoc ---
.PHONY: help
help:
	@echo "Dostepne cele:"
	@echo "  make            - kompilacja + uruchomienie + wykresy"
	@echo "  make $(BIN)        - sama kompilacja"
	@echo "  make run        - uruchomienie programu (generuje *.dat)"
	@echo "  make plots      - wykresy w gnuplocie (*.png)"
	@echo "  make clean      - usuniecie binarki, *.dat, *.png, *.gp"
	@echo "  make help       - ta pomoc"
