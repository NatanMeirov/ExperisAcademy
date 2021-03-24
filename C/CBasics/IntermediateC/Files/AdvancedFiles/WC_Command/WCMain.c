#include <stdio.h>
#include "WC.h"


int main(int argc, char* argv[])
{
	if(argc > 2)
	{
		printf("Too many arguments...\n");
		return 1;
	}
	else if(argc < 2)
	{
		printf("One argument is expected...\n");
		return 1;
	}
	
	return WordsCount(argv[1] /* Text file name */); /* Returns a status */
}
