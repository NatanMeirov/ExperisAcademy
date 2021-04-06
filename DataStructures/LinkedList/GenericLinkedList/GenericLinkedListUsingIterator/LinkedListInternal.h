/**
 * @file LinkedListInternal.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief LinkedList Internal file to be used as the base to GenericLinkedList and LinkedListIterator - creation and management of a Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * Using head and tail sentinels
 * Each node is allocated in the heap, and stores a pointer to user proveded element
 * @version 1.0
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __LINKEDLISTINTERNAL_H__
#define __LINKEDLISTINTERNAL_H__

/* Defines: */

typedef struct Node
{
	void* m_element;
	struct Node* m_prev;
	struct Node* m_next;
} Node;

struct LinkedList
{
	Node m_head; /* Head Sentinel */
	Node m_tail; /* Tail Sentinel */
};


/* ----------------------------------------- Main API Functions ------------------------------------ */

/**
 * @brief Creates a new Node from a given element
 * @details Time complexity: O(1)
 * @param[in] _element: An element to create a new Node for
 * @return Node* - A pointer to a new Node, on success / NULL, on failure
 */
Node* CreateNewNode(void* _element);


/**
 * @brief Destroys a single given Node
 * @details Time complexity: O(1)
 * @param[in] _node: A Node to destroy
 * @param[in] _elementDestroy: A function pointer to be used to destroy the Node's element,
 * 			 				   or a NULL if no such destroy is required
 * @return None
 */
void DestroySingleNode(Node* _node, void (*_elementDestroy)(void* _element));


/**
 * @brief Inserts a single Node from a LinkedList by connecting between the reference Node from the right (insertion place) - to the newly inserted Node
 * @details Time complexity: O(1)
 * @param[in] _nodeToInsert: A Node to insert
 * @param[in] _referenceNodeFromTheRight: A reference Node from the right of the wanted insertion place, to insert the given Node to be before this Node in the LinkedList
 * @return None
 */
void InsertNodeToLinkedList(Node* _nodeToInsert, Node* _referenceNodeFromTheRight);


/**
 * @brief Removes a single Node from a LinkedList by connecting between the next and the previous Nodes of the Node to remove
 * @details Time complexity: O(1)
 * @param[in] _nodeToRemove: A Node to remove
 * @return None
 */
void RemoveNodeFromLinkedList(Node* _nodeToRemove);

/* --------------------------------------- End of Main API Functions ---------------------------------- */

#endif /* #ifndef __LINKEDLISTINTERNAL_H__ */