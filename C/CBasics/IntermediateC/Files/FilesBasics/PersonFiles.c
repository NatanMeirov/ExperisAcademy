#include <stdio.h>
#define LENGTH 5
#define BYTES 1024

typedef struct Person
{
	int m_id;
	char m_name[128];
	int m_age;
	struct Person* m_next;
} Person;


void WriteAllPersonsToFile(FILE* _f, Person* _pArr, size_t _size);
void WritePersonToFile(FILE* _f, Person* _p);
void ReadAllPersonsFromFile(FILE* _f);


int main(void)
{
	Person personArray[LENGTH];
	FILE* f = NULL;
	
	Person a = {23456, "Moshe Moshe", 50, NULL};
	Person b = {1999, "Some Person", 70, NULL};
	Person c = {777, "Natan Meirov", 25, NULL};
	Person d = {4091772, "Another Person", 11, NULL};
	Person e = {1, "Old Person", 120, NULL};
	personArray[0] = a;
	personArray[1] = b;
	personArray[2] = c;
	personArray[3] = d;
	personArray[4] = e;
	
	if(!(f = fopen("PersonsFile.txt", "w")))
	{
		return 1; /* Error */
	}

	WriteAllPersonsToFile(f, personArray, LENGTH);
	
	fclose(f);
	
	if(!(f = fopen("PersonsFile.txt", "r")))
	{
		return 1; /* Error */
	}

	ReadAllPersonsFromFile(f);

	fclose(f);
	
	return 0;
}


void WriteAllPersonsToFile(FILE* _f, Person* _pArr, size_t _size)
{
	size_t i;
	
	for(i = 0; i < _size; i++)
	{
		WritePersonToFile(_f, _pArr + i);
	}
}


void WritePersonToFile(FILE* _f, Person* _p)
{
	fprintf(_f, "[{%d, %s, %d}]\n", _p->m_id, _p->m_name, _p->m_age);
}


/* Scans every word separatly (because of '\0' in every word) */
/*
void ReadAllPersonsFromFile(FILE* _f)
{
	char word[1024];
	
	fscanf(_f, "%s", word);
	
	while(!feof(_f))
	{
		printf("%s\n", word);
		fscanf(_f, "%s", word);
	}
}
*/


void ReadAllPersonsFromFile(FILE* _f)
{
	char line[BYTES];

	fgets(line, BYTES, _f);
	
	while(!feof(_f))
	{
		printf("%s\n", line);
		fgets(line, BYTES, _f);
	}
}



