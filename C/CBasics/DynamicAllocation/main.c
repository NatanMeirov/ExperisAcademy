/* Libraries: */
#include <stdio.h>
#include <stdlib.h>
#include "Dynamic_Allocation_Int_Array_Management_Library.h"

#define LENGTH 3

void PrintArray(int* _array, int _sizeOfArray);


int main(void)
{
	int size = LENGTH, arrItems = 0;
	int* dynArr = CreateDynamicIntArray(size);
	int newData = 5, anotherNewData = 199;
	int temp;
	
	if(!dynArr)
	{
		printf("Error has occured while tring to allocate memory...\nExiting program...\n");
		exit(1);
	}
	
	PrintArray(dynArr,size); /* Should print garbage */
	
	InsertToIntDynamicArray(&dynArr, newData, &size, &arrItems, 0);
	InsertToIntDynamicArray(&dynArr, anotherNewData, &size, &arrItems, LENGTH);
	InsertToIntDynamicArray(&dynArr, 1000, &size, &arrItems, 3);
	
	PrintArray(dynArr,size);
	
	RemoveFromIntDynamicArray(dynArr, &temp, &arrItems);
	
	printf("\nRemoved: %d\n", temp);
	
	PrintArray(dynArr,size);
	
	InsertToIntDynamicArray(&dynArr, 1111, &size, &arrItems, 0);
	InsertToIntDynamicArray(&dynArr, -100, &size, &arrItems, 5);
	
	PrintArray(dynArr,size);
	
	DestroyDynamicIntArray(dynArr);

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
