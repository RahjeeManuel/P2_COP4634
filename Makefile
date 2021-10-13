OBJS = mt-collatz.o
SOURCE = mt-collatz.cpp
HEADER =
OUT = mt-collatz
CC = g++
FLAGS = -g -c -Wall
LFLAGS = -pthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

mt-collatz.o: mt-collatz.cpp
	$(CC) $(FLAGS) mt-collatz.cpp

clean:
	rm -f $(OBJS) $(OUT)