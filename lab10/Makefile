# =============================================================
#  Makefile dla zadania:
#    RR pierwszego rzedu - metody BME, PME, PMT
#
#  Cele:
#    make            -> kompilacja + uruchomienie + wykresy
#    make lab10      -> sama kompilacja
#    make run        -> kompilacja + uruchomienie (generuje *.dat)
#    make plots      -> wykresy w gnuplocie (*.png)
#    make clean      -> usuniecie binarki, *.dat, *.png, *.gp
# =============================================================

# --- kompilator i flagi ---
CXX      := g++
CXXFLAGS := -O2 -Wall -Wextra
LDFLAGS  :=

# --- pliki ---
SRC      := main.cpp
BIN      := lab10
DATA     := bme_stabilne.dat bme_graniczne.dat bme_niestabilne.dat \
            pme.dat pmt.dat bledy.dat
PLOTS    := bme_stabilne.png bme_graniczne.png bme_niestabilne.png \
            pme.png pmt.png zbieznosc.png
GP_FILES := bme_stabilne.gp bme_graniczne.gp bme_niestabilne.gp \
            pme.gp pmt.gp zbieznosc.gp

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

bme_stabilne.png: bme_stabilne.dat bme_stabilne.gp
	gnuplot bme_stabilne.gp

bme_graniczne.png: bme_graniczne.dat bme_graniczne.gp
	gnuplot bme_graniczne.gp

bme_niestabilne.png: bme_niestabilne.dat bme_niestabilne.gp
	gnuplot bme_niestabilne.gp

pme.png: pme.dat pme.gp
	gnuplot pme.gp

pmt.png: pmt.dat pmt.gp
	gnuplot pmt.gp

zbieznosc.png: bledy.dat zbieznosc.gp
	gnuplot zbieznosc.gp

# --- skrypty gnuplot generowane automatycznie, jesli nie istnieja ---
bme_stabilne.gp:
	@echo 'set terminal pngcairo size 1000,700 font "sans,12"'                                >  $@
	@echo 'set output "bme_stabilne.png"'                                                     >> $@
	@echo 'set title  "BME, dt = 0.010 -- STABILNIE  (max |g_k| = 0.45)"'                     >> $@
	@echo 'set xlabel "t"'                                                                    >> $@
	@echo 'set ylabel "y(t)"'                                                                 >> $@
	@echo 'set xrange [0:1]'                                                                  >> $@
	@echo 'set grid'                                                                          >> $@
	@echo 'set key top right'                                                                 >> $@
	@echo 'plot "bme_stabilne.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \' >> $@
	@echo '     ""                u 1:3 w p  pt 7 ps 1.2 lc rgb "#d62728" t "BME"'             >> $@

bme_graniczne.gp:
	@echo 'set terminal pngcairo size 1000,700 font "sans,12"'                                          >  $@
	@echo 'set output "bme_graniczne.png"'                                                              >> $@
	@echo 'set title  "BME, dt = 1/27 = 0.037 -- PROG STABILNOSCI  (max |g_k| = 1.04)"'                 >> $@
	@echo 'set xlabel "t"'                                                                              >> $@
	@echo 'set ylabel "y(t)"'                                                                           >> $@
	@echo 'set xrange [0:1]'                                                                            >> $@
	@echo 'set grid'                                                                                    >> $@
	@echo 'set key top right'                                                                           >> $@
	@echo 'plot "bme_graniczne.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \'          >> $@
	@echo '     ""                 u 1:3 w p  pt 7 ps 1.2 lc rgb "#d62728" t "BME"'                     >> $@

bme_niestabilne.gp:
	@echo 'set terminal pngcairo size 1000,700 font "sans,12"'                                                >  $@
	@echo 'set output "bme_niestabilne.png"'                                                                  >> $@
	@echo 'set title  "BME, dt = 1/12 = 0.083 -- NIESTABILNIE  (max |g_k| = 3.58, y wybucha)"'                >> $@
	@echo 'set xlabel "t"'                                                                                    >> $@
	@echo 'set ylabel "|y(t)|"'                                                                               >> $@
	@echo 'set xrange [0:1]'                                                                                  >> $@
	@echo 'set logscale y'                                                                                    >> $@
	@echo 'set format y "10^{%L}"'                                                                            >> $@
	@echo 'set grid'                                                                                          >> $@
	@echo 'set key top left'                                                                                  >> $@
	@echo 'plot "bme_niestabilne.dat" u 1:2          w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \'     >> $@
	@echo '     ""                   u 1:(abs($$3)) w lp pt 7 ps 1.2 lc rgb "#d62728" t "|y_{BME}|"'          >> $@

