/**
 * @file GenericLinkedList.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * Using head and tail sentinels
 * Each node is allocated in the heap, and stores a pointer to user proveded element
 * Supports LinkedListIterator functionality
 * In version 2.1: Performance improvements
 * @version 2.1
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
static LinkedListResult ValidateNotUnderflowEmptyLinkedList(const LinkedList* _list);

/* Helper Functions Declarations: */

static LinkedList* InitializeNewLinkedList(LinkedList* _list);
static void DestroyAllNodes(LinkedList* _list, void (*_elementDestroy)(void* _element));
static void DeleteItemFromLinkedList(Node* _nodeToRemove, void** _pElement);


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

LinkedList* LinkedListCreate(void)
{
	LinkedList* newList = (LinkedList*)malloc(sizeof(LinkedList));

	if(!newList)
	{
		return NULL;
	}

	return InitializeNewLinkedList(newList);
}


void LinkedListDestroy(LinkedList** _list, void (*_elementDestroy)(void* _element))
{
	if(_list && *_list)
	{
		DestroyAllNodes(*_list, _elementDestroy);
		free(*_list);
        *_list = NULL;
	}
}


LinkedListResult LinkedListInsertHead(LinkedList* _list, void* _element)
{
	Node* newNode = NULL;

	if(!_list)
	{
		return LINKEDLIST_UNINITIALIZED_ERROR;
	}

	newNode = CreateNewNode(_element);
	if(!newNode)
	{
		return LINKEDLIST_ALLOCATION_ERROR;
	}

	InsertNodeToLinkedList(newNode, _list->m_head.m_next);

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListInsertTail(LinkedList* _list, void* _element)
{
	Node* newNode = NULL;

	if(!_list)
	{
		return LINKEDLIST_UNINITIALIZED_ERROR;
	}

	newNode = CreateNewNode(_element);
	if(!newNode)
	{
		return LINKEDLIST_ALLOCATION_ERROR;
	}

	InsertNodeToLinkedList(newNode, &(_list->m_tail));

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListRemoveHead(LinkedList* _list, void** _pElement)
{
	LinkedListResult statusCode;

	if(!_list || !_pElement)
	{
		return LINKEDLIST_UNINITIALIZED_ERROR;
	}

	if((statusCode = ValidateNotUnderflowEmptyLinkedList(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	DeleteItemFromLinkedList(_list->m_head.m_next, _pElement);

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListRemoveTail(LinkedList* _list, void** _pElement)
{
	LinkedListResult statusCode;

	if(!_list || !_pElement)
	{
		return LINKEDLIST_UNINITIALIZED_ERROR;
	}

	if((statusCode = ValidateNotUnderflowEmptyLinkedList(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	DeleteItemFromLinkedList(_list->m_tail.m_prev, _pElement);

	return LINKEDLIST_SUCCESS;
}


size_t LinkedListSize(const LinkedList* _list)
{
	size_t nodesCounter = 0;
	Node* currentNode = NULL;

	if(!_list)
	{
		return MAX_SIZE_T;
	}

	currentNode = _list->m_head.m_next;

	while(currentNode != &(_list->m_tail))
	{
		nodesCounter++;
		currentNode = currentNode->m_next;
	}

	return nodesCounter;
}


int LinkedListIsEmpty(const LinkedList* _list)
{
	if(!_list)
	{
		return -1;
	}

	return (_list->m_head.m_next == &(_list->m_tail)) ? IS_EMPTY : IS_NOT_EMPTY;
}


/*------------------------------------------- End of Main API Functions -------------------------------------------*/


/* Validation Functions: */

static LinkedListResult ValidateNotUnderflowEmptyLinkedList(const LinkedList* _list)
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

	while(currentNode != &(_list->m_tail)) /* While did not reach to the tail of the LinkedList */
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
