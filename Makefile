CC = g++
DEBUGFLAGS = -std=c++11 -g
FLAGS = -std=c++11 -O3
LIBS = -L/usr/lib
INCS = -I/usr/include -I.
TARGET = main

all: $(TARGET)
$(TARGET): Utility.o Action.o State.o Q.o $(TARGET).cpp
	$(CC) -std=c++11 -g $(INCS) $^ -o $@ $(LIBS)
Utility.o : Utility.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
Action.o : Action.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
State.o: Utility.o Action.o State.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)
Q.o: Utility.o Action.o State.o Q.cpp
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)

clean:
	rm main Utility.o Action.o State.o Q.o
