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


main: main.cpp $(INC)/thread_group.hpp $(SRC)/thread_group.cpp $(SRC)/thread.cpp ../src/counter.cpp



clean:
	$(RM) $(TARGET)


.PHONY: clean check