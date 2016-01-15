CC = g++
#DEBUGFLAGS = std-c++11 -g
FLAGS = -std=c++11 -O3
LIBS = -L/usr/lib
INCS = -I/usr/include -I.
TARGET = main

all: $(TARGET)
$(TARGET): TicTacToeState.o TicTacToeQ.o Action.o Utility.o $(TARGET).cpp
	$(CC) $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
TicTacToeState.o: Action.o Utility.o TicTacToeState.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
TicTacToeQ.o: Action.o Utility.o TicTacToeQ.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
Action.o: Action.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
Utility.o: Utility.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)

clean:
	rm main TicTacToeState.o TicTacToeQ.o Action.o Utility.o
