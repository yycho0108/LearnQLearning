# TICTACTOE
CC = g++
FLAGS = -std=c++11 -O3
LIBS = -L/usr/lib -L../lib -lQ #include Framework Library
INCS = -I/usr/include -I./include -I../include

BUILD = build
SRC = src

OBJECTS = $(BUILD)/TTTQ.o \
		  $(BUILD)/TTTSA.o \
		  $(BUILD)/TTTState.o \
		  $(BUILD)/TTTAction.o

TARGET = $(BUILD)/main

all: dir lib $(TARGET)

lib:
	$(MAKE) -C ../

dir:
	mkdir -p $(BUILD)

clean:
	rm $(BUILD)/*.o $(BUILD)/main 2> /dev/null

$(BUILD)/TTTQ.o: $(SRC)/TTTQ.cpp
$(BUILD)/TTTSA.o: $(SRC)/TTTSA.cpp
$(BUILD)/TTTState.o: $(SRC)/TTTState.cpp
$(BUILD)/TTTAction.o: $(SRC)/TTTAction.cpp

$(TARGET) : $(OBJECTS) $(SRC)/main.cpp
	$(CC) $(FLAGS) $(INCS) $^ -o $@ $(LIBS)

$(OBJECTS) :
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)


