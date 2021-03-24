#include <stdio.h>
#include "Strings_Exercise.h"

#define INT_MAX_DIGIT_COUNT 19 /* 64 bit */


int main(void)
{
	char string[] = "Hello";
	char str[] = "ABCDEFGHI";
	char inner[] = "HI";
	int location;
	
	StringsSqueeze(string, "l");	
	printf("%s\n", string);
	
	FindInnerStringLocation(str, inner, &location);
	printf("%d\n", location);	
	
	
	/*
	char str[] = "Natan Meirov";
	char signedNumberStr[] = "   -01240";
	char regNum[] = "11";
	char zero0[] = "0";
	char unsignedNum[] = "+22";
	char invalidNumberStr[] = "a6";
	char anotherInvalidNumberStr[] = "++4";
	char buffer[INT_MAX_DIGIT_COUNT + 2];
	int tempNumber;
	int someNumber = 1244634;
	int someSignedNumber = -185433333;
	int zero = 0;
		
	
	printf("%s", str);
	putchar('\n');
	ReverseString(str);
	printf("%s", str);
	putchar('\n');
	
	printf("--------------\n");
	
	AtoI(signedNumberStr, &tempNumber);
	printf("%d", tempNumber);
	putchar('\n');
	AtoI(invalidNumberStr, &tempNumber);
	printf("%d", tempNumber);
	putchar('\n');
	AtoI(anotherInvalidNumberStr, &tempNumber);
	printf("%d", tempNumber);
	putchar('\n');
	AtoI(unsignedNum, &tempNumber);
	printf("%d", tempNumber);
	putchar('\n');
	AtoI(zero0, &tempNumber);
	printf("%d", tempNumber);
	putchar('\n');
	AtoI(regNum, &tempNumber);
	printf("%d", tempNumber);
	putchar('\n');
	
	printf("--------------\n");
	 
	ItoA(someNumber, buffer);
	printf("%s", buffer);
	putchar('\n');
	ItoA(someSignedNumber, buffer);
	printf("%s", buffer);
	putchar('\n');	
	ItoA(zero, buffer);
	printf("%s", buffer);
	putchar('\n');
	*/
	
	return 0;
}
