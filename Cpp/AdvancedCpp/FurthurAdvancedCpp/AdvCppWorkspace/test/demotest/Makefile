TARGET= main

CXX=g++
CC=$(CXX)

CFLAGS= -g3 -pedantic -Wall # Should remain here, because the linker uses the -g flag
CXXFLAGS = -std=c++11
CXXFLAGS += -pedantic -Wall -Werror
CXXFLAGS += -g3 # If we do not need DEBUG info: just put this in a comment

CPPFLAGS = -I../inc # CPP does NOT stands for c++, but C PRE-PROCESSOR!

# $(TARGET): main.cpp # NOT NEEDED AT ALL - Make is smart enough to know that alone... BUT: ONLY IF IT ONLY depends on 1 FILE!!!


clean: # RM 
	$(RM) $(TARGET)