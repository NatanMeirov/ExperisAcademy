TARGET= main

CXX=g++
CC=$(CXX)

CFLAGS= -g3 -pedantic -Wall
CXXFLAGS = -std=c++11
CXXFLAGS += -pedantic -Wall -Werror
CXXFLAGS += -g3

CPPFLAGS = -I../inc
CPPFLAGS += -I../../inc


SRC = ../../src
INC = ../../inc


check: $(TARGET)
	./$(TARGET)


main: main.cpp $(INC)/extremes.hpp



clean:
	$(RM) $(TARGET)


.PHONY: clean check