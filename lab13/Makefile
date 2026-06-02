CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
TARGET = erf_kwadratura

.PHONY: all run plots clean

all: $(TARGET) run plots

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp -lm

run: $(TARGET)
	./$(TARGET)

plots:
	gnuplot plot_x1.gp
	gnuplot plot_x2.gp
	gnuplot plot_x3.gp

clean:
	rm -f $(TARGET) *.dat *.png *.gp
