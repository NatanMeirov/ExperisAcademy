/**
 * @file GenericLinkedList.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * Using head and tail sentinels
 * Each node is allocated in the heap, and stores a pointer to user proveded element
 * Supports LinkedListIterator functionality
 * @version 2.0
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include "LinkedListInternal.h"
#include "GenericLinkedList.h"


/* Defines: */

#define IS_EMPTY 1
#define IS_NOT_EMPTY 0
#define MAX_SIZE_T ((size_t) -1)


/* Validation Functions Declarations: */

static LinkedListResult ValidateNotNullLinkedListPtr(const LinkedList* _list);
static LinkedListResult ValidateNotNullElementPtr(void** _pElement);
static LinkedListResult ValidateNotNullElement(void* _element);
static LinkedListResult ValidateSuccessfulNodeAllocation(const Node* _node);
static LinkedListResult ValidateNotEmptyLinkedList(const LinkedList* _list);


/* Helper Functions Declarations: */

static LinkedList* InitializeNewLinkedList(LinkedList* _list);
static void DestroyAllNodes(LinkedList* _list, void (*_elementDestroy)(void* _element));
static void DeleteItemFromLinkedList(Node* _nodeToRemove, void** _pElement);


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

LinkedList* LinkedListCreate(void)
{
	LinkedList* newList = malloc(sizeof(LinkedList));

	if(ValidateNotNullLinkedListPtr(newList) != LINKEDLIST_SUCCESS)
	{
		return NULL;
	}

	return InitializeNewLinkedList(newList);
}


void LinkedListDestroy(LinkedList** _list, void (*_elementDestroy)(void* _element))
{
	if(_list && ValidateNotNullLinkedListPtr(*_list) == LINKEDLIST_SUCCESS)
	{
		DestroyAllNodes(*_list, _elementDestroy);
		free(*_list);
        *_list = NULL;
	}
}


LinkedListResult LinkedListInsertHead(LinkedList* _list, void* _element)
{
	LinkedListResult statusCode;
	Node* newNode = NULL;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	newNode = CreateNewNode(_element);

	if((statusCode = ValidateSuccessfulNodeAllocation(newNode)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	InsertNodeToLinkedList(newNode, _list->m_head.m_next);

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListInsertTail(LinkedList* _list, void* _element)
{
	LinkedListResult statusCode;
	Node* newNode = NULL;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	newNode = CreateNewNode(_element);

	if((statusCode = ValidateSuccessfulNodeAllocation(newNode)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	InsertNodeToLinkedList(newNode, &(_list->m_tail));

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListRemoveHead(LinkedList* _list, void** _pElement)
{
	LinkedListResult statusCode;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS || (statusCode = ValidateNotNullElementPtr(_pElement)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotEmptyLinkedList(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	DeleteItemFromLinkedList(_list->m_head.m_next, _pElement);

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListRemoveTail(LinkedList* _list, void** _pElement)
{
	LinkedListResult statusCode;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS || (statusCode = ValidateNotNullElementPtr(_pElement)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotEmptyLinkedList(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	DeleteItemFromLinkedList(_list->m_tail.m_prev, _pElement);

	return LINKEDLIST_SUCCESS;
}


size_t LinkedListSize(const LinkedList* _list)
{
	size_t nodesCounter = 0;
	Node* currentNode = _list->m_head.m_next;

	if(ValidateNotNullLinkedListPtr(_list) != LINKEDLIST_SUCCESS)
	{
		return MAX_SIZE_T;
	}

	while(currentNode != &(_list->m_tail))
	{
		nodesCounter++;
		currentNode = currentNode->m_next;
	}

	return nodesCounter;
}


int LinkedListIsEmpty(const LinkedList* _list)
{
	if(ValidateNotNullLinkedListPtr(_list) != LINKEDLIST_SUCCESS)
	{
		return -1;
	}

	return (_list->m_head.m_next == &(_list->m_tail)) ? IS_EMPTY : IS_NOT_EMPTY;
}


/*------------------------------------------- End of Main API Functions -------------------------------------------*/


/* Validation Functions: */

static LinkedListResult ValidateNotNullLinkedListPtr(const LinkedList* _list)
{
	return (!_list) ? LINKEDLIST_UNINITIALIZED_ERROR : LINKEDLIST_SUCCESS;
}


static LinkedListResult ValidateNotNullElementPtr(void** _pElement)
{
	return (!_pElement) ? LINKEDLIST_UNINITIALIZED_ERROR : LINKEDLIST_SUCCESS;
}


static LinkedListResult ValidateNotNullElement(void* _element)
{
    return (!_element) ? LINKEDLIST_NULL_ELEMENT_ERROR : LINKEDLIST_SUCCESS;
}


static LinkedListResult ValidateSuccessfulNodeAllocation(const Node* _node)
{
	return (!_node) ? LINKEDLIST_ALLOCATION_ERROR : LINKEDLIST_SUCCESS;
}


static LinkedListResult ValidateNotEmptyLinkedList(const LinkedList* _list)
{
	return (_list->m_head.m_next == &(_list->m_tail)) ? LINKEDLIST_UNDERFLOW_ERROR : LINKEDLIST_SUCCESS;
}


/* Helper Functions: */

static LinkedList* InitializeNewLinkedList(LinkedList* _list)
{
	_list->m_head.m_element = NULL;
	_list->m_tail.m_element = NULL;

	_list->m_head.m_next = &(_list->m_tail);
	_list->m_head.m_prev = &(_list->m_head); /* Points to itself */

	_list->m_tail.m_next = &(_list->m_tail); /* Points to itself */
	_list->m_tail.m_prev = &(_list->m_head);

	return _list;
}


static void DestroyAllNodes(LinkedList* _list, void (*_elementDestroy)(void* _element))
{
	Node* tempNextNodePtr = NULL;
	Node* currentNode = _list->m_head.m_next;

	while(currentNode != &(_list->m_tail)) /* While did not reach the tail of the LinkedList */
	{
		tempNextNodePtr = currentNode->m_next;
		DestroySingleNode(currentNode, _elementDestroy);
		currentNode = tempNextNodePtr;
	}
}


static void DeleteItemFromLinkedList(Node* _nodeToRemove, void** _pElement)
{
	*_pElement = _nodeToRemove->m_element; /* Extracting the item and return it by reference */
	RemoveNodeFromLinkedList(_nodeToRemove);
	DestroySingleNode(_nodeToRemove, NULL);
}
