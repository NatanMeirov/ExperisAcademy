TARGET = main

CXX = g++
CC = $(CXX)

CFLAGS = -g3 -pedantic -Wall
CXXFLAGS = -std=c++11
CXXFLAGS += -pedantic -Wall -Werror
CXXFLAGS += -g3

CPPFLAGS = -I../inc
CPPFLAGS += -I../../inc

LDLIBS = -lpthread

SRC = ../../src
INC = ../../inc


check: $(TARGET)
	./$(TARGET)


main: main.cpp $(INC)/thread.hpp $(SRC)/thread_destruction_policies.cpp $(SRC)/sem.cpp $(SRC)/sync_handler.cpp $(SRC)/barrier.cpp ../src/counter.cpp


stress: CPPFLAGS += -DSTRESS_RACE_TEST # tells the compiler to define a macro (after -D<...>)
stress: $(TARGET) # on stress - does all the stress commands (on)
	./$(TARGET)

clean:
	$(RM) $(TARGET)


.PHONY: clean check