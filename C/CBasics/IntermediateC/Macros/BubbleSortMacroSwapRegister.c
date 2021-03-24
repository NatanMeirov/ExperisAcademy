#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define BIG_LENGTH 50000
#define SWAP(a, b, type)\
{                       \
	type temp = a;      \
	a = b;              \
	b = temp;           \
}

int BubbleSort(int* _array, int _sizeOfArray);
void Swap(int* _ptrToFirstNum, int* _ptrToSecondNum);
int MacroBubbleSort(int* _array, int _sizeOfArray);
int BubbleSortRegister(int* _array, int _sizeOfArray);
int MacroBubbleSortRegister(int* _array, int _sizeOfArray);


int main(void)
{
	clock_t begin, end;
	int i, num;
	int array1[BIG_LENGTH];
	int array2[BIG_LENGTH];
	int array3[BIG_LENGTH];
	int array4[BIG_LENGTH];
	
	srand(time(NULL));	

	for(i = 0; i < BIG_LENGTH; i++)
	{
		num = rand();
		array1[i] = num;
		array2[i] = num;
		array3[i] = num;
		array4[i] = num;
	}

	/* Swap Function: */
	begin = clock();
	BubbleSort(array1, BIG_LENGTH);
	end = clock();
	printf("BubbleSort with Swap Function: %f seconds\n", ((double)end - begin) / CLOCKS_PER_SEC);


	/* SWAP Macro: */
	begin = clock();
	MacroBubbleSort(array2, BIG_LENGTH);
	end = clock();
	printf("BubbleSort with SWAP Macro: %f seconds\n", ((double)end - begin) / CLOCKS_PER_SEC);

	/* Swap Function Register: */
	begin = clock();
	BubbleSortRegister(array3, BIG_LENGTH);
	end = clock();
	printf("BubbleSort with Swap Function and Register: %f seconds\n", ((double)end - begin) / CLOCKS_PER_SEC);


	/* SWAP Macro Register: */
	begin = clock();
	MacroBubbleSortRegister(array4, BIG_LENGTH);
	end = clock();
	printf("BubbleSort with SWAP Macro and Register: %f seconds\n", ((double)end - begin) / CLOCKS_PER_SEC);

	return 0;
}


int MacroBubbleSort(int* _array, int _sizeOfArray)
{
	int i, j, arrayChangedFlag = 0, totalIterations = 0;
	
	for(i = 0; i < _sizeOfArray; i++)
	{
		for(j = 0; j < _sizeOfArray - 1 - totalIterations; j++)
		{
			if(_array[j] > _array[j + 1])
			{
				SWAP(_array[j], _array[j + 1], int);
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
}



int BubbleSort(int* _array, int _sizeOfArray)
{
	int i, j, arrayChangedFlag = 0, totalIterations = 0;
	
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
}


void Swap(int* _ptrToFirstNum, int* _ptrToSecondNum)
{
	int temp = *_ptrToFirstNum;
	*_ptrToFirstNum = *_ptrToSecondNum;
	*_ptrToSecondNum = temp;
}

/* With register addition: */


int MacroBubbleSortRegister(int* _array, int _sizeOfArray)
{
	int arrayChangedFlag = 0, totalIterations = 0;
	register int i, j;

	for(i = 0; i < _sizeOfArray; i++)
	{
		for(j = 0; j < _sizeOfArray - 1 - totalIterations; j++)
		{
			if(_array[j] > _array[j + 1])
			{
				SWAP(_array[j], _array[j + 1], int);
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
}



int BubbleSortRegister(int* _array, int _sizeOfArray)
{
	int arrayChangedFlag = 0, totalIterations = 0;
	register int i, j;

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
}