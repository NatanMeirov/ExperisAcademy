# makefile TasksExecutor
# IMPORTANT NOTE: After using make (that should fail - but create the .o files), use: make task, make tasktimer, make tasksexecutor: to compile particular file, and then use: {lib_label_content} IN THE TERMINAL

TARGET= Test_TasksExecutor.out
CC= gcc
CFLAGS= -ansi -pedantic -g3 -Wall -Werror -Wextra
OBJS= Test_TasksExecutor.o Test_TaskTimer.o Test_Task.o TasksExecutor.o Task.o TaskTimer.o
OBJS_TASK= Test_Task.o Task.o TaskTimer.o
OBJS_TASKTIMER= Test_TaskTimer.o TaskTimer.o
OBJS_TASKSEXECUTOR= Test_TasksExecutor.o TasksExecutor.o Task.o TaskTimer.o
INCLUDE_PATH= ../Include/
SOURCE_PATH= ../Source/
GENERIC_DATA_STRUCTURES_PATH= ../../../Libraries/DynamicLibraries/GenericDataStructures
SOURCE_GENERIC_DATA_STRUCTURES_PATH= ../../../Libraries/DynamicLibraries/GenericDataStructures/Source/
INCLUDE_GENERIC_DATA_STRUCTURES_PATH= ../../../Libraries/DynamicLibraries/GenericDataStructures/Include/


$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L$(GENERIC_DATA_STRUCTURES_PATH) -lGenericDataStructures

# $(TARGET1): $(OBJS1)
# 	$(CC) -o $(TARGET1) $(OBJS1)

# $(TARGET2): $(OBJS2)
# 	$(CC) -o $(TARGET2) $(OBJS2)

# $(TARGET3): $(OBJS3)
# 	$(CC) -o $(TARGET3) $(OBJS3) -L$(GENERIC_DATA_STRUCTURES_PATH) -lGenericDataStructures

Task.o: $(INCLUDE_PATH)Task.h $(SOURCE_PATH)TaskTimer.c $(INCLUDE_PATH)TaskTimer.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)Task.c

TaskTimer.o: $(INCLUDE_PATH)TaskTimer.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)TaskTimer.c

TasksExecutor.o: $(INCLUDE_PATH)Task.h $(SOURCE_PATH)Task.c $(SOURCE_PATH)TaskTimer.c $(INCLUDE_PATH)TaskTimer.h $(INCLUDE_PATH)TasksExecutor.h $(SOURCE_GENERIC_DATA_STRUCTURES_PATH)GenericVector/GenericVector.c $(INCLUDE_GENERIC_DATA_STRUCTURES_PATH)GenericVector/GenericVector.h $(SOURCE_GENERIC_DATA_STRUCTURES_PATH)GenericHeap/GenericHeap.c $(INCLUDE_GENERIC_DATA_STRUCTURES_PATH)GenericHeap/GenericHeap.h
	$(CC) $(CFLAGS) -c $(SOURCE_PATH)TasksExecutor.c

Test_Task.o: $(INCLUDE_PATH)Task.h $(SOURCE_PATH)TaskTimer.c $(INCLUDE_PATH)TaskTimer.h
	$(CC) $(CFLAGS) -c Test_Task.c

Test_TaskTimer.o: $(SOURCE_PATH)TaskTimer.c $(INCLUDE_PATH)TaskTimer.h
	$(CC) $(CFLAGS) -c Test_TaskTimer.c

Test_TasksExecutor.o: $(INCLUDE_PATH)Task.h $(SOURCE_PATH)Task.c $(SOURCE_PATH)TaskTimer.c $(INCLUDE_PATH)TaskTimer.h $(INCLUDE_PATH)TasksExecutor.h $(SOURCE_GENERIC_DATA_STRUCTURES_PATH)GenericVector/GenericVector.c $(INCLUDE_GENERIC_DATA_STRUCTURES_PATH)GenericVector/GenericVector.h $(SOURCE_GENERIC_DATA_STRUCTURES_PATH)GenericHeap/GenericHeap.c $(INCLUDE_GENERIC_DATA_STRUCTURES_PATH)GenericHeap/GenericHeap.h
	$(CC) $(CFLAGS) -c Test_TasksExecutor.c

clean:
	rm -f Test_Task.out $(OBJS_TASK)
	rm -f Test_TaskTimer.out $(OBJS_TASKTIMER)
	rm -f Test_TasksExecutor.out $(OBJS_TASKSEXECUTOR)

task:
	$(CC) -o Test_Task.out $(OBJS_TASK)

tasktimer:
	$(CC) -o Test_TaskTimer.out $(OBJS_TASKTIMER)

tasksexecutor:
	$(CC) -o Test_TasksExecutor.out $(OBJS_TASKSEXECUTOR) -L$(GENERIC_DATA_STRUCTURES_PATH) -lGenericDataStructures

# lib:
# 	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../../Libraries/DynamicLibraries/GenericDataStructures
# 	export LD_LIBRARY_PATH