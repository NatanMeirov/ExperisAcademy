#include <stdio.h>
#include "Triangles.h"

int getNumberFromUser(void);
char getCharacterToPrintFromUser(void);


int main(void)
{
	char c = getCharacterToPrintFromUser();
	int sizeOfTriangle = getNumberFromUser();
	putchar('\n');
	
	PrintLeftTriangle(c, sizeOfTriangle);
	putchar('\n');
	
	PrintReversedLeftTriangle(c, sizeOfTriangle);
	putchar('\n');
	
	PrintFullLeftTriangle(c, sizeOfTriangle);
	putchar('\n');
	
	PrintCenterTriangle(c, sizeOfTriangle);
	putchar('\n');
	
	PrintReversedCenterTriangle(c, sizeOfTriangle);
	putchar('\n');
	
	PrintDiamond(c, sizeOfTriangle);
	putchar('\n');

	return 0;	
}


int getNumberFromUser(void)
{
	int num;
	
	printf("Please enter size (width and height) of the triangle: ");
	scanf("%d", &num);
	
	return num;
}


char getCharacterToPrintFromUser(void)
{
	char c;
	
	printf("Please enter the character to print in the triangle: ");
	c = getchar();
	
	return c;	
}
