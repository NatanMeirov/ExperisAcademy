/**
 * @file LinkedListInternal.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief LinkedList Internal file to be used as the base to LinkedListIterator and LinkedList - creation and management of a Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * @version 1.0
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdlib.h> /* malloc, free */
#include "LinkedListInternal.h"


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

Node* CreateNewNode(void* _element)
{
	Node* newNode = malloc(sizeof(Node));

	if(!newNode)
	{
		return NULL;
	}

	newNode->m_element = _element;

	return newNode;
}


void DestroySingleNode(Node* _node, void (*_elementDestroy)(void* _element))
{
    if(_elementDestroy)
    {
        _elementDestroy(_node->m_element);
    }

	free(_node);
}


void InsertNodeToLinkedList(Node* _nodeToInsert, Node* _referenceNodeFromTheRight)
{
	_nodeToInsert->m_prev = _referenceNodeFromTheRight->m_prev;
	_nodeToInsert->m_next = _referenceNodeFromTheRight;
	_referenceNodeFromTheRight->m_prev->m_next = _nodeToInsert;
	_referenceNodeFromTheRight->m_prev = _nodeToInsert;
}


void RemoveNodeFromLinkedList(Node* _nodeToRemove)
{
	_nodeToRemove->m_next->m_prev = _nodeToRemove->m_prev;
	_nodeToRemove->m_prev->m_next = _nodeToRemove->m_next;
}

/*------------------------------------------- End of Main API Functions -------------------------------------------*/
