#include <stdio.h>
#include "LinkedList.h"
#include "TestLinkedList.h"
#include "ADTDefs.h"

#define SUCCESS "Success" 
#define FAILURE "Failure" 
#define CHECK_FUNC(condition) (printf("%s\n", (condition) ? SUCCESS : FAILURE ))


void LinkedListCreateTest(void);
void LinkedListInsertHeadTest1(void);
void LinkedListInsertHeadTest2(void);
void LinkedListInsertTailTest1(void);
void LinkedListInsertTailTest2(void);
void LinkedListRemoveHeadTest1(void);
void LinkedListRemoveHeadTest2(void);
void LinkedListRemoveHeadTest3(void);
void LinkedListRemoveHeadTest4(void);
void LinkedListRemoveTailTest1(void);
void LinkedListRemoveTailTest2(void);
void LinkedListRemoveTailTest3(void);
void LinkedListRemoveTailTest4(void);
void LinkedListCountItemsTest(void);
void LinkedListIsEmptyTest(void);
void PrintLinkedListTest(void);


int main(void)
{
	/* Tests: */
	
	LinkedListCreateTest();

	LinkedListInsertHeadTest1();
	LinkedListInsertHeadTest2();

	LinkedListInsertTailTest1();
	LinkedListInsertTailTest2();
	
	LinkedListRemoveHeadTest1();
	LinkedListRemoveHeadTest2();
	LinkedListRemoveHeadTest3();
	LinkedListRemoveHeadTest4();
	
	LinkedListRemoveTailTest1();
	LinkedListRemoveTailTest2();
	LinkedListRemoveTailTest3();
	LinkedListRemoveTailTest4();
	
	LinkedListCountItemsTest();
	
	LinkedListIsEmptyTest();

	PrintLinkedListTest();
	
	return 0;
}


void LinkedListCreateTest(void)
{
	LinkedList* list = NULL;
	
	printf("\n*******LinkedList Create Test:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n");
	
	LinkedListDestroy(list);
}

