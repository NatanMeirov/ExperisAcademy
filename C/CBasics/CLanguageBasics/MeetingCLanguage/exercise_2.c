#include <stdio.h>

int main(void)
{
	int n, i;
	int sum = 1;
	
	printf("Please enter a number N, to find its factorial: ");
	scanf("%d", &n);
	
	for(i = 1; i <= n; i++)
	{
		sum *= i;
	}
	
	printf("The factorial of N=%d, is: %d\n", n, sum);
	
	return 0;
}
