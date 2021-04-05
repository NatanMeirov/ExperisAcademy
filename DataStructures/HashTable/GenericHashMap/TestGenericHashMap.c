#include <stdio.h> /* printf */
#include <stdlib.h> /* free */
#include "GenericHashMap.h"
#include "../../LinkedList/GenericLinkedList/GenericLinkedListUsingIterator/GenericLinkedList.h"
#include "../../LinkedList/GenericLinkedList/GenericLinkedListUsingIterator/LinkedListIterator.h"
#include "../../LinkedList/GenericLinkedList/GenericLinkedListUsingIterator/LinkedListFunctions.h"

#define CAPACITY 10
#define ARR_SIZE 2000
#define MAX_SIZE_T ((size_t)-1)

#define SUCCESS ("\033[1;32mSuccess\033[0m")
#define FAILURE ("\033[1;31mFailure\033[0m")
#define CHECK_FUNC(condition) (printf("%s\n", (condition) ? SUCCESS : FAILURE ))

/*********************************************************************************/

static size_t HashFunc(const void *_element)
{
    return (*(int*)_element * 3);
}

static int EqualityFunc(const void *_firstElement, const void *_secondElement)
{
    return (*(int*)_firstElement == *(int*)_secondElement);
}

static void FreeElement(void *_element)
{
	free(_element);
}

/*********************************************************************************/

