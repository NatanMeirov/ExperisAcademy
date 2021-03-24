/* Binary Search Tree header file */
#ifndef __BINARYSEARCHTREE_H__
#define __BINARYSEARCHTREE_H__

/* Includes: */

#include "ADTDefs.h"

/* Defines: */

typedef struct BinarySearchTree BinarySearchTree;
typedef enum {PRE_ORDER, IN_ORDER, POST_ORDER} BinarySearchTreeTraverse;

/*------------------------------------ Main API Functions: --------------------------------------------*/

/*
- Description: Creates and returns a pointer to a newly created BinarySearchTree
- Input: None
- Output: A pointer to a BinarySearchTree if succeed, else NULL if error has occured
- Errors: [ADTErr] ERR_ALLOCATION_FAILED (if failed to allocate memory for the BinarySearchTree)
*/
BinarySearchTree* BinarySearchTreeCreate();



/*
- Description: Destroys a given BinarySearchTree (releasing its memory), validates no trying of a double attempts to free
- Input: [_tree:] a pointer to a BinarySearchTree
- Output: None
- Errors: [ADTErr] ERR_NOT_INITIALIZED (if pointer is NULL)
*/
void BinarySearchTreeDestroy(BinarySearchTree* _tree);



/*
- Description:
- Input:
- Output:
- Errors:
*/
ADTErr BinarySearchTreeInsert(BinarySearchTree* _tree, int _data);



/*
- Description:
- Input:
- Output:
- Errors:
*/
int BinarySearchTreeIsDataFound(BinarySearchTree* _tree, int _data);



/*
- Description:
- Input:
- Output:
- Errors:
*/
int BinarySearchTreeIsFullTree(BinarySearchTree* _tree);



/*
- Description:
- Input:
- Output:
- Errors:
*/
int BinarySearchTreeAreSimilarTrees(BinarySearchTree* _firstTree, BinarySearchTree* _secondTree);



/*
- Description:
- Input:
- Output:
- Errors:
*/
int BinarySearchTreeCheckTreeLevel(BinarySearchTree* _tree);




/*
- Description:
- Input:
- Output:
- Errors:
*/
void BinarySearchTreeMirrorTree(BinarySearchTree* _tree);




/*
- Description:
- Input:
- Output:
- Errors:
*/
int BinarySearchTreeIsPerfectTree(BinarySearchTree* _tree);




/*
- Description: Prints a given BinarySearchTree in a given traverse mode (pre-order, in-order, post-order)
- Input: [_tree:] a pointer to a BinarySearchTree | [_traverseMode:] a traverse mode to print the BinarySearchTree
- Output: None
- Errors: [ADTErr] ERR_NOT_INITIALIZED (if pointer is NULL), ERR_WRONG_INDEX (if traverse mode is not correct)
*/
void BinarySearchTreePrint(BinarySearchTree* _tree, BinarySearchTreeTraverse _traverseMode);

/*--------------------------------- End of Main API Functions -----------------------------------------*/

#endif /* #ifndef __BINARYSEARCHTREE_H__ */
