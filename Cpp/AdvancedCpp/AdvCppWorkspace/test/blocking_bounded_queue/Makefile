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


main: main.cpp $(INC)/blocking_bounded_queue.hpp $(SRC)/sync_handler.cpp $(SRC)/barrier.cpp $(SRC)/thread_destruction_policies.cpp $(SRC)/semaphore.cpp


clean:
	$(RM) $(TARGET)


.PHONY: clean check