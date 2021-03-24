#include <stdio.h>
#include <stdlib.h>
#include "WC.h"

#define SPACE ' '
#define NEW_LINE '\n'
#define TAB '\t'
#define EOS '\0'

#define BYTES 1024


size_t CountWordsInLine(char* _line);


int WordsCount(char* _textFileName)
{
	FILE* f = NULL;
	size_t linesCount = 0, wordsCount = 0, charactersCount = 0;
	char ch;
	int hasWord = 0;
	char buffer[BYTES];
	
	if(!(f = fopen(_textFileName, "r")))
	{
		printf("Error: file not found...\n");
		return 1; /* Error */
	}

	fseek(f, 0, SEEK_END);
	charactersCount = ftell(f); /* Characters calculation */
	fseek(f, 0, SEEK_SET);
	
	fgets(buffer, BYTES, f);
	
	while(!feof(f))
	{	
		wordsCount += CountWordsInLine(buffer);
		linesCount++;
		fgets(buffer, BYTES, f);
	}
	
	printf("%s: Lines: %ld, Words: %ld, Characters: %ld\n", _textFileName, linesCount, wordsCount, charactersCount);
	
	fclose(f);
	
	return 0;
}


size_t CountWordsInLine(char* _line)
{
	int hasWord = 0; /* A bool flag */
	size_t wordsCount = 0;
	
	while(*_line != EOS)
	{
		if(*_line == SPACE || *_line == TAB || *_line == NEW_LINE)
		{
			if(hasWord)
			{
				wordsCount++;
				hasWord = 0;
			}
			
			_line++;
		}
		else /* Any other character */
		{
			if(!hasWord)
			{
				hasWord = 1;
			}
			
			_line++;
		}
	}

	return wordsCount;
}




