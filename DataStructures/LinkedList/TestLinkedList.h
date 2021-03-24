/* Test Linked List header file */

#ifndef __TESTLINKEDLIST_H__
#define __TESTLINKEDLIST_H__

#include "LinkedList.h"

typedef struct Node NodeType;


/* Getters: */

NodeType* GetLinkedListHeadSentinel(LinkedList* _list);

NodeType* GetLinkedListTailSentinel(LinkedList* _list);

NodeType* GetNextNode(NodeType* _currentNode);

NodeType* GetPreviousNode(NodeType* _currentNode);

int GetDataItemFromNode(NodeType* _node);

/* Print: */

void PrintLinkedList(LinkedList* _list);


#endif /* #ifndef __TESTLINKEDLIST_H__ */