void LinkedListInsertHeadTest1(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i;
	
	printf("\n*******LinkedList Insert Head Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 2; i++)
	{
		statusCode = LinkedListInsertHead(list, i + 1);
		
		printf("Insert Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 2 SUCCESS (statusCode test)\n");

	printf("\nInsert Head & Count Items Test 1: ");

	CHECK_FUNC(2 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");
	
	printf("\nInsert Head & Print LinkedList 1:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [2] ---> [1] ---> [NULL]\n\n");
	
	for (i = 2; i < 6; i++)
	{
		statusCode = LinkedListInsertHead(list, i + 1);
		
		printf("Insert Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 4 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Head & Count Items Test 2: ");
	
	CHECK_FUNC(6 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");

	printf("\nInsert Head & Print LinkedList 2:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [6] ---> 5 ---> [4] ---> [3] ---> [2] ---> [1] ---> [NULL]\n");

	LinkedListDestroy(list);
}

void LinkedListInsertHeadTest2(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i;
	
	printf("\n*******LinkedList Insert Head Test 2:*******\n\n");
	
	printf("Insert Head:\n\n");
	
	for (i = 0; i < 2 ; i++)
	{
		statusCode = LinkedListInsertHead(list, i + 1);
		
		printf("Insert Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == ERR_NOT_INITIALIZED);
	}
	
	printf("\nExpected: 2 FAILURE (statusCode test) - list == NULL\n");

	LinkedListDestroy(list);
}

void LinkedListInsertTailTest1(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i;
	
	printf("\n*******LinkedList Insert Tail Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 2; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 2 SUCCESS (statusCode test)\n");

	printf("\nInsert Tail & Count Items Test 1: ");

	CHECK_FUNC(2 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");
	
	printf("\nInsert Tail & Print LinkedList 1:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [NULL]\n\n");
	
	for (i = 2; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 4 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Tail & Count Items Test 2: ");
	
	CHECK_FUNC(6 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");

	printf("\nInsert Tail & Print LinkedList 2:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n");

	LinkedListDestroy(list);
}

void LinkedListInsertTailTest2(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i;
	
	printf("\n*******LinkedList Insert Tail Test 2:*******\n\n");
	
	printf("Insert Tail:\n\n");
	
	for (i = 0; i < 2; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == ERR_NOT_INITIALIZED);
	}
	
	printf("\nExpected: 2 FAILURE (statusCode test) - list == NULL\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveHeadTest1(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, j, outputData;
	
	printf("\n*******LinkedList Remove Head Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Tail & Print LinkedList:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n\n");
	
	for (i = 0, j = 1; i < 3; i++, j++)
	{
		statusCode = LinkedListRemoveHead(list, &outputData);
		
		printf("Remove Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
		
		printf("Output data number %d check: ", i + 1);
	
		CHECK_FUNC(j == outputData);
	}
	
	printf("\nExpected: 3 SUCCESS (check statusCode) 3 SUCCESS (check outputData)\n");
	
	printf("\nRemove Head & Count Items Test: ");
	
	CHECK_FUNC(3 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");

	printf("\nRemove Head & Print LinkedList:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [4] ---> [5] ---> [6] ---> [NULL]\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveHeadTest2(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Remove Head Test 2:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveHead(list, &outputData);
		
		printf("Remove Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode)\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveHeadTest3(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Remove Head Test 3:*******\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveHead(list, &outputData);
		
		printf("Remove Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - list == NULL\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveHeadTest4(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int *outputData = NULL;
	int i;
	
	printf("\n*******LinkedList Remove Head Test 4:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveHead(list, outputData);
		
		printf("Remove Head number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - outputData == NULL\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveTailTest1(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, j, outputData;
	
	printf("\n*******LinkedList Remove Tail Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Tail & Print LinkedList:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n\n");
	
	for (i = 0, j = 6; i < 3; i++, --j)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
		
		printf("Output data number %d check: ", i + 1);
	
		CHECK_FUNC(j == outputData);
	}
	
	printf("\nExpected: 3 SUCCESS (check statusCode) 3 SUCCESS (check outputData)\n");
	
	printf("\nRemove Tail & Count Items Test: ");
	
	CHECK_FUNC(3 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");

	printf("\nRemove Tail & Print LinkedList:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [NULL]\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveTailTest2(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Remove Tail Test 2:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode)\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveTailTest3(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Remove Tail Test 3:*******\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - list == NULL\n");

	LinkedListDestroy(list);
}

void LinkedListRemoveTailTest4(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int *outputData = NULL;
	int i;
	
	printf("\n*******LinkedList Remove Tail Test 4:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - outputData == NULL\n");

	LinkedListDestroy(list);
}

void LinkedListCountItemsTest(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Count Items Test:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nCount Items Test 1: ");
	
	CHECK_FUNC(6 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n\n");
	
	for (i = 0; i < 4; i++)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 4 SUCCESS (check statusCode)\n");
	
	printf("\nCount Items Test 2: ");
	
	CHECK_FUNC(2 == LinkedListCountItems(list));
	
	printf("\nExpected: SUCCESS (count items test)\n");

	LinkedListDestroy(list);
}

void LinkedListIsEmptyTest(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Is Empty Test:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	printf("Is Empty Func Test 1: ");
	
	CHECK_FUNC(1 == LinkedListIsEmpty(list));
	
	printf("\nExpected: SUCCESS (is empty test)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nIs Empty Func Test 2: ");
	
	CHECK_FUNC(0 == LinkedListIsEmpty(list));
	
	printf("\nExpected: SUCCESS (is empty test)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (check statusCode)\n");
	
	printf("\nIs Empty Func Test 3: ");
	
	CHECK_FUNC(1 == LinkedListIsEmpty(list));
	
	printf("\nExpected: SUCCESS (is empty test)\n");

	LinkedListDestroy(list);
}

void PrintLinkedListTest(void)
{
	LinkedList* list = NULL;
	ADTErr statusCode = ERR_OK;
	int i, outputData;
	
	printf("\n*******LinkedList Print Test:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, i + 1);
		
		printf("Insert Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nPrint LinkedList 1:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %d check (statusCode): ", i + 1);
		
		CHECK_FUNC(ERR_OK == statusCode);
	}
	
	printf("\nExpected: 3 SUCCESS (check statusCode)\n");

	printf("\nPrint LinkedList 2:\n\n");
	
	PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [NULL]\n");

	LinkedListDestroy(list);
}


