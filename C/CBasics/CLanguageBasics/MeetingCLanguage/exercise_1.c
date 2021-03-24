#include <stdio.h>

int main(void)
{
	int num1, num2;
	
	printf("PLease enter the first number: ");
	scanf("%d", &num1);
	
	printf("PLease enter the second number: ");
	scanf("%d", &num2);
	
	printf("The sum of the numbers is: %d\n", num1 + num2);
	printf("The difference of the numbers is: %d\n", num1 - num2);
	printf("The multiplication of the numbers is: %d\n", num1 * num2);
	
	if(num2)
	{
		printf("The Modulu value of the numbers is: %d\n", num1 % num2);
	}
	else
	{
		printf("You CANNOT divide by 0!\n");
	}
	
	return 0;
}
