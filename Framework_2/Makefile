CC = g++
FLAGS = -std=c++11 -g
LIBS = -L/usr/lib
INCS = -I/usr/include -I./include

BUILD = build
SRC = src

OBJECTS = $(BUILD)/Q.o \
		  $(BUILD)/SA.o \
		  $(BUILD)/State.o \
		  $(BUILD)/Action.o \
		  $(BUILD)/Utility.o

TARGET = lib/libQ.a

all: dir $(TARGET)

dir:
	mkdir -p $(BUILD)
	mkdir -p lib

$(BUILD)/Q.o: $(SRC)/Q.cpp $(BUILD)/State.o $(BUILD)/Action.o $(BUILD)/SA.o $(BUILD)/Utility.o
$(BUILD)/SA.o: $(SRC)/SA.cpp $(BUILD)/State.o $(BUILD)/Action.o $(BUILD)/Utility.o
$(BUILD)/State.o: $(SRC)/State.cpp $(BUILD)/Action.o
$(BUILD)/Action.o: $(SRC)/Action.cpp
$(BUILD)/Utility.o: $(SRC)/Utility.cpp

$(TARGET) : $(OBJECTS)
	ar -rvs $(TARGET) $(OBJECTS)
$(OBJECTS):
	$(CC) -c $(FLAGS) $(INCS) $^ -o $@ $(LIBS)

clean:
	rm lib/libQ.a build/*.o
