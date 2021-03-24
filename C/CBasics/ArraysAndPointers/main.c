#include <stdio.h>
#include "ArraysAndPointers_Exercise.h"

#define LENGTH 15
#define TEMP_INDEX 10
#define NOT_IN_ARRAY_NUMBER 2

void PrintArray(int* _array, int _sizeOfArray);


int main(void)
{
	int array[LENGTH] = {22, 1, -3, 0, 66, 3000, 1096, 22222, 5, 4, -3, -3, 66, 14, 777};
	int mostCommonNum, evenNums, indexFound;
	
	FindMostFrequentNumInArray(array, LENGTH, &mostCommonNum);
	printf("Most common number in the array, is: %d", mostCommonNum);
	PrintArray(array, LENGTH);
	putchar('\n');
	
	evenNums = ReorderArrayAsEvensToLeftOddsToRight(array, LENGTH);
	printf("Even number count in the array, is: %d", evenNums);
	PrintArray(array, LENGTH);
		
	printf("Bubble Sorting...");
	BubbleSort(array, LENGTH);
	PrintArray(array, LENGTH);
	
	printf("Binary Searching for %d:\n", array[TEMP_INDEX]);
	indexFound = BinarySearch(array, LENGTH, array[TEMP_INDEX]);
	printf("Index: %d\n", indexFound);
	
	printf("Binary Searching for %d:\n", NOT_IN_ARRAY_NUMBER);
	indexFound = BinarySearch(array, LENGTH, NOT_IN_ARRAY_NUMBER);
	printf("Index: %d\n", indexFound);

	return 0;
}


void PrintArray(int* _array, int _sizeOfArray)
{
	int i;
	
	putchar('\n');
	putchar('[');
	for(i = 0; i < _sizeOfArray; i++)
	{
		if(i == _sizeOfArray - 1)
		{
			printf("%d", _array[i]);
		}
		else
		{
			printf("%d, ", _array[i]);
		}
	}
	putchar(']');
	putchar('\n');
}
