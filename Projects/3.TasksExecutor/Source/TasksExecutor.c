/**
 * @file TasksExecutor.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Tasks Executor to schedule, manage and execute given tasks (functions) by period time in a correct wanted order
 * @version 1.0
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#define _XOPEN_SOURCE 600 /* to define clockid_t */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <time.h> /* clockid_t, struct timespec */
#include <string.h> /* strncpy */
#include "../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericVector/GenericVector.h"
#include "../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericHeap/GenericHeap.h"
#include "../Include/Task.h"
#include "../Include/TasksExecutor.h"


/* Defines: */

#define MAX_EXECUTOR_NAME_SIZE 100
#define LENGTH 25
#define MAX_SIZE_T ((size_t) -1)
#define UNUSED(x) ((void)x)

struct TasksExecutor
{
    char* m_taskExecutorName;
    Vector* m_tasksVector;
    clockid_t m_clockMode;
    int m_isPauseRequired;
};


/* Helper Functions Declarations: */

static TasksExecutor* InitializedNewTasksExecutor(TasksExecutor* newTasksExecutor, const char*_nameOfExecutor, clockid_t _clockMode);
static void TaskDestroyWrapper(void* _task);
static int TasksCompareFunctionWrapper(const void* _firstTask, const void* _secondTask);
static int UpdateTaskExecTimeWithVectorForEachWrapper(void* _task, size_t _unUsedIndex, void* _unUsedContext);

/*-------------------------------------- Main API Functions ---------------------------------------*/

TasksExecutor* TasksExecutorCreate(const char* _nameOfExecutor, clockid_t _clockMode)
{
    TasksExecutor* newTasksExecutor = NULL;

    if(!_nameOfExecutor)
    {
        return NULL;
    }

    if(_clockMode !=  CLOCK_REALTIME && _clockMode != CLOCK_REALTIME_COARSE && _clockMode != CLOCK_MONOTONIC)
    {
        return NULL;
    }

    newTasksExecutor = malloc(sizeof(TasksExecutor));
    if(!newTasksExecutor)
    {
        return NULL;
    }

    return InitializedNewTasksExecutor(newTasksExecutor, _nameOfExecutor, _clockMode);
}


void TasksExecutorDestroy(TasksExecutor** _executor)
{
    if(_executor && *_executor)
    {
        VectorDestroy(&(*_executor)->m_tasksVector, &TaskDestroyWrapper);

        free((*_executor)->m_taskExecutorName);

        free(*_executor);
        *_executor = NULL;
    }
}


int TasksExecutorAdd(TasksExecutor* _executor, int (*_taskFunction)(void *), void* _taskContext, size_t _executeEveryPeriodTimeInMiliseconds)
{
    Task* task = NULL;

    if(!_executor || !_taskFunction || _executeEveryPeriodTimeInMiliseconds == 0)
    {
        return 0;
    }

    task = TaskCreate(_taskFunction, _taskContext, _executor->m_clockMode, _executeEveryPeriodTimeInMiliseconds);
    if(!task)
    {
        return 0;
    }

    if(VectorAppend(_executor->m_tasksVector, (void*)task) != VECTOR_SUCCESS)
    {
        return 0;
    }

    return 1;
}


size_t TasksExecutorRun(TasksExecutor* _executor)
{
    Heap* tasksScheduler = NULL;
    Task* currentTask = NULL;
    int isExecuteAgainRequired;
    size_t executionCycles = 0;

    if(!_executor || !VectorSize(_executor->m_tasksVector))
    {
        return MAX_SIZE_T;
    }

    /* Update the next execution time of each Task */
    VectorForEach(_executor->m_tasksVector, &UpdateTaskExecTimeWithVectorForEachWrapper, NULL);

    /* Build the Heap using the given updated next execution time */
    tasksScheduler = HeapBuild(_executor->m_tasksVector, &TasksCompareFunctionWrapper);
    if(!tasksScheduler)
    {
        return MAX_SIZE_T;
    }

    while(HeapSize(tasksScheduler) /* There are more existing Tasks */ && !_executor->m_isPauseRequired)
    {
        currentTask = (Task*)HeapExtract(tasksScheduler);

        isExecuteAgainRequired = TaskExecute(currentTask);

        /* Handling the returned status code */
        if(isExecuteAgainRequired)
        {
            TaskUpdateNextExecutionTime(currentTask);
            HeapInsert(tasksScheduler, (void*)currentTask);
        }
        else /* Task has finished */
        {
            TaskDestroy(&currentTask);
        }

        executionCycles++;
    }

    /* Check if pause is required */
    if(_executor->m_isPauseRequired)
    {
        _executor->m_isPauseRequired = 0;
    }

    HeapDestroy(&tasksScheduler);

    return executionCycles;
}


size_t TasksExecutorPause(TasksExecutor* _executor)
{
    if(!_executor)
    {
        return MAX_SIZE_T;
    }

    _executor->m_isPauseRequired = 1;

    return VectorSize(_executor->m_tasksVector);
}

/*----------------------------------- End of Main API Functions -----------------------------------*/


/* Helper Functions: */

static TasksExecutor* InitializedNewTasksExecutor(TasksExecutor* newTasksExecutor, const char*_nameOfExecutor, clockid_t _clockMode)
{
    newTasksExecutor->m_taskExecutorName = malloc(MAX_EXECUTOR_NAME_SIZE);
    if(!newTasksExecutor->m_taskExecutorName)
    {
        return NULL;
    }
    strncpy(newTasksExecutor->m_taskExecutorName, _nameOfExecutor, MAX_EXECUTOR_NAME_SIZE);

    newTasksExecutor->m_clockMode = _clockMode;

    newTasksExecutor->m_isPauseRequired = 0;

    newTasksExecutor->m_tasksVector = VectorCreate(LENGTH, LENGTH);
    if(!newTasksExecutor->m_tasksVector)
    {
        return NULL;
    }

    return newTasksExecutor;
}


static void TaskDestroyWrapper(void* _task)
{
    TaskDestroy((Task**)&_task);
}


static int TasksCompareFunctionWrapper(const void* _firstTask, const void* _secondTask)
{
    return TaskIsExecutionBeforeOtherTask((Task*)_firstTask, (Task*) _secondTask);
}


static int UpdateTaskExecTimeWithVectorForEachWrapper(void* _task, size_t _unUsedIndex, void* _unUsedContext)
{
    UNUSED(_unUsedIndex);
    UNUSED(_unUsedContext);

    TaskUpdateNextExecutionTime((Task*)_task);

    return 1; /* Continue the iteration */
}
