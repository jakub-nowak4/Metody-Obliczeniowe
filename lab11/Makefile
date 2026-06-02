CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET   = lab11d

# ── Budowanie ─────────────────────────────────────────
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp -lm
	@echo "Skompilowano: $(TARGET)"

# ── Uruchomienie i generowanie wykresów ───────────────
all: $(TARGET) run plots

run: $(TARGET)
	./$(TARGET)

plots: run
	gnuplot plot1.gp
	gnuplot plot2.gp
	gnuplot plot3.gp
	@echo "Wykresy gotowe: wykres1_*.png, wykres2a/b/c_*.png, wykres3_*.png"

# ── Czyszczenie ───────────────────────────────────────
clean:
	rm -f $(TARGET) *.dat *.png

.PHONY: all run plots clean
