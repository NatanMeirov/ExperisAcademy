/* Linked List implementation source file */

/* Includes: */
#include <stdlib.h> /* Used for size_t, malloc, free */
#include "LinkedList.h"
#include "TestLinkedList.h"
#include <stdio.h> /* Used for printf */


/* Defines: */
#define MAGIC_NUM 6872615
#define IS_EMPTY 1
#define IS_NOT_EMPTY 0

typedef struct Node
{
	int m_data;
	struct Node* m_prev;
	struct Node* m_next;
} Node;


struct LinkedList
{
	Node m_head; /* Sentinel */
	Node m_tail; /* Sentinel */
	int m_magicNumber;
};


/* Validation Functions Declarations: */
ADTErr ValidateNotNullLinkedListPtr(LinkedList* _list);
ADTErr ValidateNotNullDataPtr(int* _data);
ADTErr ValidateSuccessfulNodeAllocation(Node* _node);
ADTErr ValidateNotEmptyLinkedList(LinkedList* _list);


/* Helper Functions Declarations: */
LinkedList* InitializeNewLinkedList(LinkedList* _list);
void DestroyAllNodes(LinkedList* _list);
void DestroySingleNode(Node* _node);
Node* CreateNewNode(int _data);
void InsertNodeToLinkedList(Node* _nodeToInsert, Node* _referenceNodeFromTheRight);
void RemoveNodeFromLinkedList(Node* _nodeToRemove);
void DeleteDataItemFromLinkedList(Node* _nodeToRemove, int* _data);


/*--------------------------------------------------------- Main API Functions: ----------------------------------------------------------*/

LinkedList* LinkedListCreate(void)
{
	LinkedList* newList = malloc(sizeof(LinkedList));
	
	if(ValidateNotNullLinkedListPtr(newList) == ERR_NOT_INITIALIZED)
	{
		return NULL; /* ERR_ALLOCATION_FAILED */
	}
	
	return InitializeNewLinkedList(newList);
}


void LinkedListDestroy(LinkedList* _list)
{
	if(ValidateNotNullLinkedListPtr(_list) != ERR_NOT_INITIALIZED && _list->m_magicNumber == MAGIC_NUM)
	{
		DestroyAllNodes(_list);
		_list->m_head.m_next = NULL;
		_list->m_tail.m_prev = NULL;
		_list->m_magicNumber = 0;
		free(_list);
	}
}


ADTErr LinkedListInsertHead(LinkedList* _list, int _data)
{
	ADTErr statusCode;
	Node* newNode = NULL;
	
	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != ERR_OK)
	{
		return statusCode;
	}
	
	newNode = CreateNewNode(_data);
	 
	if((statusCode = ValidateSuccessfulNodeAllocation(newNode)) != ERR_OK)
	{
		return statusCode;
	}
	 
	InsertNodeToLinkedList(newNode, _list->m_head.m_next);
	
	return ERR_OK;
}


ADTErr LinkedListInsertTail(LinkedList* _list, int _data)
{
	ADTErr statusCode;
	Node* newNode = NULL;
	
	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != ERR_OK)
	{
		return statusCode;
	}
	
	newNode = CreateNewNode(_data);
	 
	if((statusCode = ValidateSuccessfulNodeAllocation(newNode)) != ERR_OK)
	{
		return statusCode;
	}
	 
	InsertNodeToLinkedList(newNode, &(_list->m_tail));
	
	return ERR_OK;
}


ADTErr LinkedListRemoveHead(LinkedList* _list, int* _data)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != ERR_OK || (statusCode = ValidateNotNullDataPtr(_data)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotEmptyLinkedList(_list)) != ERR_OK)
	{
		return statusCode;
	}
	
	DeleteDataItemFromLinkedList(_list->m_head.m_next, _data);
	
	return ERR_OK;
}


ADTErr LinkedListRemoveTail(LinkedList* _list, int* _data)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != ERR_OK || (statusCode = ValidateNotNullDataPtr(_data)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotEmptyLinkedList(_list)) != ERR_OK)
	{
		return statusCode;
	}
	
	DeleteDataItemFromLinkedList(_list->m_tail.m_prev, _data);
	
	return ERR_OK;
}


size_t LinkedListCountItems(LinkedList* _list)
{
	size_t nodesCounter = 0;
	ADTErr statusCode;
	Node* currentNode = _list->m_head.m_next;
	
	if((statusCode = ValidateNotNullLinkedListPtr(_list)) != ERR_OK)
	{
		return statusCode;
	}
	
	while(currentNode != &(_list->m_tail))
	{
		nodesCounter++;
		currentNode = currentNode->m_next;
	}
	
	return nodesCounter;
}


