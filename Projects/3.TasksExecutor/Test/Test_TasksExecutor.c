#define _XOPEN_SOURCE 600 /* to define clockid_t */

#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t, malloc, free */
#include <time.h> /* clockid_t */
#include "../Include/TasksExecutor.h"

#define LENGTH 10
#define EXECUTION_TIMES 10

typedef struct IntArrayTaskObject
{
    int* m_array;
    size_t m_sizeOfArray;
    size_t m_totalTimesToExecute;
    size_t m_currentExecutedTimes;
    int m_sum;
} IntArrayTaskObject;

typedef struct PauseObject
{
    size_t m_totalTimesToExecuteTillPause;
    size_t m_currentExecutedTimes;
    TasksExecutor* m_executorToPause;
} PauseObject;

static void Test_General_TasksExecutor(void);

static void InitIntArray(int** _arr, size_t _size);
static int PrintIntArrayTask(void* _object);
static int SumIntArrayTask(void* _object);
static int PrintEvenNumIntArrayTask(void* _object);
static void InitIntArrayTaskObject(IntArrayTaskObject* _obj);
static int PauseTheExecution(void* _pauseObject);
static void InitPauseObject(PauseObject* _obj);



int main(void)
{
    Test_General_TasksExecutor();

    return 0;
}



static void InitIntArray(int** _arr, size_t _size)
{
    size_t i;

    (*_arr) = malloc(_size * sizeof(int));

    if(*_arr)
    {
        for(i = 0; i < _size; i++)
        {
            (*_arr)[i] = i;
        }
    }
}


static int PrintIntArrayTask(void* _object)
{
    size_t i;

    putchar('\n');
    putchar('{');
    putchar(' ');


    for(i = 0; i < ((IntArrayTaskObject*)_object)->m_sizeOfArray; i++)
    {
        printf("%d ", ((IntArrayTaskObject*)_object)->m_array[i]);
    }


    putchar('}');
    putchar('\n');


    ((IntArrayTaskObject*)_object)->m_currentExecutedTimes++;

    if(((IntArrayTaskObject*)_object)->m_currentExecutedTimes < ((IntArrayTaskObject*)_object)->m_totalTimesToExecute)
    {
        return 1; /* Continue executing the task */
    }

    return 0; /* End the task */
}


static int SumIntArrayTask(void* _object)
{
    size_t i;

    putchar('\n');

    ((IntArrayTaskObject*)_object)->m_sum = 0;
    for(i = 0; i < ((IntArrayTaskObject*)_object)->m_sizeOfArray; i++)
    {
        ((IntArrayTaskObject*)_object)->m_sum += ((IntArrayTaskObject*)_object)->m_array[i];
    }
    printf("---> Sum of the array: %d\n", ((IntArrayTaskObject*)_object)->m_sum);

    ((IntArrayTaskObject*)_object)->m_currentExecutedTimes++;

    if(((IntArrayTaskObject*)_object)->m_currentExecutedTimes < ((IntArrayTaskObject*)_object)->m_totalTimesToExecute)
    {
        return 1; /* Continue executing the task */
    }

    return 0; /* End the task */
}


static int PrintEvenNumIntArrayTask(void* _object)
{
    size_t i;

    printf("Printing only Even Numbers:\n");
    putchar('{');
    putchar(' ');


    for(i = 0; i < ((IntArrayTaskObject*)_object)->m_sizeOfArray; i++)
    {
        if(((IntArrayTaskObject*)_object)->m_array[i] % 2 == 0)
        {
            printf("%d ", ((IntArrayTaskObject*)_object)->m_array[i]);
        }
    }


    putchar('}');
    putchar('\n');


    ((IntArrayTaskObject*)_object)->m_currentExecutedTimes++;

    if(((IntArrayTaskObject*)_object)->m_currentExecutedTimes < ((IntArrayTaskObject*)_object)->m_totalTimesToExecute)
    {
        return 1; /* Continue executing the task */
    }

    return 0; /* End the task */
}


static int PauseTheExecution(void* _pauseObject)
{
    size_t tasksToFinishAfterPause;

    if(((PauseObject*)_pauseObject)->m_currentExecutedTimes == ((PauseObject*)_pauseObject)->m_totalTimesToExecuteTillPause)
    {
        tasksToFinishAfterPause = TasksExecutorPause(((PauseObject*)_pauseObject)->m_executorToPause);
        printf("Tasks still pending after pause: %ld\n", tasksToFinishAfterPause);

        return 0; /* Stop */
    }

    ((PauseObject*)_pauseObject)->m_currentExecutedTimes++;
    return 1; /* Continue */
}


static void InitIntArrayTaskObject(IntArrayTaskObject* _obj)
{
    InitIntArray(&_obj->m_array, LENGTH);
    _obj->m_sizeOfArray = LENGTH;
    _obj->m_sum = 0;
    _obj->m_currentExecutedTimes = 0;
    _obj->m_totalTimesToExecute = EXECUTION_TIMES;
}

static void InitPauseObject(PauseObject* _obj)
{
    _obj->m_currentExecutedTimes = 0;
    _obj->m_totalTimesToExecuteTillPause = 5;
    _obj->m_executorToPause = NULL;
}

static void Test_General_TasksExecutor(void)
{
    IntArrayTaskObject obj, obj2, obj3;
    PauseObject pauseObj;
    TasksExecutor* exec = TasksExecutorCreate("Exec", CLOCK_REALTIME);
    if(!exec)
    {
        printf("Failed to create TasksExecutor...\n");
        return;
    }

    InitIntArrayTaskObject(&obj);
    InitIntArrayTaskObject(&obj2);
    InitIntArrayTaskObject(&obj3);
    InitPauseObject(&pauseObj);
    pauseObj.m_executorToPause = exec;

    if(!obj.m_array || !obj2.m_array || !obj3.m_array)
    {
        printf("Failed to allocate memory...\n");
        return;
    }

    TasksExecutorAdd(exec, &PrintIntArrayTask, (void*)&obj, 100);
    TasksExecutorAdd(exec, &SumIntArrayTask, (void*)&obj2, 1000);
    TasksExecutorAdd(exec, &PrintEvenNumIntArrayTask, (void*)&obj3, 10000);
    TasksExecutorAdd(exec, &PauseTheExecution, (void*)&pauseObj, 2500);

    TasksExecutorRun(exec);

    /* Pause */

    TasksExecutorAdd(exec, &PauseTheExecution, (void*)&pauseObj, 5000);

    TasksExecutorRun(exec);

    /* Another pause */

    TasksExecutorAdd(exec, &SumIntArrayTask, (void*)&obj2, 100); /* should be executed only one single time */
    TasksExecutorAdd(exec, &PrintIntArrayTask, (void*)&obj, 300); /* should be executed only one single time */

    TasksExecutorRun(exec); /* Finish the tasks */

    printf("All tasks had finished successfully!\n");

    TasksExecutorDestroy(&exec);
}


