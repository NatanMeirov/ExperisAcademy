#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 100000
#define TIMES 20000

int RegularSearch(int* _array, size_t _sizeOfArray, int _numToSearch)
{
	size_t i;
	for(i = 0; i < _sizeOfArray; i++)
	{
		if(_array[i] == _numToSearch)
		{
			return 1; /* Found */
		}
	}

	return 0; /* Not Found */
}


int ImprovedRegularSearch(int* _array, size_t _sizeOfArray, int _numToSearch)
{
	size_t i;
	int lastArrayItem;
	
	if(_array[_sizeOfArray - 1] == _numToSearch)
	{
		return 1;
	}
	
	lastArrayItem = _array[_sizeOfArray - 1];
	_array[_sizeOfArray - 1] = _numToSearch; /* Now the number will must be in the array - we doesn't need to check if i < _sizeOfArray */
	
	
	for(i = 0; /* condition not needed! */ ;i++)
	{
		if(_array[i] == _numToSearch)
		{
			break;
		}
	}

	_array[_sizeOfArray - 1] = lastArrayItem;

	if(i == _sizeOfArray - 1)
	{
		return 0; /* Reached the last index => Not Found */
	}
	
	return 1; /* Found */
}


int main(void)
{
	int temp;
    clock_t start, end;
    int arr[LENGTH];
    size_t i;

    srand(time(NULL));
    
    for(i = 0; i < LENGTH; i++)
    {
        arr[i] = rand() % LENGTH;
    }


    /* Regular Search: */
    start = clock();
    for(i = 0; i < TIMES; i++)
    {
    	RegularSearch(arr, LENGTH, i);
    }
    end = clock();
    printf("RegularSearch %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);
    
    
    /* Improved Regular Search: */
    start = clock();
    for(i = 0; i < TIMES; i++)
    {
    	ImprovedRegularSearch(arr, LENGTH, i);
    }
    end = clock();
    printf("ImprovedRegularSearch %f seconds\n", ((double)end - start) / CLOCKS_PER_SEC);

	return 0;
}


