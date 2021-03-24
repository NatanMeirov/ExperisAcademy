# makefile for TestSortingAlgorithms.out

TARGET= TestSortingAlgorithms.out
CC= gcc
CFLAGS= -ansi -pedantic -g
OBJS= TestSortingAlgorithms.o SortingAlgorithms.o ADTDefs.o Vector.o Stack.o
VEC_PATH= ../../Dynamic_Vector/
STACK_PATH = ../../Stack/

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

Vector.o: $(VEC_PATH)Vector.c $(VEC_PATH)Vector.h ADTDefs.h
	$(CC) $(CFLAGS) -c $(VEC_PATH)Vector.c

Stack.o: $(STACK_PATH)Stack.c $(STACK_PATH)Stack.h $(VEC_PATH)Vector.h ADTDefs.h
	$(CC) $(CFLAGS) -c $(STACK_PATH)Stack.c

ADTDefs.o: ADTDefs.c ADTDefs.h
	$(CC) $(CFLAGS) -c ADTDefs.c

SortingAlgorithms.o: SortingAlgorithms.c SortingAlgorithms.h $(VEC_PATH)Vector.h $(STACK_PATH)Stack.h ADTDefs.h
	$(CC) $(CFLAGS) -c SortingAlgorithms.c

TestSortingAlgorithms.o: TestSortingAlgorithms.c SortingAlgorithms.h $(VEC_PATH)Vector.h ADTDefs.h
	$(CC) $(CFLAGS) -c TestSortingAlgorithms.c

clean:
	rm -f $(TARGET) $(OBJS)
