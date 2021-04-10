#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include "../../Include/GenericLinkedList/GenericLinkedList.h"
#include "../../Include/GenericLinkedList/LinkedListIterator.h"
#include "../../Include/GenericLinkedList/LinkedListIteratorFunctions.h"

#define ARRAY_SIZE 5
#define TIMES_IN_ARRAY 1


int PrintLinkedListElement(void* _elm, void* _context)
{
    printf("[%d] -> ", *((int*)_elm));

    return 1;
}

int CheckIfElementEqualTo(void* _elm, void* _context)
{
    return (*((int*)_elm) == *((int*)_context)) ? 1 : 0;
}


int main(void)
{
    size_t i;
    LinkedListIterator iterator;
    int array[ARRAY_SIZE] = {10, 20, 30, 40, 50};
    int temp = 30;
    int answer;
    int newToInsert = 100;
    void* p = NULL;
    LinkedList* list = LinkedListCreate();

    if(!list)
    {
        printf("Failed to create list...\n");
        return 1;
    }

    for(i = 0; i < ARRAY_SIZE; i++)
    {
        LinkedListInsertTail(list, array + i);
    }

    iterator = LinkedListIteratorBegin(list);

    if(LinkedListIteratorGet(iterator) != &array[0])
    {
        printf("Failed to get correct begin iterator or failed in IteratorGet...\n");
        return 1;
    }

    LinkedListIteratorForEach(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &PrintLinkedListElement, NULL);
    printf("[NULL] \n");

    answer = LinkedListIteratorCountIf(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &CheckIfElementEqualTo, (void*)&temp);
    printf("[%d], times in list - answer: %d, should be: %d\n", temp, answer, 1);

    iterator = LinkedListIteratorFindFirst(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &CheckIfElementEqualTo, (void*)&temp);
    if(iterator == LinkedListIteratorEnd(list))
    {
        printf("Failed in FindFirst...\n");
    }

    printf("Found: %d, should be: %d\n", *((int*)LinkedListIteratorGet(iterator)), temp);


    printf("Set using the current iterator:\n");
    LinkedListIteratorSet(iterator, &array[0]);

    LinkedListIteratorForEach(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &PrintLinkedListElement, NULL);
    printf("[NULL] \n");



    printf("Removing using the previous iterator:\n");
    iterator = LinkedListIteratorPrev(iterator);
    p = LinkedListIteratorRemove(iterator);

    LinkedListIteratorForEach(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &PrintLinkedListElement, NULL);
    printf("[NULL] \n");
    printf("Removed: %d\n", *((int*)p));


    printf("Inserting using the end iterator:\n");
    iterator = LinkedListIteratorEnd(list);
    LinkedListIteratorInsertBefore(iterator, &newToInsert);

    LinkedListIteratorForEach(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &PrintLinkedListElement, NULL);
    printf("[NULL] \n");



    printf("Inserting using the begin iterator:\n");
    iterator = LinkedListIteratorBegin(list);
    LinkedListIteratorInsertBefore(iterator, &newToInsert);

    LinkedListIteratorForEach(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &PrintLinkedListElement, NULL);
    printf("[NULL] \n");


    temp = 10;
    answer = LinkedListIteratorCountIf(LinkedListIteratorBegin(list), LinkedListIteratorEnd(list), &CheckIfElementEqualTo, (void*)&temp);
    printf("[%d], times in list - answer: %d, should be: %d\n", temp, answer, 2);


    LinkedListDestroy(&list, NULL);
    return 0;
}
