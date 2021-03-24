#include <stdio.h>
#include "Triangles.h"

#define MIN_TRIANGLE_SIZE 0
#define MAX_TRIANGLE_SIZE 71
#define MIN_CHAR_ASCII_VALUE '!'
#define MAX_CHAR_ASCII_VALUE '~'

#define OK 0
#define WRONG_CHARACTER 1
#define WRONG_SIZE 2


int CheckParams(char _c, int _sizeOfTriangle);
void PrintLine(char _c, int _sizeOfLine);
void PrintLineWithSwitchingChars(char _firstChar, char _secondChar, int _sizeOfLine);


int PrintLeftTriangle(char _c, int _sizeOfTriangle)
{
	int i, statusCode;
	
	if((statusCode = CheckParams(_c, _sizeOfTriangle)) != OK)
	{
		return statusCode;
	}
	
	for(i = 0; i < _sizeOfTriangle; i++)
	{
		PrintLine(_c, i + 1);
		putchar('\n');
	}
	
	return OK;
}


int PrintReversedLeftTriangle(char _c, int _sizeOfTriangle)
{
	int i, statusCode;
	
	if((statusCode = CheckParams(_c, _sizeOfTriangle)) != OK)
	{
		return statusCode;
	}
	
	for(i = _sizeOfTriangle; i > 0; i--)
	{
		PrintLine(_c, i);
		putchar('\n');
	}
	
	return OK;	
}


int PrintFullLeftTriangle(char _c, int _sizeOfTriangle)
{
	int statusCode1, statusCode2, statusCodeToReturn;
	
	if(((statusCode1 = CheckParams(_c, _sizeOfTriangle)) != OK) || (statusCode2 = CheckParams(_c, _sizeOfTriangle - 1)) != OK)
	{
		statusCodeToReturn = (statusCode1 != OK) ? statusCode1 : statusCode2;
		return statusCodeToReturn;
	}
	
	PrintLeftTriangle(_c, _sizeOfTriangle);
	PrintReversedLeftTriangle(_c, _sizeOfTriangle - 1);
	
	return OK;
}


int PrintCenterTriangle(char _c, int _sizeOfTriangle)
{
	int i, statusCode, remainingCharsToPrintInLine, blanksToPrintTillSwitch = _sizeOfTriangle, totalCharsToPrintInLine = _sizeOfTriangle;
	
	if((statusCode = CheckParams(_c, _sizeOfTriangle)) != OK)
	{
		return statusCode;
	}
	
	for(i = 0; i < _sizeOfTriangle; i++)
	{
		PrintLine(' ', blanksToPrintTillSwitch - 1);
		
		remainingCharsToPrintInLine = totalCharsToPrintInLine - blanksToPrintTillSwitch + 1;
		
		PrintLineWithSwitchingChars(_c, ' ', remainingCharsToPrintInLine);
		
		putchar('\n');
		
		blanksToPrintTillSwitch--;
		totalCharsToPrintInLine++;
	}
	
	return OK;
}

int PrintReversedCenterTriangle(char _c, int _sizeOfTriangle)
{
	int i, statusCode, remainingCharsToPrintInLine, blanksToPrintTillSwitch = 0, totalCharsToPrintInLine = _sizeOfTriangle * 2;

	if((statusCode = CheckParams(_c, _sizeOfTriangle)) != OK)
	{
		return statusCode;
	}
	
	for(i = 0; i < _sizeOfTriangle; i++)
	{
		PrintLine(' ', blanksToPrintTillSwitch);
		
		remainingCharsToPrintInLine = totalCharsToPrintInLine - blanksToPrintTillSwitch;
		
		PrintLineWithSwitchingChars(_c, ' ', remainingCharsToPrintInLine);
		
		putchar('\n');
		
		blanksToPrintTillSwitch++;
		totalCharsToPrintInLine--;
	}
	
	return OK;
}


int PrintDiamond(char _c, int _sizeOfTriangle)
{
	int statusCode1, statusCode2, statusCodeToReturn, i, remainingCharsToPrintInLine, blanksToPrintTillSwitch = 1, totalCharsToPrintInLine = _sizeOfTriangle * 2;
	
	if(((statusCode1 = CheckParams(_c, _sizeOfTriangle)) != OK) || (statusCode2 = CheckParams(_c, _sizeOfTriangle + 1)) != OK)
	{
		statusCodeToReturn = (statusCode1 != OK) ? statusCode1 : statusCode2;
		return statusCodeToReturn;
	}
	
	PrintCenterTriangle(_c, _sizeOfTriangle + 1);
	
	for(i = _sizeOfTriangle; i > 0; i--)
	{
		PrintLine(' ', blanksToPrintTillSwitch);
		
		remainingCharsToPrintInLine = totalCharsToPrintInLine - blanksToPrintTillSwitch;
		
		PrintLineWithSwitchingChars(_c, ' ', remainingCharsToPrintInLine);
		
		putchar('\n');
		
		blanksToPrintTillSwitch++;
		totalCharsToPrintInLine--;
	}
	
	return OK;
}


void PrintLine(char _c, int _sizeOfLine)
{
	int i;
	
	for(i = 0; i < _sizeOfLine; i++)
	{
		putchar(_c);
	}
}


void PrintLineWithSwitchingChars(char _firstChar, char _secondChar, int _sizeOfLine)
{
	int i;
	char currentCharToPrint = _firstChar;
	
	for(i = 0; i < _sizeOfLine; i++)
	{	
		putchar(currentCharToPrint);
		currentCharToPrint = (currentCharToPrint == _firstChar) ? _secondChar : _firstChar;
	}
}


int CheckParams(char _c, int _sizeOfTriangle)
{
	if(_c < MIN_CHAR_ASCII_VALUE || _c > MAX_CHAR_ASCII_VALUE)
	{
		return WRONG_CHARACTER;
	}
	else if(_sizeOfTriangle < MIN_TRIANGLE_SIZE || _sizeOfTriangle > MAX_TRIANGLE_SIZE)
	{
		return WRONG_SIZE;
	}
	
	return OK;
}






















