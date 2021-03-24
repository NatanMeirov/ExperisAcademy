#include <stdio.h> /* printf */
#include <stdlib.h> /* srand, rand, size_t */
#include "ADTDefs.h"
#include "../../Dynamic_Vector/Vector.h"
#include <time.h>
#include "SortingAlgorithms.h"

#define LENGTH 60000
#define RADIX_DIGITS 4


int CheckIfSortSucceed(Vector* _v)
{
    size_t i, sizeOfVector;
    int firstItem, secondItem;

    VectorGetItemsNumber(_v, &sizeOfVector);

    for(i = 0; i < sizeOfVector - 1; i++)
    {
        VectorGet(_v, i, &firstItem);
        VectorGet(_v, i + 1, &secondItem);

        if(firstItem > secondItem)
        {
            return 0;
        }
    }

    return 1;
}


int	main(void)
{
    size_t i, j;
    Vector* v1 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v2 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v3 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v4 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v5 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v6 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v7 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v8 = VectorCreate(LENGTH * 2, LENGTH);
    Vector* v9 = VectorCreate(LENGTH * 2, LENGTH);

    int temp;
    clock_t start, end;

    Vector* vRadix = VectorCreate(LENGTH * 2, LENGTH);

    for(i = 0; i < LENGTH; i++)
    {
        temp = 0;
        for(j = 0; j < RADIX_DIGITS; j++)
        {
            temp *= 10;
            temp += (rand() % 9 + 1); /* To avoid adding 0 to a begin of a new number */
        }
        VectorAdd(vRadix, temp);
    }

    srand(time(NULL));
    for(i = 0; i < LENGTH; i++)
    {
        temp = rand() % LENGTH;
        VectorAdd(v1, temp);
        VectorAdd(v2, temp);
        VectorAdd(v3, temp);
        VectorAdd(v4, temp);
        VectorAdd(v5, temp);
        VectorAdd(v6, temp);
        VectorAdd(v7, temp);
        VectorAdd(v8, temp);
        VectorAdd(v9, temp);
    }


    /* Bubble Sort: */
    start = clock();
    BubbleSort(v1);
    end = clock();
    printf("BubbleSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v1))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Shake Sort: */
    start = clock();
    ShakeSort(v2);
    end = clock();
    printf("ShakeSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v2))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Quick Sort: */
    start = clock();
    QuickSort(v3);
    end = clock();
    printf("QuickSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v3))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Selection Sort: */
    start = clock();
    SelectionSort(v4);
    end = clock();
    printf("SelectionSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v4))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Insertion Sort: */
    start = clock();
    InsertionSort(v5);
    end = clock();
    printf("InsertionSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v5))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Shell Sort: */
    start = clock();
    ShellSort(v6);
    end = clock();
    printf("ShellSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v6))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Merge Sort: */
    start = clock();
    MergeSort(v7);
    end = clock();
    printf("MergeSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v7))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Counting Sort (Not Stable): */
    start = clock();
    CountingSort(v8, 0, LENGTH, 0);
    end = clock();
    printf("CountingSort (Not Stable): %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v8))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Counting Sort (Stable): */
    start = clock();
    CountingSort(v9, 0, LENGTH, 1);
    end = clock();
    printf("CountingSort (Stable): %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(v9))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    /* Radix Sort (Stable): */
    start = clock();
    RadixSort(vRadix, RADIX_DIGITS);
    end = clock();
    printf("RadixSort: %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    if(CheckIfSortSucceed(vRadix))
    {
        printf("Sorted!\n");
    }
    else
    {
        printf("Not Sorted...\n");
    }


    VectorDestroy(v1);
    VectorDestroy(v2);
    VectorDestroy(v3);
    VectorDestroy(v4);
    VectorDestroy(v5);
    VectorDestroy(v6);
    VectorDestroy(v7);
    VectorDestroy(v8);
    VectorDestroy(v9);
    VectorDestroy(vRadix);

    return 0;
}