void HashMapCreateTest1(void)
{
	HashMap* hashMapTest = NULL;

	printf("\033[1;34m\n*******Hash Map Create Test 1:*******\n\n\033[0m");
	
	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");
	
	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapCreateTest2(void)
{
	HashMap* hashMapTest = NULL;

	printf("\033[1;34m\n*******Hash Map Create Test 2:*******\n\n\033[0m");
	
	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(1, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL == hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL == hashMapTest) - capacity is 1\n");
	
	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapCreateTest3(void)
{
	HashMap* hashMapTest = NULL;

	printf("\033[1;34m\n*******Hash Map Create Test 3:*******\n\n\033[0m");
	
	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, NULL, EqualityFunc);
	
	CHECK_FUNC(NULL == hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL == hashMapTest) - HashFunc is NULL\n");
	
	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapCreateTest4(void)
{
	HashMap* hashMapTest = NULL;

	printf("\033[1;34m\n*******Hash Map Create Test 4:*******\n\n\033[0m");
	
	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, NULL);
	
	CHECK_FUNC(NULL == hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL == hashMapTest) - EqualityFunc is NULL\n");
	
	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapDestroyTest(void)
{
	HashMap* hashMapTest = NULL;
    int *key = NULL;
    int *value = NULL;
    int status;

    key = (int*)malloc(sizeof(int));
    value = (int*)malloc(sizeof(int));

    *key = 4;
    *value = 5;

	printf("\033[1;34m\n*******Hash Map Destroy Test 1:*******\n\n\033[0m");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

	status = HashMapInsert(hashMapTest, key, value);
		
	printf("Insert number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);
	
    printf("\nExpected: 1 SUCCESS (check status)\n");

	HashMapDestroy(&hashMapTest, FreeElement, FreeElement);
}

void HashMapInsertTest1(void)
{
	HashMap* hashMapTest = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Insert Test 1:*******\n\n\033[0m");
	
	printf("number of insert = 5\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 5;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i + 10;

		status = HashMapInsert(hashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 5 SUCCESS (check status)\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapInsertTest2(void)
{
	HashMap* hashMapTest = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Insert Test 2:*******\n\n\033[0m");
	
	printf("number of insert = 100\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 1500;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i + 10;

		status = HashMapInsert(hashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 100 SUCCESS (check status)\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapInsertTest3(void)
{
	HashMap* hashMapTest = NULL;
    int keyArr[ARR_SIZE];
    int *value = NULL;
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Insert Test 3:*******\n\n\033[0m");
	
	printf("number of insert = 8\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nInsert Func Test 1:\n\n");

    keyArr[0] = 10;
    keyArr[1] = 5;
    keyArr[2] = 10;

    for (i = 0;i < 3;++i)
	{
		status = HashMapInsert(hashMapTest, &keyArr[i], value);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 2 SUCCESS 1 FAILURE (check status) - duplicate\n");

    printf("\nInsert Func Test 2:\n\n");

    keyArr[3] = 20;
    keyArr[4] = 6;
    keyArr[5] = 4;
    keyArr[6] = 7;
    keyArr[7] = 2;

    for (i = 3;i < 8;++i)
	{
		status = HashMapInsert(hashMapTest, &keyArr[i], value);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}

    printf("\nExpected: 5 SUCCESS (check status)\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapInsertTest4(void)
{
	HashMap* hashMapTest = NULL;
    int *key = NULL;
    int *value = NULL;
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Insert Test 4:*******\n\n\033[0m");
	
	printf("number of insert = 2\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 2;++i)
	{
		status = HashMapInsert(hashMapTest, key, value);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_UNINITIALIZED_ERROR == status);
	}
	
    printf("\nExpected: 2 SUCCESS (check status) - element is NULL\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapInsertTest5(void)
{
	HashMap* hashMapTest = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Insert Test 5:*******\n\n\033[0m");
	
	printf("number of insert = 2\n\n");

    printf("Insert Func Test:\n\n");

    for (i = 0;i < 2;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i;

		status = HashMapInsert(hashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_UNINITIALIZED_ERROR == status);
	}

    printf("\nExpected: 2 SUCCESS (check status) - hashMapTest is NULL\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}


void HashMapRemoveTest1(void)
{
	HashMap* HashMapTest = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Remove Test 1:*******\n\n\033[0m");
	
	printf("number of insert = 9, number of remove = 3\n\n");

	printf("Create Func Test: ");

	HashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != HashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != HashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 9;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i;

		status = HashMapInsert(HashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 9 SUCCESS (check status)\n");

    printf("\nRemove Func Test 1:\n\n");

	status = HashMapRemove(HashMapTest, &keyArr[0], &pKey, &pValue);
		
	printf("Remove number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 1 check (value): ");

    CHECK_FUNC(0 == *(int*)pKey);

    printf("Remove number 1 check (value): ");

    CHECK_FUNC(0 == *(int*)pValue);

    

    printf("\nRemove Func Test 2:\n\n");

	status = HashMapRemove(HashMapTest, &keyArr[7], &pKey, &pValue);
		
	printf("Remove number 2 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(7 == *(int*)pKey);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(7 == *(int*)pValue);



    printf("\nRemove Func Test 3:\n\n");

	status = HashMapRemove(HashMapTest, &keyArr[2], &pKey, &pValue);
		
	printf("Remove number 3 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 3 check (value): ");

    CHECK_FUNC(2 == *(int*)pKey);

    printf("Remove number 3 check (value): ");

    CHECK_FUNC(2 == *(int*)pValue);

    printf("\nExpected: 3 SUCCESS (check status) 6 SUCCESS (check value)\n");

    printf("\nSize Func Test: ");

    CHECK_FUNC(6 == HashMapSize(HashMapTest));

	HashMapDestroy(&HashMapTest, NULL, NULL);
}

void HashMapRemoveTest2(void)
{
	HashMap* HashMapTest = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Remove Test 2:*******\n\n\033[0m");
	
	printf("number of insert = 9, number of remove = 2\n\n");

	printf("Create Func Test: ");

	HashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != HashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != HashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 9;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i;

		status = HashMapInsert(HashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 9 SUCCESS (check status)\n");

    printf("\nRemove Func Test 1:\n\n");

	status = HashMapRemove(HashMapTest, &keyArr[0], &pKey, &pValue);
		
	printf("Remove number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 1 check (pKey value): ");

    CHECK_FUNC(0 == *(int*)pKey);

    printf("Remove number 1 check (pValue value): ");

    CHECK_FUNC(0 == *(int*)pValue);

    printf("\nRemove Func Test 2:\n\n");

    status = HashMapRemove(HashMapTest, &keyArr[0], &pKey, &pValue);

    printf("Remove number 2 check (status): ");
		
	CHECK_FUNC(HASHMAP_KEY_NOT_FOUND_ERROR == status);

    printf("\nExpected: 2 SUCCESS (check status) 1 SUCCESS (check value) - is already freed\n");

	HashMapDestroy(&HashMapTest, NULL, NULL);
}

void HashMapRemoveTest3(void)
{
	HashMap* HashMapTest = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Remove Test 3:*******\n\n\033[0m");
	
	printf("number of insert = 9, number of remove = 2\n\n");

	printf("Create Func Test: ");

	HashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != HashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != HashMapTest)\n");

    printf("\nInsert Func Test 1:\n\n");

    for (i = 0;i < 9;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i;

		status = HashMapInsert(HashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 9 SUCCESS (check status)\n");

    printf("\nRemove Func Test 1:\n\n");

	status = HashMapRemove(HashMapTest, &keyArr[0], &pKey, &pValue);
		
	printf("Remove number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 1 check (pKey value): ");

    CHECK_FUNC(0 == *(int*)pKey);

    printf("Remove number 1 check (pValue value): ");

    CHECK_FUNC(0 == *(int*)pValue);

    printf("\nExpected: 1 SUCCESS (check status) 2 SUCCESS (check value)\n");

    printf("\nInsert Func Test 2:\n\n");

	status = HashMapInsert(HashMapTest, &keyArr[0], &valueArr[0]);
		
	printf("Insert number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

	HashMapDestroy(&HashMapTest, NULL, NULL);
}

void HashMapRemoveTest4(void)
{
	HashMap* HashMapTest = NULL;
    void *key = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Remove Test 4:*******\n\n\033[0m");
	
	printf("number of remove = 2\n\n");

	printf("Create Func Test: ");

	HashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != HashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != HashMapTest)\n");

    printf("\nRemove Func Test:\n\n");

    for (i = 0;i < 2;++i)
	{
	    status = HashMapRemove(HashMapTest, key, &pKey, &pValue);
		
		printf("Remove number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_UNINITIALIZED_ERROR == status);
	}
	
    printf("\nExpected: 2 SUCCESS (check status) - key/pKey/pValue is NULL\n");

	HashMapDestroy(&HashMapTest, NULL, NULL);
}

void HashMapRemoveTest5(void)
{
	HashMap* HashMapTest = NULL;
    void *key = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Remove Test 5:*******\n\n\033[0m");
	
	printf("number of remove = 2\n\n");

    printf("Remove Func Test:\n\n");

    for (i = 0;i < 2;++i)
	{
		status = HashMapRemove(HashMapTest, key, &pKey, &pValue);
		
		printf("Remove number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_UNINITIALIZED_ERROR == status);
	}
	
    printf("\nExpected: 2 SUCCESS (check status) - HashMapTest is NULL\n");

	HashMapDestroy(&HashMapTest, NULL, NULL);
}

void HashMapFindTest1(void)
{
	HashMap* hashMapTest = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int foundElement1 = 12;
    int foundElement2 = 0;
    int foundElement3 = 4;
    int foundElement4 = 2;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Find Test 1:*******\n\n\033[0m");
	
	printf("number of insert = 9, number of remove = 3, number of is found = 5\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 9;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i;

		status = HashMapInsert(hashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 9 SUCCESS (check status)\n");

    printf("\nFind Func Test 1 (status): ");

    status = HashMapFind(hashMapTest, &keyArr[0], &pValue);
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Find Func Test 1 (value): ");

    CHECK_FUNC(0 == *(int*)pValue);
    

    printf("\nFind Func Test 2 (status): ");

    status = HashMapFind(hashMapTest, &foundElement1, &pValue);
		
	CHECK_FUNC(HASHMAP_KEY_NOT_FOUND_ERROR == status);


    printf("\nRemove Func Test 1:\n\n");

	status = HashMapRemove(hashMapTest, &keyArr[0], &pKey, &pValue);
		
	printf("Remove number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 1 check (pKey value): ");

    CHECK_FUNC(0 == *(int*)pKey);

    printf("Remove number 1 check (pValue value): ");

    CHECK_FUNC(0 == *(int*)pValue);


    printf("\nFind Func Test 3 (status): ");

    status = HashMapFind(hashMapTest, &foundElement1, &pValue);
		
	CHECK_FUNC(HASHMAP_KEY_NOT_FOUND_ERROR == status);


    printf("\nRemove Func Test 2:\n\n");

    status = HashMapRemove(hashMapTest, &keyArr[7], &pKey, &pValue);
		
	printf("Remove number 2 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(7 == *(int*)pKey);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(7 == *(int*)pValue);


    printf("\nRemove Func Test 3:\n\n");

    status = HashMapRemove(hashMapTest, &keyArr[4], &pKey, &pValue);
		
	printf("Remove number 2 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(4 == *(int*)pKey);


    printf("\nFind Func Test 4 (status): ");

    status = HashMapFind(hashMapTest, &foundElement3, &pValue);
		
	CHECK_FUNC(HASHMAP_KEY_NOT_FOUND_ERROR == status);


    printf("\nExpected: 3 SUCCESS (check status) 8 SUCCESS (check value)\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapFindTest2(void)
{
	HashMap* hashMapTest = NULL;
    void *pValue = NULL;
    int *key = NULL;
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Find Test 2:*******\n\n\033[0m");
	
	printf("number of is found = 2\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nIs Found Func Test:\n\n");

    for (i = 0;i < 2;++i)
	{
	    status = HashMapFind(hashMapTest, key, &pValue);
		
		printf("Find number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_UNINITIALIZED_ERROR == status);
	}
	
    printf("\nExpected: 2 SUCCESS (check status) - key/pValue is NULL\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapFindTest3(void)
{
	HashMap* hashMapTest = NULL;
    void *pValue = NULL;
    int *key = NULL;
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Find Test 3:*******\n\n\033[0m");
	
	printf("number of find = 2\n\n");

    printf("Find Func Test:\n\n");

    for (i = 0;i < 2;++i)
	{
	    status = HashMapFind(hashMapTest, key, &pValue);
		
		printf("Is Found number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_UNINITIALIZED_ERROR == status);
	}
	
    printf("\nExpected: 2 SUCCESS (check status) - hashMapTest is NULL\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}
void HashMapSizeTest1(void)
{
	HashMap* hashMapTest = NULL;
    void *pKey = NULL;
    void *pValue = NULL;
    int keyArr[ARR_SIZE];
    int valueArr[ARR_SIZE];
    int status;
    size_t i;

	printf("\033[1;34m\n*******Hash Map Size Test 1:*******\n\n\033[0m");
	
	printf("number of insert = 9, number of remove = 3, number of size = 4\n\n");

	printf("Create Func Test: ");

	hashMapTest = HashMapCreate(CAPACITY, HashFunc, EqualityFunc);
	
	CHECK_FUNC(NULL != hashMapTest);
	
	printf("\nExpected: SUCCESS (NULL != hashMapTest)\n");

    printf("\nSize Func Test 1: ");

    CHECK_FUNC(0 == HashMapSize(hashMapTest));

    printf("\nExpected: 1 SUCCESS (value check)\n");

    printf("\nInsert Func Test:\n\n");

    for (i = 0;i < 9;++i)
	{
		keyArr[i] = i;

        valueArr[i] = i;

		status = HashMapInsert(hashMapTest, &keyArr[i], &valueArr[i]);
		
		printf("Insert number %ld check (status): ", i + 1);
		
		CHECK_FUNC(HASHMAP_SUCCESS == status);
	}
	
    printf("\nExpected: 9 SUCCESS (check status)\n");

    printf("\nSize Func Test 2: ");

    CHECK_FUNC(9 == HashMapSize(hashMapTest));

    printf("\nRemove Func Test 1:\n\n");

	status = HashMapRemove(hashMapTest, &keyArr[0], &pKey, &pValue);
		
	printf("Remove number 1 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 1 check (value): ");

    CHECK_FUNC(0 == *(int*)pKey);

    printf("Remove number 1 check (value): ");

    CHECK_FUNC(0 == *(int*)pValue);

    printf("\nSize Func Test 3: ");

    CHECK_FUNC(8 == HashMapSize(hashMapTest));

    printf("\nRemove Func Test 2:\n\n");

	status = HashMapRemove(hashMapTest, &keyArr[7], &pKey, &pValue);
		
	printf("Remove number 2 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(7 == *(int*)pKey);

    printf("Remove number 2 check (value): ");

    CHECK_FUNC(7 == *(int*)pValue);

    printf("\nRemove Func Test 3:\n\n");

	status = HashMapRemove(hashMapTest, &keyArr[2], &pKey, &pValue);
		
	printf("Remove number 3 check (status): ");
		
	CHECK_FUNC(HASHMAP_SUCCESS == status);

    printf("Remove number 3 check (value): ");

    CHECK_FUNC(2 == *(int*)pKey);

    printf("Remove number 3 check (value): ");

    CHECK_FUNC(2 == *(int*)pValue);

    printf("\nSize Func Test 4: ");

    CHECK_FUNC(6 == HashMapSize(hashMapTest));

    printf("\nExpected: 12 SUCCESS (check status + value)\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

void HashMapSizeTest2(void)
{
	HashMap* hashMapTest = NULL;

	printf("\033[1;34m\n*******Hash Map Size Test 1:*******\n\n\033[0m");
	
	printf("number of size = 1\n\n");

    printf("Size Func Test: ");
		
	CHECK_FUNC(MAX_SIZE_T ==  HashMapSize(hashMapTest));
	
    printf("\nExpected: 1 SUCCESS (check status) - hashMapTest is NULL\n");

	HashMapDestroy(&hashMapTest, NULL, NULL);
}

int main(void)
{
    HashMapCreateTest1();
    HashMapCreateTest2();
    HashMapCreateTest3();
    HashMapCreateTest4();

    HashMapDestroyTest();

    HashMapInsertTest1();
    HashMapInsertTest2();
    HashMapInsertTest3();
    HashMapInsertTest4();
    HashMapInsertTest5();

    HashMapRemoveTest1();
    HashMapRemoveTest2();
    HashMapRemoveTest3();
    HashMapRemoveTest4();
    HashMapRemoveTest5();

    HashMapFindTest1();
    HashMapFindTest2();
    HashMapFindTest3();

    HashMapSizeTest1();
    HashMapSizeTest2();

    return 0;
}