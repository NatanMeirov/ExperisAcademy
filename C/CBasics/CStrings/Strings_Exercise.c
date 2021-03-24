#include "Strings_Exercise.h"
#include <string.h>
#include <stdlib.h>

#define BLANK ' '
#define PLUS_SIGN '+'
#define MINUS_SIGN '-'

int ValidateCharPtr(char* _str);
int ValidateIntPtr(int* _num);
void CharSwap(char* _firstChar, char* _secondChar);
int SkipLeadingBlanks(char* _str);
int GetCountOfDigits(int _number);
int CheckSign(char* _str, int currentIndexToCheck); /* Returns 1 if '-' (minus sign), 0 if '+' (plus sign), -1 else */
int CalculateNumber(char* _str, int currentIndex, int signedNumberFlag);
int CheckCharMatching(char _charToCheck, char* _str);
void RemoveCharFromString(char* _str, int currentIndex, int currentStrLength);
void InitializeCurrentCharBuffer(char* charSlidingWindowBuffer, int currentIndex, int limitIndex, char* _str);
/* int GetStringLength(char* _str); */


int ReverseString(char* _str)
{
	int statusCode, i;
	char* strHead;
	char* strTail;
	
	if((statusCode = ValidateCharPtr(_str)) != OK)
	{
		return statusCode;
	}
	
	strHead = _str;
	strTail = _str + strlen(_str) - 1;
	
	while(strHead < strTail)
	{
		CharSwap(strHead++, strTail--);
	}
	
	return OK;
}


int AtoI(char* _str, int* _convertedNumber)
{
	int statusCode1, statusCode2, i, convertedNumber = 0, startingIndex = 0, convertStartedFlag = 0, signedNumberFlag = 0;
	
	if((statusCode1 = ValidateCharPtr(_str)) != OK || (statusCode2 = ValidateIntPtr(_convertedNumber)) != OK)
	{
		return (statusCode1 != OK) ? (statusCode1) : (statusCode2);
	}

	startingIndex = SkipLeadingBlanks(_str);

	signedNumberFlag = CheckSign(_str, startingIndex);
		
	if(signedNumberFlag == 1 || signedNumberFlag == 0)
	{
		startingIndex++;
	}
	else
	{
		signedNumberFlag = 0; /* No sign => flag = false (had -1 before) */
	}
	
	*_convertedNumber = CalculateNumber(_str, startingIndex, signedNumberFlag);
	
	return OK;
}


int ItoA(int _num, char* _buffer)
{
	int statusCode, i, endingIndex, startingIndex = 0;
	
	if((statusCode = ValidateCharPtr(_buffer)) != OK)
	{
		return statusCode;
	}
	
	if(_num < 0)
	{
		_buffer[startingIndex++] = MINUS_SIGN;
		_num *= -1; /* Absolute value */
	}
	
	endingIndex = GetCountOfDigits(_num);
	
	for(i = endingIndex - 1 + startingIndex /* offseting from the start */; i >= startingIndex; i--)
	{
		_buffer[i] = (_num % 10) + '0';
		_num /= 10;
	}
	
	_buffer[endingIndex + startingIndex /* offseting from the start */] = '\0';
	
	return OK;
}


int StringsSqueeze(char* _str1, char* _str2)
{
	int statusCode, i, str1Length;

	if((statusCode = ValidateCharPtr(_str1)) != OK || (statusCode = ValidateCharPtr(_str2)) != OK)
	{
		return statusCode;
	}
	
	str1Length = strlen(_str1);
	
	for(i = 0; i < str1Length; i++)
	{
		if(CheckCharMatching(_str1[i], _str2))
		{
			RemoveCharFromString(_str1, i, str1Length);
			str1Length--;
			i--; /* There is a new character in this index - check again */
		}
	}
	
	return OK;
}


