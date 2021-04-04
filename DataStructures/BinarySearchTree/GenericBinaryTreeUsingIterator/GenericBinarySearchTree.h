/**
 * @file GenericBinarySearchTree.h
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


#ifndef __GENERICBINARYSEARCHTREE_H__
#define __GENERICBINARYSEARCHTREE_H__


/* Defines: */

typedef struct BinarySearchTree BinarySearchTree;
typedef void*  BinarySearchTreeIterator;

/**
 * @brief A comparison function that returns true (none zero value) if a < b, or false (zero) otherwise
 * 
 */
typedef int (*LessThanComparator)(void* _left, void* _right);

/**
 * @brief A comparison function that returns true (none zero value) if a == b, or false (zero) otherwise
 * 
 */
typedef int (*EqualityFunction)(void* _left, void* _right);

/** 
 * @brief Predicate function returns a none zero value if predicate holds for element
 *
 * @param[in] _element: element to test
 * @param[in] _context: context to be used
 * @return none zero if predicate holds
 */
typedef int (*PredicateFunction)(void * _element, void* _context);

/** 
 * @brief Action function to operate on an element
 *
 * @param[in] _element: element to test
 * @param[in] _context: context to be used
 * @return zero if action fails
 */
typedef int (*ActionFunction)(void* _element, void* _context);

typedef enum BinarySearchTreeTraversalMode
{
    BINARYSEARCHTREE_TRAVERSAL_PREORDER = 0,
    BINARYSEARCHTREE_TRAVERSAL_INORDER,
    BINARYSEARCHTREE_TRAVERSAL_POSTORDER
} BinarySearchTreeTraversalMode;


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

/** 
 * @brief Creates an empty binary search tree with a sentinel
 * @param[in] _ptrFuncLessThen: A comparison function
 *				that returns true (none zero value) if a < b
 *				or false (zero) otherwise
 * @param[in] _ptrFuncEqualTo: A comparison function
 *				that returns true (none zero value) if a == b
 *				or false (zero) otherwise
 * @return a pointer to the newly created tree
 * @retval NULL on failure due to allocation failure or NULL function pointer given
 */
BinarySearchTree* BinarySearchTreeCreate(LessThanComparator _ptrFuncLessThen, EqualityFunction _ptrFuncEqualTo);


/** 
 * @brief Destroy tree
 * Destroys the tree, freeing the memory.
 * If supplied with non-NULL destroyer function, frees items in tree.
 * Average time complexity: O(1)..O(n) (depends on memory freeing)
 *
 * @param[in] _tree: A previously created Tree ADT returned via BinarySearchTreeCreate
 * @param[in] _elementDestroy: A function to destroy the data in the tree (may be NULL if unnecessary)
 */
void BinarySearchTreeDestroy(BinarySearchTree** _tree, void (*_elementDestroy)(void*));


/** 
 * @brief Get an in-order iterator to the tree's begin 
 *
 * @param[in] _tree: tree to create iterator from
 * @return an iterator pointing at the tree's begin or end if tree is empty 
 */
BinarySearchTreeIterator BinarySearchTreeIteratorBegin(const BinarySearchTree* _tree);


/** 
 * @brief Get iterator to the tree's end (in order)
 *
 * @param[in] _tree: tree to create iterator from
 * @return an iterator pointing at the tree's end
 */
BinarySearchTreeIterator BinarySearchTreeIteratorEnd(const BinarySearchTree* _tree);


/** 
 * @brief Compare two iterators
 *
 * @param[in] _firstIterator: Iterators to compare
 * @param[in] _secondIterator: Iterators to compare
 * @return : 0 if are not the same, 1 if iterators are the same
 */
int BinarySearchTreeIteratorEquals(const BinarySearchTreeIterator _firstIterator, const BinarySearchTreeIterator _secondIterator);


/** 
 * @brief Get iterator to the next element from current iterator
 *
 * @param[in] _iterator: A tree iterator
 * @returns an iterator pointing at the next element after _iterator or end iterator.
 */
BinarySearchTreeIterator BinarySearchTreeIteratorNext(BinarySearchTreeIterator _iterator);


/** 
 * @brief Get iterator to the previous element from current iterator
 *
 * @param _iterator: A tree iterator
 * @return an iterator pointing at the previous element
 * or to the beginning if _iterator points to the beginning
 */
BinarySearchTreeIterator BinarySearchTreeIteratorPrev(BinarySearchTreeIterator _iterator);


/** 
 * @brief Inserts an element to a given tree if it's not already there
 * Insert element to binary tree, using the tree's comparison function
 * Average time complexity O(log n).
 * Worst: O(n)
 *
 * @param[in] _tree: A previously created Tree ADT returned via BinarySearchTreeCreate
 * @param[in] _element: An element to insert to the tree
 * @return iterator pointing to the item added or iterator to end on failure (if _tree is not NULL, else NULL will be returned)
 * @warning don't allow duplicates
 */
BinarySearchTreeIterator BinarySearchTreeInsert(BinarySearchTree* _tree, void* _element);


/** 
 * @brief Removes element from tree
 * @details Remove element pointed to by _it and rearranges the tree so that it maintains binary search tree arrangement
 * Average/Worst time complexity O(1).
 *
 * @param[in] _iterator: A tree iterator
 * @return removed item or NULL on failure
 */
void* BinarySearchTreeIteratorRemove(BinarySearchTreeIterator _iterator);


/** 
 * @brief Get element pointed to by iterator from tree
 * Average/Worst time complexity O(1).
 *
 * @param[in] _iterator: A tree iterator
 * @return the item the iterator points at or NULL if _iterator as at end
 */
void* BinarySearchTreeIteratorGet(BinarySearchTreeIterator _iterator);


/** 
 * @brief Search the first element for which the given predicate returns 0
 * iterating using in-order manner over elements
 * O(k * n) where O(k) is the time complexity of the predicate
 *
 * @param[in] _tree: A previously created Tree ADT returned via BinarySearchTreeCreate
 * @param[in] _predicate: Predicate function
 * @param[in] _params: Predicate parameters
 * @return an iterator pointing to the first data found, to end of tree if not found or NULL on NULL input
 */
/* BinarySearchTreeIterator BinarySearchTreeFindFirst(const BinarySearchTree* _tree, PredicateFunction _predicate, void* _context); - NO NEED TO IMPLEMENT FOR NOW */


/** 
 * @brief Performs an action function on every element in tree, by given traversal mode
 * @details iteration will on the first element for which action returns a zero
 * or on reaching end of the container
 *
 * @param[in] _tree: tree to iterate over
 * @param[in] _mode: Traversal mode - BINARYSEARCHTREE_TRAVERSAL_PREORDER, BINARYSEARCHTREE_TRAVERSAL_INORDER, BINARYSEARCHTREE_TRAVERSAL_POSTORDER
 * @param[in] _action: Action function to call for each element
 * @param[in] _context: Parameters for the function
 * @return Iterator to end or an iterator to the specific element where action returned none zero value
 */
/* BinarySearchTreeIterator BinarySearchTree_ForEach(const BinarySearchTree* _tree, BinarySearchTreeTraversalMode _mode, ActionFunction _action, void* _context); - NO NEED TO IMPLEMENT FOR NOW */

/*------------------------------------------- End of Main API Functions -------------------------------------------*/

#endif /* #ifndef __GENERICBINARYSEARCHTREE_H__ */
