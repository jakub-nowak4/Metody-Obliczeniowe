CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

TARGET = main
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.dat *.plt *.png

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
