CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -O0

SRCS = main.cpp bisekcja.cpp picard.cpp newton.cpp sieczne.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
