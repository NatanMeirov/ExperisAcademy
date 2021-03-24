#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include "BubbleSortCallbackCompareFunc.h"

#define LENGTH 20

void PrintArray(int* _array, int _sizeOfArray);
int IsBigger(int _first, int _second);
int IsSmaller(int _first, int _second);
int HasMoreDigits(int _first, int _second);
int EvenToRight(int _first, int _second);


int main(void)
{

	int array[] = {123, 32, 56, 333, 8, 0, -34, 12, 55, 526, -123, 66, -7777, 98654, 345, -11, 23, 85, -75, 5};
	printf("Original array:\n");
	PrintArray(array, LENGTH);

	printf("Lower to Bigger array:\n");
	BubbleSort(array, LENGTH, IsBigger);
	PrintArray(array, LENGTH);

	printf("Lower to Bigger array:\n");
	BubbleSort(array, LENGTH, IsSmaller);
	PrintArray(array, LENGTH);

	printf("Higher Digits Count from Left to Right array:\n");
	BubbleSort(array, LENGTH, HasMoreDigits);
	PrintArray(array, LENGTH);
	
	printf("Evens to Right Odds to Left array:\n");
	BubbleSort(array, LENGTH, EvenToRight);
	PrintArray(array, LENGTH);
	
	return 0;
}


int IsBigger(int _first, int _second)
{
	return _first > _second;
}


int IsSmaller(int _first, int _second)
{
	return _first < _second;
}


int HasMoreDigits(int _first, int _second)
{
	size_t counterOfFirst = 0, counterOfSecond = 0;
	
	if(_first == 0)
	{
		counterOfFirst++;
	}
	
	if(_second == 0)
	{
		counterOfSecond++;
	}
	
	if(_first < 0)
	{
		_first *= -1;
	}
	
	if(_second < 0)
	{
		_second *= -1;
	}
	
	while(_first)
	{
		_first /= 10;
		counterOfFirst++;
	}
	
	while(_second)
	{
		_second /= 10;
		counterOfSecond++;
	}
	
	return counterOfFirst > counterOfSecond;
}


int EvenToRight(int _first, int _second)
{
	return ((_first % 2 == 0) && (_second % 2 != 0));
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



