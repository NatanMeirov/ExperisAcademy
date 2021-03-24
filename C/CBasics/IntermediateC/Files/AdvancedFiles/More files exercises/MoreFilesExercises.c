#include <stdio.h>
#include <stdlib.h>

#define LOWEST_VALID_CHAR '!'
#define TOTAL_NUM_OF_VALID_CHARS 93
#define BYTES 1024

void MapCharactersInFile(char* fileName);
void MapCharsInSingleLine(int* _counters, char* _line);
void PrintMappedCharacterInFile(int* _counters);


int main(void)
{
	MapCharactersInFile("testtext.txt");
	return 0;
}


void MapCharactersInFile(char* fileName)
{
	FILE* f = NULL;
	int countingArray[TOTAL_NUM_OF_VALID_CHARS] = {0}; /* Valid ASCII chars, from 33 ('!') to 126 ('~') */
	char buffer[BYTES];
	
	if(!(f = fopen(fileName, "r")))
	{
		printf("Error: file not found...");
		exit(1);
	}

	fgets(buffer, BYTES, f);
	
	while(!feof(f))
	{
		MapCharsInSingleLine(countingArray, buffer);
		fgets(buffer, BYTES, f);
	}
	
	PrintMappedCharacterInFile(countingArray);
	
	fclose(f);
}


void MapCharsInSingleLine(int* _counters, char* _line)
{
	size_t i;

	while(*_line != '\n')
	{
		_counters[*_line - LOWEST_VALID_CHAR]++; /* Increments the char occurances by 1 in the right place*/
		_line++;
	}
}


void PrintMappedCharacterInFile(int* _counters)
{
	size_t i;
	
	for (i = 0; i < TOTAL_NUM_OF_VALID_CHARS; i++)
	{
		if(_counters[i] != 0)
		{
			printf("%c: %d times.\n", (char)(i + LOWEST_VALID_CHAR), _counters[i]);
		}
	}
}


