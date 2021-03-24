#include <stdio.h>
#include <stdlib.h>
#include "TAIL.h"

#define BYTES 1024

#define NEW_LINE '\n'


size_t FindStartingLocationOfWantedLine(FILE* _f, size_t _endOfFileCursorPosition, size_t _lines);
void PrintLinesFromFile(FILE* _f);


int ShowTail(char* _textFileName, char* _lines)
{
	FILE* f = NULL;
	long temp = atoi(_lines);
	size_t lines, wantedLocationInFile, endOfFileCursorPosition;
	
	if(temp < 0)
	{
		printf("Error: Negative line number was given...\n");
		return 1; /* Error */
	}
	
	lines = temp;
	
	if(!(f = fopen(_textFileName, "r")))
	{
		printf("Error: file not found...\n");
		return 1; /* Error */
	}

	fseek(f, 0, SEEK_END);
	endOfFileCursorPosition = ftell(f);
	
	wantedLocationInFile = FindStartingLocationOfWantedLine(f, endOfFileCursorPosition, lines);
	
	fseek(f, wantedLocationInFile, SEEK_SET);
	PrintLinesFromFile(f);
	
	fclose(f);

	return 0;
}


size_t FindStartingLocationOfWantedLine(FILE* _f, size_t _endOfFileCursorPosition, size_t _lines)
{
	size_t currentNewLinesSeen = 0;
	size_t cursor = _endOfFileCursorPosition - 2; /* -2: to skip the EOF, and the first '\n' in the end of the file */
	char ch;

	fseek(_f, cursor, SEEK_SET);
	
	ch = fgetc(_f);
		
	while(currentNewLinesSeen < _lines)
	{
		if(ch == NEW_LINE)
		{
			currentNewLinesSeen++;
			
			if(currentNewLinesSeen == _lines)
			{
				cursor++;  /* Stopped in '\n' - step +1 step forward */
				fseek(_f, cursor, SEEK_SET);
				break;
			}
		}
		
		cursor--;	
		fseek(_f, cursor, SEEK_SET);
		
		if(cursor == 0)
		{
			break;
		}
		
		ch = fgetc(_f);
	}
	
	return ftell(_f);
}


void PrintLinesFromFile(FILE* _f)
{
	char buffer[BYTES];
	
	fgets(buffer, BYTES, _f);
	while(!feof(_f))
	{
		printf("%s", buffer);
		fgets(buffer, BYTES, _f);
	}
}



