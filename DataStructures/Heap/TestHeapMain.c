#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t, srand, rand*/
#include "Heap.h"
#include "Vector.h"
#include "ADTDefs.h"
#include <time.h> /* time */

#define LENGTH 10
#define MAX_VALUE 100

void Test_BuildHeap(void);
void Test_HeapDestroy(void);
void Test_HeapInsert(void);
void Test_HeapMax(void);
void Test_HeapExtractMax(void);
void Test_HeapGetItemsCount(void);

int main(void)
{
    Test_BuildHeap();
    Test_HeapDestroy();
    Test_HeapInsert();
    Test_HeapMax();
    Test_HeapExtractMax();
    Test_HeapGetItemsCount();
}


void Test_BuildHeap(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;

    srand(time(NULL));
    for(i = 0; i < LENGTH; i++)
    {
        VectorAdd(v, rand() % MAX_VALUE);
    }
    VectorAdd(v, MAX_VALUE);

    VectorPrint(v);

    h = HeapBuild(v);
    HeapPrint(h);

    HeapDestroy(h);
    VectorDestroy(v);

    printf("Test_BuildHeap: Passed!\n");
}

void Test_HeapDestroy(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;

    srand(time(NULL));
    for(i = 0; i < LENGTH; i++)
    {
        VectorAdd(v, rand() % MAX_VALUE);
    }
    VectorAdd(v, MAX_VALUE);

    VectorPrint(v);

    h = HeapBuild(v);
    HeapPrint(h);

    HeapDestroy(h);
    HeapPrint(h);
    HeapDestroy(h);
    VectorDestroy(v);

    printf("Test_HeapDestroy: Passed!\n");
}

void Test_HeapInsert(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;

    srand(time(NULL));
    for(i = 0; i < LENGTH; i++)
    {
        VectorAdd(v, rand() % MAX_VALUE);
    }

    VectorPrint(v);

    h = HeapBuild(v);
    HeapPrint(h);

    printf("\nAdding %d:", MAX_VALUE);
    HeapInsert(h, MAX_VALUE);
    HeapPrint(h);

    HeapDestroy(h);
    VectorDestroy(v);

    printf("Test_HeapInsert: Passed!\n");
}

void Test_HeapMax(void)
{
    int max;
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;

    srand(time(NULL));
    for(i = 0; i < LENGTH; i++)
    {
        VectorAdd(v, rand() % MAX_VALUE);
    }

    VectorPrint(v);

    h = HeapBuild(v);
    HeapPrint(h);

    printf("\nAdding %d:", MAX_VALUE);
    HeapInsert(h, MAX_VALUE);
    HeapPrint(h);

    HeapMax(h, &max);
    printf("Max: %d\n", max);

    HeapDestroy(h);
    VectorDestroy(v);

    printf("Test_HeapMax: Passed!\n");
}

void Test_HeapExtractMax(void)
{
    int max;
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;

    srand(time(NULL));
    for(i = 0; i < LENGTH; i++)
    {
        VectorAdd(v, rand() % MAX_VALUE);
    }

    VectorPrint(v);

    h = HeapBuild(v);
    HeapPrint(h);

    printf("\nAdding %d:", MAX_VALUE);
    HeapInsert(h, MAX_VALUE);
    HeapPrint(h);

    HeapExtractMax(h, &max);
    printf("Max: %d\n", max);
    HeapPrint(h);

    HeapDestroy(h);
    VectorDestroy(v);

    printf("Test_HeapExtractMax: Passed!\n");
}

void Test_HeapGetItemsCount(void)
{
    size_t i, vecSize, heapSize;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;

    srand(time(NULL));
    for(i = 0; i < MAX_VALUE; i++)
    {
        VectorAdd(v, rand() % MAX_VALUE);
    }
    VectorAdd(v, MAX_VALUE);

    VectorPrint(v);

    h = HeapBuild(v);
    HeapPrint(h);

    VectorGetItemsNumber(v, &vecSize);
    heapSize = HeapGetItemsCount(h);

    printf("Heap size: %ld\n", heapSize);

    if(vecSize == heapSize && heapSize == MAX_VALUE + 1)
    {
        printf("Test_HeapGetItemsCount: Passed!\n");
    }
    else
    {
        printf("Test_HeapGetItemsCount: Failed...\n");
    }

    HeapDestroy(h);
    VectorDestroy(v);
}