#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t, srand, rand*/
#include "GenericHeap.h"
#include "../../Vector/GenericVector/GenericVector.h"

#define LENGTH 10
#define TEST_ITEMS_COUNT 15

void Test_BuildHeap(void);
void Test_HeapDestroy(void);
void Test_HeapInsert(void);
void Test_HeapPeekTop(void);
void Test_HeapExtract(void);
void Test_HeapSize(void);

int main(void)
{
    Test_BuildHeap();
    Test_HeapDestroy();
    Test_HeapInsert();
    Test_HeapPeekTop();
    Test_HeapExtract();
    Test_HeapSize();
}


int IntsCompareFunction(const void* _left, const void* _right)
{
    return (*((int*)_left) < *((int*)_right)) ? 1 : 0;
}


int PrintSingleHeapElement(const void *_elem, void * _context)
{
    printf("[%d] ", *((int*)_elem));

    return 1;
}


void Test_BuildHeap(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;
    int items[] = {57, 98, 101, 2, 75, 88, 20, 1111, 777, 87, 101010, 5873, 777, -10, 0};

    for(i = 0; i < TEST_ITEMS_COUNT; i++)
    {
        VectorAppend(v, items + i);
    }


    h = HeapBuild(v, &IntsCompareFunction);
    HeapForEach(h, &PrintSingleHeapElement, NULL);

    HeapDestroy(&h);
    VectorDestroy(&v, NULL);

    printf("\nTest_BuildHeap: Passed!\n");
}

void Test_HeapDestroy(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;
    int items[] = {57, 98, 101, 2, 75, 88, 20, 1111, 777, 87, 101010, 5873, 777, -10, 0};

    for(i = 0; i < TEST_ITEMS_COUNT; i++)
    {
        VectorAppend(v, items + i);
    }


    h = HeapBuild(v, &IntsCompareFunction);
    HeapForEach(h, &PrintSingleHeapElement, NULL);

    HeapDestroy(&h);
    if(!h)
    {
        printf("\nTest_HeapDestroy: Passed!\n");
    }

    VectorDestroy(&v, NULL);
}

void Test_HeapInsert(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;
    int items[] = {57, 98, 101, 2, 75, 88, 20, 1111, 777, 87, 101010, 5873, 777, -10, 0};
    int newItem = 100000000;

    for(i = 0; i < TEST_ITEMS_COUNT; i++)
    {
        VectorAppend(v, items + i);
    }


    h = HeapBuild(v, &IntsCompareFunction);
    HeapForEach(h, &PrintSingleHeapElement, NULL);


    printf("\nAdding %d:\n", newItem);
    HeapInsert(h, &newItem);
    HeapForEach(h, &PrintSingleHeapElement, NULL);

    HeapDestroy(&h);

    VectorDestroy(&v, NULL);

    printf("\nTest_HeapInsert: Passed!\n");
}

void Test_HeapPeekTop(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;
    int items[] = {57, 98, 101, 2, 75, 88, 20, 1111, 777, 87, 101010, 5873, 777, -10, 0};
    int newItem = 999999999;
    int* max = NULL;

    for(i = 0; i < TEST_ITEMS_COUNT; i++)
    {
        VectorAppend(v, items + i);
    }


    h = HeapBuild(v, &IntsCompareFunction);
    HeapForEach(h, &PrintSingleHeapElement, NULL);


    printf("\nAdding %d:\n", newItem);
    HeapInsert(h, &newItem);
    HeapForEach(h, &PrintSingleHeapElement, NULL);

    max = (int*)HeapPeekTop(h);
    if(max)
    {
        printf("\nMax: %d\n", *max);
        printf("\nTest_HeapPeekTop: Passed!\n");
    }
    else
    {
        printf("\nTest_HeapPeekTop: Failed...\n");
    }

    HeapDestroy(&h);
    VectorDestroy(&v, NULL);
}

void Test_HeapExtract(void)
{
    size_t i;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;
    int items[] = {57, 98, 101, 2, 75, 88, 20, 1111, 777, 87, 101010, 5873, 777, -10, 0};
    int newItem = 1234567890;
    int* max = NULL;

    for(i = 0; i < TEST_ITEMS_COUNT; i++)
    {
        VectorAppend(v, items + i);
    }


    h = HeapBuild(v, &IntsCompareFunction);
    HeapForEach(h, &PrintSingleHeapElement, NULL);


    printf("\nAdding %d:\n", newItem);
    HeapInsert(h, &newItem);
    HeapForEach(h, &PrintSingleHeapElement, NULL);

    max = (int*)HeapExtract(h);

    if(max)
    {
        printf("\nMax: %d\n", *max);
        printf("\nTest_HeapExtract: Passed!\n");
        HeapForEach(h, &PrintSingleHeapElement, NULL);
    }
    else
    {
        printf("\nTest_HeapExtract: Failed...\n");
    }

    HeapDestroy(&h);
    VectorDestroy(&v, NULL);
}

void Test_HeapSize(void)
{
    size_t i, vecSize, heapSize, fakeVecSize, fakeHeapSize;
    Vector* v = VectorCreate(LENGTH, LENGTH);
    Heap* h;
    int items[] = {57, 98, 101, 2, 75, 88, 20, 1111, 777, 87, 101010, 5873, 777, -10, 0};

    for(i = 0; i < TEST_ITEMS_COUNT; i++)
    {
        VectorAppend(v, items + i);
    }


    h = HeapBuild(v, &IntsCompareFunction);
    HeapForEach(h, &PrintSingleHeapElement, NULL);;

    vecSize = VectorSize(v);
    heapSize = HeapSize(h);
    fakeVecSize = VectorSize(NULL);
    fakeHeapSize = HeapSize(NULL);

    printf("\nHeap size: %ld\n", heapSize);

    if(vecSize == heapSize && heapSize == TEST_ITEMS_COUNT && fakeVecSize == fakeHeapSize && fakeHeapSize == ((size_t)-1))
    {
        printf("\nTest_Size: Passed!\n");
    }
    else
    {
        printf("\nTest_HeapSize: Failed...\n");
    }

    HeapDestroy(&h);
    VectorDestroy(&v, NULL);
}

