CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -pedantic

.PHONY: all run plots clean

all: runge

runge: main.cpp
	$(CXX) $(CXXFLAGS) -o runge main.cpp

run: runge
	./runge

plots: run
	gnuplot plot1.gp
	gnuplot plot2.gp
	gnuplot plot3.gp

clean:
	rm -f runge funkcja.dat bledy.dat interp_unif_n*.dat interp_cheb_n*.dat \
	      plot1.gp plot2.gp plot3.gp \
	      wykres1_rownoodlegle.png wykres2_czebyszew.png wykres3_bledy.png
