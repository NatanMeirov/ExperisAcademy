#include <stdio.h>

int main(void)
{

	int number, palindrome = 0, temp;
	
	printf("Please enter a number to check if it is a palindrome: ");
	scanf("%d", &number);
	temp = number;
	
	while(temp > 0)
	{
		palindrome *= 10;
		palindrome += temp % 10;
		temp /= 10;
	}
	
	(number == palindrome) ? printf("The number %d is a Palindrome.\n", number) : printf("The number %d is NOT a Palindrome.\n", number);
	
	return 0;
	
}
