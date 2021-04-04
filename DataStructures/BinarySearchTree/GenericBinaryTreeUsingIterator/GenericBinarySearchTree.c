/**
 * @file GenericBinarySearchTree.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Binary Search Tree data type that stores pointer to user provided elements of generic type
 * The BST is implemented using a sentinel
 * The first Node in the BST (root) is the sentinel's left Subtree (left son)
 * Supports BinarySearchTreeIterator functionality
 * @version 1.0
 * @date 2021-03-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */


 /* Includes: */

#include <stdlib.h> /* malloc, free*/
#include "GenericBinarySearchTree.h"


/* Defines: */

typedef struct Node
{
    void* m_element;
    struct Node* m_father;
    struct Node* m_left;
    struct Node* m_right;
} Node;

struct BinarySearchTree
{
    Node m_root; /* Sentinel */
    LessThanComparator m_ptrFuncLessThen;
    EqualityFunction m_ptrFuncEqualTo;
};


/* Helper Functions Declarations: */

static BinarySearchTree* InitializeNewBinarySearchTree(BinarySearchTree* _tree, LessThanComparator _ptrFuncLessThen, EqualityFunction _ptrFuncEqualTo);
static void DestroySubTree(Node* _currentNode, void (*_elementDestroy)(void*));
static void DestroySingleNode(Node* _node, void (*_elementDestroy)(void*));
static Node* FindBinarySearchTreeMostLeftElement(Node* _node);
static Node* FindBinarySearchTreeMostRightElement(Node* _node);
static int IsEndSentinel(Node* _node);
static Node* CreateNewNode(Node* _fatherNode, void* _item);
static Node* FindPlaceForNewNode(Node* _node, void* _element, LessThanComparator _ptrFuncLessThen, EqualityFunction _ptrFuncEqualTo);
static void PlaceSonNodeToFatherNode(Node* _fatherNode, Node* _sonNode, LessThanComparator _ptrFuncLessThen);


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

BinarySearchTree* BinarySearchTreeCreate(LessThanComparator _ptrFuncLessThen, EqualityFunction _ptrFuncEqualTo)
{
    BinarySearchTree* newTree = NULL;

    if(!_ptrFuncLessThen || !_ptrFuncEqualTo)
    {
        return NULL;
    }

    newTree = malloc(sizeof(BinarySearchTree));

    if(!newTree)
    {
        return NULL;
    }

    return InitializeNewBinarySearchTree(newTree, _ptrFuncLessThen, _ptrFuncEqualTo);
}


void BinarySearchTreeDestroy(BinarySearchTree** _tree, void (*_elementDestroy)(void*))
{
    if(_tree && *_tree)
    {
        DestroySubTree((*_tree)->m_root.m_left, _elementDestroy); /* If no left son - will do nothing */
        free(*_tree);
        *_tree = NULL;
    }
}


BinarySearchTreeIterator BinarySearchTreeIteratorBegin(const BinarySearchTree* _tree)
{
    if(!_tree)
    {
        return NULL;
    }

    if(!_tree->m_root.m_left) /* Empty BST */
    {
        return (BinarySearchTreeIterator)&_tree->m_root; /* Returning itself (the sentinel) -> end iterator */
    }

    return (BinarySearchTreeIterator)FindBinarySearchTreeMostLeftElement(_tree->m_root.m_left);
}


BinarySearchTreeIterator BinarySearchTreeIteratorEnd(const BinarySearchTree* _tree)
{
    if(!_tree)
    {
        return NULL;
    }

    return (BinarySearchTreeIterator)&_tree->m_root; /* Returning itself (the sentinel) */
}


int BinarySearchTreeIteratorEquals(const BinarySearchTreeIterator _firstIterator, const BinarySearchTreeIterator _secondIterator)
{
    return _firstIterator == _secondIterator; /* Comparing addresses - will work even if one of them is NULL */
}


BinarySearchTreeIterator BinarySearchTreeIteratorNext(BinarySearchTreeIterator _iterator)
{
    /*TODO - have to return the next item -> can be: -> father -> left -> left .... to left (to find the exact one node after) */
}


BinarySearchTreeIterator BinarySearchTreeIteratorPrev(BinarySearchTreeIterator _iterator)
{
    /*TODO - the opposite of the next*/

}


BinarySearchTreeIterator BinarySearchTreeInsert(BinarySearchTree* _tree, void* _element)
{
    Node* fatherNode = NULL;
    Node* newNode = NULL;

    if(!_tree)
    {
        return NULL;
    }

    if(!_element)
    {
        return (BinarySearchTreeIterator)&(_tree->m_root); /* Returning the sentinel -> end iterator */
    }

    if(!_tree->m_root.m_left) /* The root->left is still NULL - the BST is empty */
    {
        _tree->m_root.m_left = CreateNewNode(&_tree->m_root, _element);
        if(!_tree->m_root.m_left) /* Error has occurred while trying to allocate memory for the new Node */
        {
            return (BinarySearchTreeIterator)&_tree->m_root; /* Returning the sentinel -> end iterator */
        }
    }

    fatherNode = FindPlaceForNewNode(_tree->m_root.m_left, _element, _tree->m_ptrFuncLessThen, _tree->m_ptrFuncEqualTo); /* Finding the father of the new node */

    if(!fatherNode) /* Error has occurred while duplicate key was found in the BST */
    {
        return (BinarySearchTreeIterator)&_tree->m_root; /* Returning the sentinel -> end iterator */
    }

    newNode = CreateNewNode(fatherNode, _element);
    if(!newNode) /* Error has occurred while trying to allocate memory for the new Node */
    {
        return (BinarySearchTreeIterator)&_tree->m_root; /* Returning the sentinel -> end iterator */
    }

    PlaceSonNodeToFatherNode(fatherNode, newNode, _tree->m_ptrFuncLessThen);

    return (BinarySearchTreeIterator)newNode;
}


