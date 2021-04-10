#include <stdio.h> /* printf */
#include "../../Include/GenericLinkedList/GenericLinkedList.h"
#include "../../Include/GenericLinkedList/LinkedListIterator.h"

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
void LinkedListSizeTest(void);
void LinkedListIsEmptyTest(void);
/* void PrintLinkedListTest(void); */


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
	
	LinkedListSizeTest();
	
	LinkedListIsEmptyTest();

	/* PrintLinkedListTest(); */
	
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
	
	LinkedListDestroy(&list, NULL);
}

void LinkedListInsertHeadTest1(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i;
	
	printf("\n*******LinkedList Insert Head Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 2; i++)
	{
		statusCode = LinkedListInsertHead(list, &i);
		
		printf("Insert Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 2 SUCCESS (statusCode test)\n");

	printf("\nInsert Head & Size Test 1: ");

	CHECK_FUNC(2 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");
	
	printf("\nInsert Head & Print LinkedList 1:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [2] ---> [1] ---> [NULL]\n\n");
	
	for (i = 2; i < 6; i++)
	{
		statusCode = LinkedListInsertHead(list, &i);
		
		printf("Insert Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 4 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Head & Size Test 2: ");
	
	CHECK_FUNC(6 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");

	printf("\nInsert Head & Print LinkedList 2:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [6] ---> 5 ---> [4] ---> [3] ---> [2] ---> [1] ---> [NULL]\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListInsertHeadTest2(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i;
	
	printf("\n*******LinkedList Insert Head Test 2:*******\n\n");
	
	printf("Insert Head:\n\n");
	
	for (i = 0; i < 2 ; i++)
	{
		statusCode = LinkedListInsertHead(list, &i);
		
		printf("Insert Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == LINKEDLIST_UNINITIALIZED_ERROR);
	}
	
	printf("\nExpected: 2 FAILURE (statusCode test) - list == NULL\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListInsertTailTest1(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i;
	
	printf("\n*******LinkedList Insert Tail Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 2; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 2 SUCCESS (statusCode test)\n");

	printf("\nInsert Tail & Size Test 1: ");

	CHECK_FUNC(2 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");
	
	printf("\nInsert Tail & Print LinkedList 1:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [1] ---> [2] ---> [NULL]\n\n");
	
	for (i = 2; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 4 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Tail & Size Test 2: ");
	
	CHECK_FUNC(6 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");

	printf("\nInsert Tail & Print LinkedList 2:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListInsertTailTest2(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i;
	
	printf("\n*******LinkedList Insert Tail Test 2:*******\n\n");
	
	printf("Insert Tail:\n\n");
	
	for (i = 0; i < 2; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == LINKEDLIST_UNINITIALIZED_ERROR);
	}
	
	printf("\nExpected: 2 FAILURE (statusCode test) - list == NULL\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveHeadTest1(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, j, *outputData;
	
	printf("\n*******LinkedList Remove Head Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Tail & Print LinkedList:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n\n");
	
	for (i = 0, j = 0; i < 3; i++, j++)
	{
		statusCode = LinkedListRemoveHead(list, (void**)&outputData);
		
		printf("Remove Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
		
		printf("Output data number %x check: ", &i);
	
		CHECK_FUNC(j == *outputData);
	}
	
	printf("\nExpected: 3 SUCCESS (check statusCode) 3 SUCCESS (check outputData)\n");
	
	printf("\nRemove Head & Size Test: ");
	
	CHECK_FUNC(3 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");

	printf("\nRemove Head & Print LinkedList:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [4] ---> [5] ---> [6] ---> [NULL]\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveHeadTest2(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, *outputData;
	
	printf("\n*******LinkedList Remove Head Test 2:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveHead(list, (void**)&outputData);
		
		printf("Remove Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode)\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveHeadTest3(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, *outputData;
	
	printf("\n*******LinkedList Remove Head Test 3:*******\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveHead(list, (void**)&outputData);
		
		printf("Remove Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - list == NULL\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveHeadTest4(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i;
	
	printf("\n*******LinkedList Remove Head Test 4:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveHead(list, NULL);
		
		printf("Remove Head number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - outputData == NULL\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveTailTest1(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, j, *outputData;
	
	printf("\n*******LinkedList Remove Tail Test 1:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nInsert Tail & Print LinkedList:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n\n");
	
	for (i = 0, j = 6; i < 3; i++, --j)
	{
		statusCode = LinkedListRemoveTail(list, (void**)&outputData);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
		
		printf("Output data number %x check: ", &i);
	
		CHECK_FUNC(i == *outputData);
	}
	
	printf("\nExpected: 3 SUCCESS (check statusCode) 3 SUCCESS (check outputData)\n");
	
	printf("\nRemove Tail & Size Test: ");
	
	CHECK_FUNC(3 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");

	printf("\nRemove Tail & Print LinkedList:\n\n");
	
	/* PrintLinkedList(list); */
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [NULL]\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveTailTest2(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, *outputData;
	
	printf("\n*******LinkedList Remove Tail Test 2:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, (void**)&outputData);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode)\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveTailTest3(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, *outputData;
	
	printf("\n*******LinkedList Remove Tail Test 3:*******\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, (void**)&outputData);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - list == NULL\n");
	CHECK_FUNC(i == *outputData);
	printf("\nExpected: 1 FAILURE\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListRemoveTailTest4(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i;
	
	printf("\n*******LinkedList Remove Tail Test 4:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, NULL);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 FAILURE (check statusCode) - outputData == NULL\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListSizeTest(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, *outputData;
	
	printf("\n*******LinkedList Size Test:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nSize Test 1: ");
	
	CHECK_FUNC(6 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n\n");
	
	for (i = 0; i < 4; i++)
	{
		statusCode = LinkedListRemoveTail(list, (void**)&outputData);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 4 SUCCESS (check statusCode)\n");
	
	printf("\nSize Test 2: ");
	
	CHECK_FUNC(2 == LinkedListSize(list));
	
	printf("\nExpected: SUCCESS (Size test)\n");

	LinkedListDestroy(&list, NULL);
}

void LinkedListIsEmptyTest(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, *outputData;
	
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
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nIs Empty Func Test 2: ");
	
	CHECK_FUNC(0 == LinkedListIsEmpty(list));
	
	printf("\nExpected: SUCCESS (is empty test)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListRemoveTail(list, (void**)&outputData);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (check statusCode)\n");
	
	printf("\nIs Empty Func Test 3: ");
	
	CHECK_FUNC(1 == LinkedListIsEmpty(list));
	
	printf("\nExpected: SUCCESS (is empty test)\n");

	LinkedListDestroy(&list, NULL);
}

/*
void PrintLinkedListTest(void)
{
	LinkedList* list = NULL;
	LinkedListResult statusCode = LINKEDLIST_SUCCESS;
	int i, outputData;
	
	printf("\n*******LinkedList Print Test:*******\n\n");
	
	printf("Create Func Test: ");

	list = LinkedListCreate();
	
	CHECK_FUNC(NULL != list);
	
	printf("\nExpected: SUCCESS (NULL != list)\n\n");
	
	for (i = 0; i < 6; i++)
	{
		statusCode = LinkedListInsertTail(list, &i);
		
		printf("Insert Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 6 SUCCESS (statusCode test)\n");
	
	printf("\nPrint LinkedList 1:\n\n");
	
	// PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [4] ---> [5] ---> [6] ---> [NULL]\n\n");
	
	for (i = 0; i < 3; i++)
	{
		statusCode = LinkedListRemoveTail(list, &outputData);
		
		printf("Remove Tail number %x check (statusCode): ", &i);
		
		CHECK_FUNC(LINKEDLIST_SUCCESS == statusCode);
	}
	
	printf("\nExpected: 3 SUCCESS (check statusCode)\n");

	printf("\nPrint LinkedList 2:\n\n");
	
	// PrintLinkedList(list);
	
	printf("\n\nExpected: [1] ---> [2] ---> [3] ---> [NULL]\n");

	LinkedListDestroy(&list, NULL);
}
*/

