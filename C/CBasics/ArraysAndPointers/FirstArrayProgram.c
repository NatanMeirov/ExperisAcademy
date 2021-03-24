#include <stdio.h>

#define LENGTH 10


int main(void)
{
	int array[LENGTH] = {1, 23, 56, 43, 11, 87, 90, 10, 20, 22};
	int i, sum = 0;
	
	for(i = 0; i < LENGTH; i++)
	{
		sum += array[i];	
	}
	
	printf("The sum of the array, is: %d. \nThe average value of the array, is: %d\n.", sum, (sum / LENGTH));
	
	return 0;
}
