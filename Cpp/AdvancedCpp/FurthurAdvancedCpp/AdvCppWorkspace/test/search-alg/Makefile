TARGET= main

CXX=g++
CC=$(CXX)

CFLAGS= -g3 -pedantic -Wall # Should remain here, because the linker uses the -g3 flag
CXXFLAGS = -std=c++11
CXXFLAGS += -pedantic -Wall -Werror
CXXFLAGS += -g3 # If we do not need DEBUG info: just put this in a comment

CPPFLAGS = -I../inc # CPP does NOT stands for c++, but C PRE-PROCESSOR!
CPPFLAGS += -I../../inc # CPP does NOT stands for c++, but C PRE-PROCESSOR!


SRC = ../../src
INC = ../../inc


check: $(TARGET) # this line is the first - and run this first line, but it depends on the main executable file... line - so it goes there and after that this line
	./$(TARGET)


main: main.cpp $(INC)/search_alg.hpp $(INC)/fillers.hpp #MAKE ERROR: $(INC)/inl/fillers.hxx $(INC)/inl/search_alg.hxx # Depending on these files - write all dependencies

# NO NEED EXPLICIT RULES HERE!

clean: # RM is a pre-known constant!
	$(RM) $(TARGET)


.PHONY: clean check # add here: phony targets - targets that DOES NOT CREATE NEW FILES...