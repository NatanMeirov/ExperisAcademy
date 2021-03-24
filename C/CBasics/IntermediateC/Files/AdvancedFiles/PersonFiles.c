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


void WriteAllPersonsToBinFile(FILE* _f, Person* _pArr, size_t _size);
void ReadAllPersonsFromBinFile(FILE* _f, Person* _pArr, size_t _size);
void PrintAllPersons(Person* _pArr, size_t _size);
void PrintPerson(Person* _p);


int main(void)
{
	Person personArray[LENGTH];
	Person otherPersonArray[LENGTH];
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
	
	if(!(f = fopen("PersonsFile", "w")))
	{
		return 1; 
	}

	WriteAllPersonsToBinFile(f, personArray, LENGTH);
	
	fclose(f);

	if(!(f = fopen("PersonsFile", "r")))
	{
		return 1; /* Error */
	}

	ReadAllPersonsFromBinFile(f, otherPersonArray, LENGTH);

	fclose(f);
	
	PrintAllPersons(otherPersonArray, LENGTH);
	
	return 0;
}


void WriteAllPersonsToBinFile(FILE* _f, Person* _pArr, size_t _size)
{
	fwrite(_pArr, sizeof(Person), _size, _f); /* write bytes: sizeof(Person) * _size (of array) */
}


void ReadAllPersonsFromBinFile(FILE* _f, Person* _pArr, size_t _size)
{
	fread(_pArr, sizeof(Person), _size, _f); /* Read bytes: sizeof(Person) * _size (of array) */
}


void PrintAllPersons(Person* _pArr, size_t _size)
{
	size_t i;
	
	for(i = 0; i < _size; i++)
	{
		PrintPerson(_pArr + i);
	}
}


void PrintPerson(Person* _p)
{
	printf("[{%d, %s, %d}]\n", _p->m_id, _p->m_name, _p->m_age);
}




