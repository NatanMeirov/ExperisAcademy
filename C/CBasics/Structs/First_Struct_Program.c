#include <stdio.h>

typedef struct person
{
	long m_Id;
	char m_Name[100];
	int m_Age;
	int m_Education;
} Person;


void PrintPerson(Person* pPtr);
Person GetPersonFromUser(void);


int main(void)
{
	Person p;
	Person* personPtr = &p;
	Person personArray[3] = {{123123, "Meirov", 22, 12}, {123123, "Avraham", 22, 12}, {123123, "Natan", 22, 12}};
	
	/* Part 1: */
	
	p = GetPersonFromUser();
	
	PrintPerson(personPtr);
	
	/* Part 2: */
	
	PrintPerson(&personArray[0]);
	
	
	return 0;
}


void PrintPerson(Person* pPtr)
{
	printf("\n\nPerson: \n{\n\tID: %ld,\n\tName: %s,\n\tAge: %d,\n\tEducation: %d\n}\n", pPtr->m_Id, pPtr->m_Name, pPtr->m_Age, pPtr->m_Education);
}


Person GetPersonFromUser(void)
{
	Person p;
	
	printf("ID: ");
	scanf("%ld", &p.m_Id);
	
	printf("FullName: ");
	scanf("%s", p.m_Name);
	
	printf("Age: ");
	scanf("%d", &p.m_Age);

	printf("Education: ");
	scanf("%d", &p.m_Education);

	return p;
}




