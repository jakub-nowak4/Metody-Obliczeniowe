CC = gcc
CXX = g++
CXXFLAGS = -Wall -O2
LDFLAGS = -lm -lstdc++

TARGET = main
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

plot: run
	gnuplot wykres_podstawowa.gp
	gnuplot wykres_ulepszona.gp

clean:
	rm -f $(TARGET) bledy_podstawowa.txt bledy_ulepszona.txt wykres_podstawowa.png wykres_ulepszona.png

.PHONY: run clean plot
