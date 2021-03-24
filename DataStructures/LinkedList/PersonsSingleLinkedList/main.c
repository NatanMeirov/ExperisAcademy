#include <stdio.h>
#include <stdlib.h>
#include "PersonLinkedList.h"

#define LENGTH 5

void NullPersonNextPtrFields(Person* _p);
void NullAllPersonsNextPtrFields(Person* _pArr, size_t _size);
void PrintPerson(Person* _p);
void Test_ListInsertHead(Person* _pArr, size_t _size);
void Test_ListRemoveHead(Person* _pArr, size_t _size);
void Test_ListInsertByKey(Person* _pArr, size_t _size);
void Test_ListInsertByKeyRec(Person* _pArr, size_t _size);
void Test_ListRemoveByKey(Person* _pArr, size_t _size);
void Test_ListRemoveByKeyRec(Person* _pArr, size_t _size);


int main(void)
{
	Person* head = NULL;
	
	Person personArray[LENGTH];
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
	
	/* Tests: */
	
	Test_ListInsertHead(personArray, LENGTH);
	Test_ListRemoveHead(personArray, LENGTH);
	Test_ListInsertByKey(personArray, LENGTH);
	Test_ListInsertByKeyRec(personArray, LENGTH);
	Test_ListRemoveByKey(personArray, LENGTH);
	Test_ListRemoveByKeyRec(personArray, LENGTH);

	return 0;
}


/* Trigger after every test: */
void NullPersonNextPtrFields(Person* _p)
{
	_p->m_next = NULL;
}

void NullAllPersonsNextPtrFields(Person* _pArr, size_t _size)
{
	size_t i;
	
	for(i = 0; i < _size; i++)
	{
		NullPersonNextPtrFields(_pArr + i);
	}
}

void PrintPerson(Person* _p)
{
	printf("[{%d, %s, %d}]\n", _p->m_id, _p->m_name, _p->m_age);
}


void Test_ListInsertHead(Person* _pArr, size_t _size)
{
	size_t i;
	Person* head = _pArr;
	
	NullAllPersonsNextPtrFields(_pArr, _size);
	
	for(i = 1; i < _size; i++)
	{
		head = ListInsertHead(head, _pArr + i);
	}
	
	PrintList(head);
	
	i = 2;
	NullAllPersonsNextPtrFields(_pArr, _size);
	
	head = ListInsertHead(NULL, _pArr + i);
	PrintList(head);
	
	head = ListInsertHead(head, NULL);
	PrintList(head);
	
	printf("Test_ListInsertHead: Passed!\n");
}


void Test_ListRemoveHead(Person* _pArr, size_t _size)
{
	size_t i;
	Person* head = _pArr;
	Person *temp = NULL;
	
	NullAllPersonsNextPtrFields(_pArr, _size);
	
	for(i = 1; i < _size; i++)
	{
		head = ListInsertHead(head, _pArr + i);
	}
	PrintList(head);
	
	head = ListRemoveHead(head, &temp);
	PrintList(head);
	printf("Removed: ");
	PrintPerson(temp);
	
	head = ListRemoveHead(head, &temp);
	PrintList(head);
	printf("Removed: ");
	PrintPerson(temp);
	
	head = ListRemoveHead(NULL, &temp);
	PrintList(head); /* No list to print - head is NULL */
	
	head = ListRemoveHead(head, NULL);
	PrintList(head);
	
	printf("Test_ListRemoveHead: Passed!\n");
}


void Test_ListInsertByKey(Person* _pArr, size_t _size)
{
	/* TODO */
}


void Test_ListInsertByKeyRec(Person* _pArr, size_t _size)
{
	/* TODO */
}


void Test_ListRemoveByKey(Person* _pArr, size_t _size)
{
	/* TODO */
}


void Test_ListRemoveByKeyRec(Person* _pArr, size_t _size)
{
	/* TODO */
}


