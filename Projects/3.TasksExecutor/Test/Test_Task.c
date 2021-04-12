#define _XOPEN_SOURCE 600 /* to define clockid_t */

#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include <time.h> /* clockid_t */
#include "../Include/Task.h"

#define LENGTH 10

static void InitIntArray(int* _arr, size_t _size);
static int PrintTask(void* _printerObject);
static int PrintTask2(void* _unUsedContext);
static void Test_General_Task(void);


typedef struct IntArrayPrinterObject
{
    int* arrayToPrint;
    size_t sizeOfArray;
} IntArrayPrinterObject;


int main(void)
{
    Test_General_Task();

    return 0;
}


static void Test_General_Task(void)
{
    Task* task = NULL;
    Task* task2 = NULL;
    IntArrayPrinterObject iapo;
    size_t i;

    iapo.arrayToPrint = malloc(LENGTH * sizeof(int));
    if(!iapo.arrayToPrint)
    {
        printf("Failed to allocate memory...\n");
        return;
    }

    InitIntArray(iapo.arrayToPrint, LENGTH);
    iapo.sizeOfArray = LENGTH;

    task = TaskCreate(&PrintTask, (void*)&iapo, CLOCK_REALTIME, 500);
    task2 = TaskCreate(&PrintTask2, NULL, CLOCK_REALTIME, 5000);

    if(!task || !task2)
    {
        printf("Failed to create Task...\n");
        return;
    }

    for(i = 0; i < LENGTH; i++)
    {
        TaskUpdateNextExecutionTime(task);
        TaskUpdateNextExecutionTime(task2);
    }

    for(i = 0; i < LENGTH; i++)
    {
        TaskExecute(task);
        TaskUpdateNextExecutionTime(task);

        TaskExecute(task2);
        TaskUpdateNextExecutionTime(task2);
    }

    printf("Passed!\n");
}


static void InitIntArray(int* _arr, size_t _size)
{
    size_t i;

    for(i = 0; i < _size; i++)
    {
        _arr[i] = i;
    }
}


static int PrintTask(void* _printerObject)
{
    size_t i;

    putchar('\n');
    putchar('{');
    putchar(' ');


    for(i = 0; i < ((IntArrayPrinterObject*)_printerObject)->sizeOfArray; i++)
    {
        printf("%d ", ((IntArrayPrinterObject*)_printerObject)->arrayToPrint[i]);
    }


    putchar('}');
    putchar('\n');

    return 1; /* To force running the task again */
}


static int PrintTask2(void* _unUsedContext)
{
    size_t i;
    (void)_unUsedContext;

    for(i = 0; i < LENGTH; i++)
    {
        printf("%ld ", i*i);
    }

    putchar('\n');

    return 1; /* To force running the task again */
}