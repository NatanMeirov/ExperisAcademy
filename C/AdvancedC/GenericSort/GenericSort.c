/* Generic Bubble Sort Source File */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include "GenericSort.h"

static void MemorySwap(void* _tempPtr, void* _firstElm, void* _secondElm, size_t _bytesToCopy);


int GenericBubbleSort(void* _arrayToSort, size_t _sizeOfArray, size_t _eachElementSizeInBytes, SortCriteria _criteria)
{
	/* Use memcpy, and copy the bytes using a temp var in _eachElementSizeInBytes */
	int arrayChangedFlag = 0, totalIterations = 0;
	size_t i, j;
	void* tempPtr = NULL;

	if(!_arrayToSort)
	{
		return 0;
	}

	tempPtr = malloc(_eachElementSizeInBytes); /* Allocating memory to use in the swap function */
	if(!tempPtr)
	{
		return 0;
	}

	for(i = 0; i < _sizeOfArray; i++)
	{
		for(j = 0; j < _sizeOfArray - 1 - totalIterations; j++)
		{
			if(_criteria((void*)((char*)_arrayToSort + (j * _eachElementSizeInBytes)), (void*)((char*)_arrayToSort + ((j + 1) * _eachElementSizeInBytes))))
			{
				MemorySwap(tempPtr, (char*)_arrayToSort + (j * _eachElementSizeInBytes), (char*)_arrayToSort + ((j + 1) * _eachElementSizeInBytes), _eachElementSizeInBytes);

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

	free(tempPtr);

	return 1;
}


static void MemorySwap(void* _tempPtr, void* _firstElm, void* _secondElm, size_t _bytesToCopy)
{
	memcpy(_tempPtr, _firstElm, _bytesToCopy);
	memcpy(_firstElm, _secondElm, _bytesToCopy);
	memcpy(_secondElm, _tempPtr,_bytesToCopy);
}