int LinkedListIsEmpty(LinkedList* _list)
{
	ADTErr statusCode;
	
	if(ValidateNotNullLinkedListPtr(_list) != ERR_OK)
	{
		return -1;
	}
	
	return (_list->m_head.m_next == &(_list->m_tail)) ? IS_EMPTY : IS_NOT_EMPTY;
}


/*------------------------------------------------------ End of Main API Function --------------------------------------------------------*/


/* Validation Functions: */
ADTErr ValidateNotNullLinkedListPtr(LinkedList* _list)
{
	return (!_list) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotNullDataPtr(int* _data)
{
	return (!_data) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateSuccessfulNodeAllocation(Node* _node)
{
	return (!_node) ? ERR_ALLOCATION_FAILED : ERR_OK;
}


ADTErr ValidateNotEmptyLinkedList(LinkedList* _list)
{
	return (_list->m_head.m_next != &(_list->m_tail)) ? ERR_OK : ERR_UNDERFLOW;
}


/* Helper Functions: */
LinkedList* InitializeNewLinkedList(LinkedList* _list)
{
	_list->m_head.m_data = 0;
	_list->m_tail.m_data = 0;
	
	_list->m_head.m_next = &(_list->m_tail);
	_list->m_head.m_prev = NULL;
	
	_list->m_tail.m_next = NULL;
	_list->m_tail.m_prev = &(_list->m_head);
		
	_list->m_magicNumber = MAGIC_NUM;

	return _list;
}


void DestroyAllNodes(LinkedList* _list)
{
	Node* tempNextNodePtr = NULL;
	Node* currentNode = _list->m_head.m_next;
	
	while(currentNode != &(_list->m_tail)) /* While did not reach the tail of the Linked List */
	{
		tempNextNodePtr = currentNode->m_next;
		DestroySingleNode(currentNode);
		currentNode = tempNextNodePtr;
	}
}


void DestroySingleNode(Node* _node)
{
	free(_node);
}


Node* CreateNewNode(int _data)
{
	Node* newNode = malloc(sizeof(Node));
	
	if(!newNode)
	{
		return NULL;
	}
	
	newNode->m_data = _data;
	
	return newNode;
}


void InsertNodeToLinkedList(Node* _nodeToInsert, Node* _referenceNodeFromTheRight /* Reference node "from the right" of the wanted insertion place */)
{
	_nodeToInsert->m_prev = _referenceNodeFromTheRight->m_prev;
	_nodeToInsert->m_next = _referenceNodeFromTheRight;
	_referenceNodeFromTheRight->m_prev->m_next = _nodeToInsert;
	_referenceNodeFromTheRight->m_prev = _nodeToInsert;
}


void DeleteDataItemFromLinkedList(Node* _nodeToRemove, int* _data)
{
	*_data = _nodeToRemove->m_data; /* Extract the data item and return it by reference */
	RemoveNodeFromLinkedList(_nodeToRemove);
	DestroySingleNode(_nodeToRemove);
}


void RemoveNodeFromLinkedList(Node* _nodeToRemove)
{
	_nodeToRemove->m_next->m_prev = _nodeToRemove->m_prev;
	_nodeToRemove->m_prev->m_next = _nodeToRemove->m_next;
}


/* Testing Functions: */

Node* GetLinkedListHeadSentinel(LinkedList* _list)
{
	return &(_list->m_head);
}


Node* GetLinkedListTailSentinel(LinkedList* _list)
{
	return &(_list->m_tail);
}


Node* GetNextNode(Node* _currentNode)
{
	return _currentNode->m_next;
}


Node* GetPreviousNode(Node* _currentNode)
{
	return _currentNode->m_prev;
}


int GetDataItemFromNode(Node* _node)
{
	return _node->m_data;
}


void PrintLinkedList(LinkedList* _list)
{
	Node* currentNode = _list->m_head.m_next;
	
	while(currentNode != &(_list->m_tail))
	{
		if(currentNode->m_next != &(_list->m_tail))
		{
			printf("[%d] ---> ", currentNode->m_data);
		}
		else /* Reached the Tail Sentinel - end of the LinkedList */
		{
			printf("[%d] ---> [NULL]", currentNode->m_data);
		}
		
		currentNode = currentNode->m_next;
	}
}


