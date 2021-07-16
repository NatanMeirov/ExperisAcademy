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


main: main.cpp $(INC)/thread_pool.hpp ../src/counter.cpp $(SRC)/thread_destruction_policies.cpp $(SRC)/barrier.cpp $(SRC)/sync_handler.cpp $(SRC)/suicide_mission.cpp $(SRC)/semaphore.cpp $(SRC)/two_way_multi_sync_handler.cpp



clean:
	$(RM) $(TARGET)


.PHONY: clean check