void* BinarySearchTreeIteratorRemove(BinarySearchTreeIterator _iterator)
{
    /*TODO - need to change the tree after the remove + prevent not removing the end sentinel! */
}


void* BinarySearchTreeIteratorGet(BinarySearchTreeIterator _iterator)
{
    if(!_iterator)
    {
        return NULL;
    }

    return ((Node*)_iterator)->m_element; /* Will be NULL if iterator points to the end (the sentinel), because its element is initialized to be NULL */
}

/*------------------------------------------- End of Main API Functions -------------------------------------------*/


/* Helper Functions: */

static BinarySearchTree* InitializeNewBinarySearchTree(BinarySearchTree* _tree, LessThanComparator _ptrFuncLessThen, EqualityFunction _ptrFuncEqualTo)
{
    _tree->m_root.m_element = NULL;
    _tree->m_root.m_right = NULL;

    _tree->m_root.m_father = &(_tree->m_root); /* Pointing on self */
    _tree->m_root.m_left = NULL; /* The whole BST will start from here */

    _tree->m_ptrFuncLessThen = _ptrFuncLessThen;
    _tree->m_ptrFuncEqualTo = _ptrFuncEqualTo;

    return _tree;
}


static void DestroySubTree(Node* _currentNode, void (*_elementDestroy)(void*))
{
    if(!_currentNode)
    {
        return;
    }

    DestroySubTree(_currentNode->m_left, _elementDestroy);
    DestroySubTree(_currentNode->m_right, _elementDestroy);
    DestroySingleNode(_currentNode, _elementDestroy);
}


static void DestroySingleNode(Node* _node, void (*_elementDestroy)(void*))
{
    if(_elementDestroy)
    {
        _elementDestroy(_node->m_element);
    }

    free(_node);
}


static Node* FindBinarySearchTreeMostLeftElement(Node* _node)
{
    if(!_node->m_left) /* No more left sons in the subtree */
    {
        return _node;
    }

    return FindBinarySearchTreeMostLeftElement(_node->m_left);
}


static Node* FindBinarySearchTreeMostRightElement(Node* _node)
{
    if(!_node->m_right) /* No more right sons in the subtree */
    {
        return _node;
    }

    return FindBinarySearchTreeMostRightElement(_node->m_right);
}


static int IsEndSentinel(Node* _node)
{
    return _node == _node->m_father;
}


static Node* CreateNewNode(Node* _fatherNode, void* _element)
{
    Node* newNode = malloc(sizeof(Node));

    if(!newNode)
    {
        return NULL;
    }

    newNode->m_element = _element;
    newNode->m_father = _fatherNode;
    newNode->m_left = NULL; /* A new Node will always be a leaf */
    newNode->m_right = NULL; /* A new Node will always be a leaf */

    return newNode;
}


static Node* FindPlaceForNewNode(Node* _node, void* _element, LessThanComparator _ptrFuncLessThen, EqualityFunction _ptrFuncEqualTo)
{
    if(_ptrFuncEqualTo(_node->m_element, _element))
    {
        return NULL; /* Duplicate error */
    }

    if(_ptrFuncLessThen(_element, _node->m_element)) /* Go left */
    {
        if(_node->m_left)
        {
            return FindPlaceForNewNode(_node->m_left, _element, _ptrFuncLessThen, _ptrFuncEqualTo);
        }

        /* If reached here: No left son - Found the correct place to insert (left son of the current node) */
    }
    else /* Go right */
    {
        if(_node->m_right)
        {
            return FindPlaceForNewNode(_node->m_right, _element, _ptrFuncLessThen, _ptrFuncEqualTo);
        }

        /* If reached here: No right son - Found the correct place to insert (right son of the current node) */
    }

    return _node;
}


static void PlaceSonNodeToFatherNode(Node* _fatherNode, Node* _sonNode, LessThanComparator _ptrFuncLessThen)
{
    if(_ptrFuncLessThen(_sonNode->m_element, _fatherNode->m_element)) /* _ptrFuncLessThen(_sonNode->m_element, _fatherNode->m_element) == 1 : _sonNode->m_element < _fatherNode->m_element */
    {
        _fatherNode->m_left = _sonNode;
    }
    else /* _ptrFuncLessThen(_ptrFuncLessThen(_sonNode->m_element, _fatherNode->m_element) == 0 : _sonNode->m_element > _fatherNode->m_element */
    {
        _fatherNode->m_right = _sonNode;
    }
}