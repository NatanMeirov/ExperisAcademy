#include "ArraysAndPointers_Exercise.h"

#define NULL 0

int ValidateParams(int* _array, int _sizeOfArray);
void Swap(int* _firstNum, int* _secondNum);
int CountFrequencyOfCurrentNum(int* _array, int _sizeOfArray, int _currentNumIndex);
void SwapInRange(int* _array, int _start, int _end, int _reversedOrder); /* Reversed order - a flag */
int* FindFirstMatchedKeyIndex(int* _arrayLocation, int* _currentLocation);


int FindMostFrequentNumInArray(int* _array, int _sizeOfArray, int* _mostFrequentNum)
{
	int statusCode, i, currentMostFrequentNum = _array[0], currentNumberFrequencyCounter = 0, previousNumberFrequencyCounter = 0;

	if((statusCode = ValidateParams(_array, _sizeOfArray)) != OK)
	{
		return statusCode;
	}
	
	if(!_mostFrequentNum)
	{
		return NULL_PTR_ERROR;
	}
	
	for(i = 0; i < _sizeOfArray; i++)
	{
		currentNumberFrequencyCounter = CountFrequencyOfCurrentNum(_array, _sizeOfArray, i); /* CurrentNumber = _array[i] - No need to pass it to the function */
		
		if(currentNumberFrequencyCounter > previousNumberFrequencyCounter)
		{
			currentMostFrequentNum = _array[i];
			previousNumberFrequencyCounter = currentNumberFrequencyCounter;
		}
	}
	
	*_mostFrequentNum = currentMostFrequentNum;
	return OK;
}


int ReorderArrayAsEvensToLeftOddsToRight(int* _array, int _sizeOfArray)
{
	int statusCode, i, j, evenNumCounter = 0, reverseModeFlag = 1, lastEvenNumSawIndex = 0, newEvenNumSawIndex = 0;
	
	if((statusCode = ValidateParams(_array, _sizeOfArray)) != OK)
	{
		return statusCode;
	}
	
	for(i = 0; i < _sizeOfArray; i++)
	{
		if(_array[i] % 2 == 0) /* Even number */ //COUNT EVENS ON A ROW FUNC
		{
			lastEvenNumSawIndex++;
			evenNumCounter++;
		}
		else /* Odd number */
		{
			j = i;
			while(_array[j] % 2 != 0 && j < _sizeOfArray) // COUNT ODDS ON A ROW FUNC
			{
				j++;
			}
			
			/* Saw an even number - stop the loop and continue from here (to "bubble it down" to the left side of the array): */
			newEvenNumSawIndex = j;
			
			SwapInRange(_array, lastEvenNumSawIndex + 1, newEvenNumSawIndex, reverseModeFlag);
			
			evenNumCounter++;
			lastEvenNumSawIndex++;
			i = j + 1;
		}
	}		
	
	return evenNumCounter;
}


int BubbleSort(int* _array, int _sizeOfArray)
{
	int statusCode, i, j, arrayChangedFlag = 0, totalIterations = 0;
	
	if((statusCode = ValidateParams(_array, _sizeOfArray)) != OK)
	{
		return statusCode;
	}
	
	for(i = 0; i < _sizeOfArray; i++)
	{
		for(j = 0; j < _sizeOfArray - 1 - totalIterations; j++)
		{
			if(_array[j] > _array[j + 1])
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
	
	return OK;
}


int BinarySearch(int* _array, int _sizeOfArray, int _key)
{
	int statusCode;
	int* head = _array; /* Points to the first location of the array */
	int* tail = _array + _sizeOfArray - 1; /* Points to the last location of the array */
	int* median = NULL;
	
	if((statusCode = ValidateParams(_array, _sizeOfArray)) != OK)
	{
		return statusCode;
	}
	
	while(head < tail)
	{
		median = head + ((tail - head) / 2); /* The average value between the head and the tail addresses */
		
		if(*median == _key)
		{
			median = FindFirstMatchedKeyIndex(_array, median);
		
			return (median - _array); /* Returning the current index (calculated by the offset from median pointer to the starting location of the array) */ 
		}
		else if(*median > _key)
		{
			tail = median - 1; /* Go to the left side of the array */
		}
		else /* if(*median < _key) */
		{
			head = median + 1; /* Go to the right side of the array */
		}
	}
	
	return -1; /* Key not found */
}


int ValidateParams(int* _array, int _sizeOfArray)
{
	if(!_array) /* if(_array == NULL) */
	{
		return NULL_PTR_ERROR;
	}
	else if(_sizeOfArray < 1)
	{
		return SIZE_ERROR;
	}
	
	return OK;
}


void Swap(int* _ptrToFirstNum, int* _ptrToSecondNum)
{
	int temp = *_ptrToFirstNum;
	*_ptrToFirstNum = *_ptrToSecondNum;
	*_ptrToSecondNum = temp;
}


int CountFrequencyOfCurrentNum(int* _array, int _sizeOfArray, int _currentNumIndex)
{
	int i, currentNumber = _array[_currentNumIndex], frequencyCounter = 0;

	for(i = _currentNumIndex + 1; i <= _sizeOfArray; i++)
	{
		if(_array[i] == currentNumber)
		{
			frequencyCounter++;
		}
	}
	
	return frequencyCounter;
}


void SwapInRange(int* _array, int _start, int _end, int _reversedOrder)
{
	int i;
	
	if(_reversedOrder)
	{
		for(i = _end; i >= _start; i--)
		{
			Swap(&_array[i], &_array[i - 1]);
		}
	}
	else
	{
		for(i = _start; i <= _end; i++)
		{
			Swap(&_array[i], &_array[i + 1]);
		}
	}
}

int* FindFirstMatchedKeyIndex(int* _arrayLocation, int* _currentLocation)
{
	while((_currentLocation - _arrayLocation) > 0)
	{
		if(*(_currentLocation - 1) == *_currentLocation)
		{
			_currentLocation--;
		}
		else
		{
			break; /* First matched key index found */
		}
	}
	
	return _currentLocation;
}
