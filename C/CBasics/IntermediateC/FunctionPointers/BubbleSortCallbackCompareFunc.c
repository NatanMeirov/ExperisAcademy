#include <stdlib.h> /* size_t */
#include "BubbleSortCallbackCompareFunc.h"

static void Swap(int* _ptrToFirstNum, int* _ptrToSecondNum);


int BubbleSort(int* _array, int _sizeOfArray, callbackCompareFunction _callback)
{
	int i, j, arrayChangedFlag = 0, totalIterations = 0;
	
	if(!_array)
	{
		return 0;
	}
	
	for(i = 0; i < _sizeOfArray; i++)
	{
		for(j = 0; j < _sizeOfArray - 1 - totalIterations; j++)
		{
			if((*_callback)(_array[j], _array[j + 1]))
			{
				Swap(&_array[j], &_array[j + 1]);
				arrayChangedFlag = 1;
			}
		}
		
		if(!arrayChangedFlag)
		{
		/* Array already sorted */
			break;
		}
		
		arrayChangedFlag = 0;
		totalIterations++;
	}
	
	return 1;
}


static void Swap(int* _ptrToFirstNum, int* _ptrToSecondNum) /* static - available only in this module */
{
	int temp = *_ptrToFirstNum;
	*_ptrToFirstNum = *_ptrToSecondNum;
	*_ptrToSecondNum = temp;
}
