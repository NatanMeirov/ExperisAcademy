/**
 * @file GenericLinkedList.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * Using head and tail sentinels
 * Each node is allocated in the heap, and stores a pointer to user proveded element
 * @version 1.0
 * @date 2021-03-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include "GenericLinkedList.h"


/* Defines: */

#define IS_EMPTY 1
#define IS_NOT_EMPTY 0
#define MAX_SIZE_T ((size_t) -1)

typedef struct Node
{
	void* m_item;
	struct Node* m_prev;
	struct Node* m_next;
} Node;


struct LinkedList
{
	Node m_head; /* Sentinel */
	Node m_tail; /* Sentinel */
};


/* Validation Functions Declarations: */

static LinkedListResult ValidateNotNullLinkedListPtr(const LinkedList* _list);
static LinkedListResult ValidateNotNullItemPtr(void** _pItem);
static LinkedListResult ValidateNotNullItem(void* _item);
static LinkedListResult ValidateSuccessfulNodeAllocation(const Node* _node);
static LinkedListResult ValidateNotEmptyLinkedList(const LinkedList* _list);


/* Helper Functions Declarations: */

static LinkedList* InitializeNewLinkedList(LinkedList* _list);
static void DestroyAllNodes(LinkedList* _list, void (*_elementDestroy)(void* _item));
static void DestroySingleNode(Node* _node, void (*_elementDestroy)(void* _item));
static Node* CreateNewNode(void* _item);
static void InsertNodeToLinkedList(Node* _nodeToInsert, Node* _referenceNodeFromTheRight);
static void RemoveNodeFromLinkedList(Node* _nodeToRemove);
static void DeleteItemFromLinkedList(Node* _nodeToRemove, void** _pItem);


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


void LinkedListDestroy(LinkedList** _list, void (*_elementDestroy)(void* _item))
{
	if(_list && ValidateNotNullLinkedListPtr(*_list) == LINKEDLIST_SUCCESS)
	{
		DestroyAllNodes(*_list, _elementDestroy);
		free(*_list);
        *_list = NULL;
	}
}


LinkedListResult LinkedListInsertHead(LinkedList* _list, void* _item)
{
	LinkedListResult statusCode;
	Node* newNode = NULL;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	newNode = CreateNewNode(_item);

	if((statusCode = ValidateSuccessfulNodeAllocation(newNode)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	InsertNodeToLinkedList(newNode, _list->m_head.m_next);

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListInsertTail(LinkedList* _list, void* _item)
{
	LinkedListResult statusCode;
	Node* newNode = NULL;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	newNode = CreateNewNode(_item);

	if((statusCode = ValidateSuccessfulNodeAllocation(newNode)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	InsertNodeToLinkedList(newNode, &(_list->m_tail));

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListRemoveHead(LinkedList* _list, void** _pItem)
{
	LinkedListResult statusCode;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS || (statusCode = ValidateNotNullItemPtr(_pItem)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotEmptyLinkedList(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	DeleteItemFromLinkedList(_list->m_head.m_next, _pItem);

	return LINKEDLIST_SUCCESS;
}


LinkedListResult LinkedListRemoveTail(LinkedList* _list, void** _pItem)
{
	LinkedListResult statusCode;

	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != LINKEDLIST_SUCCESS || (statusCode = ValidateNotNullItemPtr(_pItem)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotEmptyLinkedList(_list)) != LINKEDLIST_SUCCESS)
	{
		return statusCode;
	}

	DeleteItemFromLinkedList(_list->m_tail.m_prev, _pItem);

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


static LinkedListResult ValidateNotNullItemPtr(void** _pItem)
{
	return (!_pItem) ? LINKEDLIST_UNINITIALIZED_ERROR : LINKEDLIST_SUCCESS;
}


static LinkedListResult ValidateNotNullItem(void* _item)
{
    return (!_item) ? LINKEDLIST_NULL_ITEM_ERROR : LINKEDLIST_SUCCESS;
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
	_list->m_head.m_item = NULL;
	_list->m_tail.m_item = NULL;

	_list->m_head.m_next = &(_list->m_tail);
	_list->m_head.m_prev = &(_list->m_head); /* Points to itself */

	_list->m_tail.m_next = &(_list->m_tail); /* Points to itself */
	_list->m_tail.m_prev = &(_list->m_head);

	return _list;
}


static void DestroyAllNodes(LinkedList* _list, void (*_elementDestroy)(void* _item))
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


static void DestroySingleNode(Node* _node, void (*_elementDestroy)(void* _item))
{
    if(_elementDestroy)
    {
        _elementDestroy(_node->m_item);
    }

	free(_node);
}


static Node* CreateNewNode(void* _item)
{
	Node* newNode = malloc(sizeof(Node));

	if(!newNode)
	{
		return NULL;
	}

	newNode->m_item = _item;

	return newNode;
}


static void InsertNodeToLinkedList(Node* _nodeToInsert, Node* _referenceNodeFromTheRight /* Reference node "from the right" of the wanted insertion place */)
{
	_nodeToInsert->m_prev = _referenceNodeFromTheRight->m_prev;
	_nodeToInsert->m_next = _referenceNodeFromTheRight;
	_referenceNodeFromTheRight->m_prev->m_next = _nodeToInsert;
	_referenceNodeFromTheRight->m_prev = _nodeToInsert;
}


static void DeleteItemFromLinkedList(Node* _nodeToRemove, void** _pItem)
{
	*_pItem = _nodeToRemove->m_item; /* Extract the item and return it by reference */
	RemoveNodeFromLinkedList(_nodeToRemove);
	DestroySingleNode(_nodeToRemove, NULL);
}


static void RemoveNodeFromLinkedList(Node* _nodeToRemove)
{
	_nodeToRemove->m_next->m_prev = _nodeToRemove->m_prev;
	_nodeToRemove->m_prev->m_next = _nodeToRemove->m_next;
}
