#include <stdio.h>
#include "TAIL.h"


int main(int argc, char* argv[])
{
	if(argc > 3)
	{
		printf("Error: Too many arguments...\n");
		return 1;
	}
	else if(argc < 3)
	{
		printf("Error: Two arguments are expected...\n");
		return 1;
	}
	
	return ShowTail(argv[1] /* Text file name */, argv[2] /* Number of lines from the EOF */); /* Returns a status */
}