int FindInnerStringLocation(char* _str, char* _innerStr, int* _locationIndex)
{
	int statusCode, i, outterStrLength, innerStrLength, limitIndex;
	char* charSlidingWindowBuffer = NULL;
	
	if((statusCode = ValidateCharPtr(_str)) != OK || (statusCode = ValidateCharPtr(_innerStr)) != OK || (statusCode = ValidateIntPtr(_locationIndex)))
	{
		return statusCode;
	}

	outterStrLength = strlen(_str);
	innerStrLength = strlen(_innerStr);
	charSlidingWindowBuffer = calloc((innerStrLength + 1), sizeof(char));
	
		
	for(i = 0; i < outterStrLength; i++)
	{
		limitIndex = i + innerStrLength;
		
		if(limitIndex > outterStrLength)
		{
			/* Not Found */
			*_locationIndex = -1;
			break;
		}
		
		InitializeCurrentCharBuffer(charSlidingWindowBuffer, i, limitIndex, _str);
		
		if(CompaireStrings(charSlidingWindowBuffer, _innerStr))
		{
			/* Found */
			*_locationIndex = i;
			break;
		}
	}
	
	return OK;
}


/* Valitation Functions: */

int ValidateCharPtr(char* _str)
{
	if(!_str)
	{
		return NULL_PTR_ERROR;
	}
	
	return OK;
}

int ValidateIntPtr(int* _numPtr)
{
	if(!_numPtr)
	{
		return NULL_PTR_ERROR;
	}
	
	return OK;
}


/* Helper Functions: */

void CharSwap(char* _firstChar, char* _secondChar)
{
	char temp = *_firstChar;
	*_firstChar = *_secondChar;
	*_secondChar = temp;
}


int SkipLeadingBlanks(char* _str)
{
	int i, blanksCounter = 0; /* Saw at least 1 blank character already */
	
	for(i = 0; _str[i] != '\0'; i++)
	{
		if(_str[i] == BLANK)
		{
			blanksCounter++;
		}
		else /* Already skipped all leading blanks */
		{
			break;
		}
	}
	
	return blanksCounter;
}


int CheckSign(char* _str, int currentIndexToCheck)
{
	if(_str[currentIndexToCheck] == MINUS_SIGN)
	{
		return 1;
	}
	else if(_str[currentIndexToCheck] == PLUS_SIGN)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


int CalculateNumber(char* _str, int currentIndex, int signedNumberFlag)
{
	int i, convertedNumber = 0;
	
	for(i = currentIndex; _str[i] != '\0'; i++)
	{
		if('0' <= _str[i] && _str[i] <= '9')
		{			
			convertedNumber *= 10;
			convertedNumber += (_str[i] - '0');			
		}
		else if(_str[i] < '0' || _str[i] > '9')
		{
			break;
		}
	}
	
	if(signedNumberFlag)
	{
		convertedNumber *= -1;
	}

	return convertedNumber;
}


int GetCountOfDigits(int _num)
{
	int counter = 0;
	
	if(_num == 0)
	{
		return 1; /* counter = 1 */
	}
	
	if(_num < 0)
	{
		_num *= -1; /* Absolute value */
	}
	
	while(_num > 0)
	{
		_num /= 10;
		counter++;
	}
	
	return counter;
}


int CheckCharMatching(char _charToCheck, char* _str)
{
	int i;
	
	for(i = 0; _str[i] != '\0'; i++)
	{
		if(_str[i] == _charToCheck)
		{
			return 1; /* true */
		}
	}
	
	return 0;
}


void RemoveCharFromString(char* _str, int currentIndex, int currentStrLength)
{
	int i;
	
	for(i = currentIndex; i < currentStrLength - 1; i++)
	{
		CharSwap(&_str[i], &_str[i + 1]); /* "Bubbles" the char to remove - up */
	}
	
	_str[currentStrLength - 1] = '\0';
}


void InitializeCurrentCharBuffer(char* charSlidingWindowBuffer, int currentIndex, int limitIndex, char* _str)
{
	int i;
	
	for(i = currentIndex; i < limitIndex; i++)
	{
		charSlidingWindowBuffer[i - currentIndex] = _str[i];
	}
	
	charSlidingWindowBuffer[limitIndex] = '\0';
}


int CompaireStrings(char* charSlidingWindowBuffer, char* _innerStr)
{
	return (strcmp(charSlidingWindowBuffer, _innerStr) == 0) ? 1 /* true */ : 0 /* false */;
}


/*
int GetStringLength(char* _str)
{
	int i, counter = 0;
	
	for(i = 0; _str[i] != '\0'; i++)
	{
		counter++;
	}

	return counter;
}
*/