pme.gp:
	@echo 'set terminal pngcairo size 1000,700 font "sans,12"'                       >  $@
	@echo 'set output "pme.png"'                                                     >> $@
	@echo 'set title  "PME, dt = 0.010 -- bezwarunkowo stabilna"'                    >> $@
	@echo 'set xlabel "t"'                                                           >> $@
	@echo 'set ylabel "y(t)"'                                                        >> $@
	@echo 'set xrange [0:1]'                                                         >> $@
	@echo 'set grid'                                                                 >> $@
	@echo 'set key top right'                                                        >> $@
	@echo 'plot "pme.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"    t "analityczne", \' >> $@
	@echo '     ""       u 1:3 w p  pt 7 ps 1.2 lc rgb "#1f77b4" t "PME"'            >> $@

pmt.gp:
	@echo 'set terminal pngcairo size 1000,700 font "sans,12"'                       >  $@
	@echo 'set output "pmt.png"'                                                     >> $@
	@echo 'set title  "PMT, dt = 0.010 -- bezwarunkowo stabilna"'                    >> $@
	@echo 'set xlabel "t"'                                                           >> $@
	@echo 'set ylabel "y(t)"'                                                        >> $@
	@echo 'set xrange [0:1]'                                                         >> $@
	@echo 'set grid'                                                                 >> $@
	@echo 'set key top right'                                                        >> $@
	@echo 'plot "pmt.dat" u 1:2 w l  lw 1 dt 2 lc rgb "black"      t "analityczne", \' >> $@
	@echo '     ""       u 1:3 w p  pt 7 ps 1.2 lc rgb "dark-green" t "PMT"'           >> $@

zbieznosc.gp:
	@echo 'set terminal pngcairo size 900,600 font "sans,12"'        >  $@
	@echo 'set output "zbieznosc.png"'                               >> $@
	@echo 'set title  "Maksymalny blad bezwzgledny w funkcji dt"'    >> $@
	@echo 'set xlabel "dt"'                                          >> $@
	@echo 'set ylabel "|err|_{max}"'                                 >> $@
	@echo 'set logscale xy'                                          >> $@
	@echo 'set format x "10^{%L}"'                                   >> $@
	@echo 'set format y "10^{%L}"'                                   >> $@
	@echo 'set grid'                                                 >> $@
	@echo 'set key bottom right'                                     >> $@
	@echo 'plot "bledy.dat" u 1:2 w lp pt 7 ps 0.8 lc rgb "#d62728"    t "BME", \' >> $@
	@echo '     ""          u 1:3 w lp pt 5 ps 0.8 lc rgb "#1f77b4"    t "PME", \' >> $@
	@echo '     ""          u 1:4 w lp pt 9 ps 0.9 lc rgb "dark-green" t "PMT", \' >> $@
	@echo '     1e0*x       w l dt 2 lw 2 lc rgb "gray40" t "O(dt)",          \' >> $@
	@echo '     1e1*x**2    w l dt 3 lw 2 lc rgb "gray20" t "O(dt^2)"'           >> $@

# --- sprzatanie ---
.PHONY: clean
clean:
	rm -f $(BIN) $(DATA) $(PLOTS) $(GP_FILES)

# --- pomoc ---
.PHONY: help
help:
	@echo "Dostepne cele:"
	@echo "  make            - kompilacja + uruchomienie + wykresy"
	@echo "  make $(BIN)      - sama kompilacja"
	@echo "  make run        - uruchomienie programu (generuje *.dat)"
	@echo "  make plots      - wykresy w gnuplocie (*.png)"
	@echo "  make clean      - usuniecie binarki, *.dat, *.png, *.gp"
	@echo "  make help       - ta pomoc"
