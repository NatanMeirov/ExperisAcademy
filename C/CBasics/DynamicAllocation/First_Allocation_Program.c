/* Libraries: */
#include <stdio.h>
#include <stdlib.h>

/* Defines: */
#define N 10

/* Declarations: */
void GetInputFromUser(int* _array, int _sizeOfArray);
void PrintArray(int* _array, int _sizeOfArray);


int main(void)
{
	int i;
	int* dynamicArrayPtr = (int*)malloc(N * sizeof(int));
	
	GetInputFromUser(dynamicArrayPtr, N);
	
	PrintArray(dynamicArrayPtr, N);
	
	free(dynamicArrayPtr);

	return 0;
}


/* Helper Functions */

void GetInputFromUser(int* _dynamicArrayPtr, int _sizeOfArray)
{
	int i;
	
	printf("Please enter 10 numbers:\n");
	
	for(i = 0; i < N; i++)
	{
		printf("Number %d: ", i + 1);
		scanf("%d", _dynamicArrayPtr + i); /* Same as: _dynamicArrayPtr[i] */
	}
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
