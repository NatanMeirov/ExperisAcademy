#include <stdio.h>


int main(void)
{
	FILE* f = NULL;
	
	if(!(f = fopen("Test.txt", "w")))
	{
		return;
	}
	
	fprintf(f, "Hello World of Files!");
	
	fclose(f);
}
