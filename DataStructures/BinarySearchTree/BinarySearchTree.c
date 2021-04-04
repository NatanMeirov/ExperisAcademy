/* Includes: */

#include <stdlib.h> /* malloc, free, size_t, NULL */
#include <stdio.h> /* printf */
#include "ADTDefs.h"
#include "BinarySearchTree.h"

/* Macros: */

#define MAGIC_NUM 4725314
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct Node
{
    int m_data;
    struct Node* m_left;
    struct Node* m_right;
    struct Node* m_father;
} Node;

struct BinarySearchTree
{
    Node* m_root;
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static ADTErr ValidateNotNullTree(BinarySearchTree* _tree);
static ADTErr ValidateNotNullNode(Node* _node);


/* Helper Functions Declarations: */

static BinarySearchTree* InitializeNewBinarySearchTree(BinarySearchTree* _tree);
static Node* CreateNewNode(Node* _fatherNode, int _data);
static void DestroySubTree(Node* _currentNode);
static void DestroySingleNode(Node* _node);
static Node* FindPlaceForNodeByData(Node* _node, int _data);
static Node* FindNodeByData(Node* _node, int _data);
static int IsFullTree(Node* _node);
static int AreSimilarTrees(Node* _firstTreeNode, Node* _secondTreeNode);
static int CheckTreeLevel(Node* _node);
static void MirrorTree(Node* _node);
static int IsPerfectTree(Node* _node);
static void SwapNodes(Node** _firstNode, Node** _secondNode);


/*------------------------------------ Main API Functions: --------------------------------------------*/

BinarySearchTree* BinarySearchTreeCreate()
{
    BinarySearchTree* newTree = NULL;

    newTree = malloc(sizeof(BinarySearchTree));

    if(ValidateNotNullTree(newTree) == ERR_NOT_INITIALIZED)
    {
        return NULL; /* ERR_ALLOCATION_FAILED */
    }

    return InitializeNewBinarySearchTree(newTree);
}


void BinarySearchTreeDestroy(BinarySearchTree* _tree)
{
    if(ValidateNotNullTree(_tree) == ERR_OK && _tree->m_magicNumber == MAGIC_NUM)
    {
        if(ValidateNotNullNode(_tree->m_root) == ERR_OK)
        {
            DestroySubTree(_tree->m_root);
            _tree->m_root = NULL;
        }

        _tree->m_magicNumber = 0;
        free(_tree);
    }
}


ADTErr BinarySearchTreeInsert(BinarySearchTree* _tree, int _data)
{
    ADTErr statusCode;
    Node* newNode = NULL;
    Node* fatherNode = NULL;

    if((statusCode = ValidateNotNullTree(_tree)) != ERR_OK)
    {
        return statusCode;
    }

    if(!_tree->m_root) /* The root is still NULL - the tree is empty */
    {
        _tree->m_root = CreateNewNode(NULL, _data);
        statusCode = ValidateNotNullNode(_tree->m_root);

        return statusCode; /* ERR_OK status if succeed, else error status */
    }

    fatherNode = FindPlaceForNodeByData(_tree->m_root, _data);

    if(ValidateNotNullNode(fatherNode) != ERR_OK)
    {
        return ERR_DUPLICATE;
    }

    newNode = CreateNewNode(fatherNode, _data);

    if(fatherNode->m_data > newNode->m_data)
    {
        fatherNode->m_left = newNode;
    }
    else /* fatherNode->m_data < newNode->m_data */
    {
        fatherNode->m_right = newNode;
    }

    return ERR_OK;
}


int BinarySearchTreeIsDataFound(BinarySearchTree* _tree, int _data)
{
    if(ValidateNotNullTree(_tree) != ERR_OK)
    {
        return -1;
    }

    if(FindNodeByData(_tree->m_root, _data))
    {
        return 1; /* If not NULL - Data was found */
    }

    return 0;
}


int BinarySearchTreeIsFullTree(BinarySearchTree* _tree)
{
    if(ValidateNotNullTree(_tree) != ERR_OK)
    {
        return -1;
    }

    if(ValidateNotNullNode(_tree->m_root) != ERR_OK)
    {
        return -1;
    }

    return IsFullTree(_tree->m_root);
}


int BinarySearchTreeAreSimilarTrees(BinarySearchTree* _firstTree, BinarySearchTree* _secondTree)
{
    if(ValidateNotNullTree(_firstTree) != ERR_OK || ValidateNotNullTree(_secondTree) != ERR_OK)
    {
        return -1;
    }

    if(ValidateNotNullNode(_firstTree->m_root) != ERR_OK || ValidateNotNullNode(_secondTree->m_root) != ERR_OK)
    {
        return -1;
    }

    return AreSimilarTrees(_firstTree->m_root, _secondTree->m_root);
}


int BinarySearchTreeCheckTreeLevel(BinarySearchTree* _tree)
{
    if(ValidateNotNullTree(_tree) != ERR_OK)
    {
        return -1;
    }

    if(ValidateNotNullNode(_tree->m_root) != ERR_OK)
    {
        return -1;
    }

    return CheckTreeLevel(_tree->m_root);
}


void BinarySearchTreeMirrorTree(BinarySearchTree* _tree)
{
    if(ValidateNotNullTree(_tree) != ERR_OK)
    {
        return;
    }

    if(ValidateNotNullNode(_tree->m_root) != ERR_OK)
    {
        return;
    }

    MirrorTree(_tree->m_root);
}


int BinarySearchTreeIsPerfectTree(BinarySearchTree* _tree)
{
    if(ValidateNotNullTree(_tree) != ERR_OK)
    {
        return -1;
    }

    if(ValidateNotNullNode(_tree->m_root) != ERR_OK)
    {
        return -1;
    }

    return IsPerfectTree(_tree->m_root);
}


void BinarySearchTreePrint(BinarySearchTree* _tree, BinarySearchTreeTraverse _traverseMode)
{
    if(ValidateNotNullTree(_tree) != ERR_OK)
    {
        return;
    }

    /*TODO - validate correct _traverseMode, and then implement 3 diff recursive functions */
}


/*--------------------------------- End of Main API Functions -----------------------------------------*/


/* Validation Functions: */

static ADTErr ValidateNotNullTree(BinarySearchTree* _tree)
{
    return (!_tree) ? ERR_NOT_INITIALIZED : ERR_OK;
}


static ADTErr ValidateNotNullNode(Node* _node)
{
    return (!_node) ? ERR_NOT_INITIALIZED : ERR_OK;
}


/* Helper Functions: */

static BinarySearchTree* InitializeNewBinarySearchTree(BinarySearchTree* _tree)
{
    _tree->m_root = NULL;
    _tree->m_magicNumber = MAGIC_NUM;

    return _tree;
}


static Node* CreateNewNode(Node* _fatherNode, int _data)
{
    Node* newNode = malloc(sizeof(Node));

    if(ValidateNotNullNode(newNode) == ERR_NOT_INITIALIZED)
    {
        return NULL; /* ERR_ALLOCATION_FAILED */
    }

    newNode->m_data = _data;
    newNode->m_father = _fatherNode;
    newNode->m_left = NULL; /* A new Node will always be a leaf */
    newNode->m_right = NULL; /* A new Node will always be a leaf */

    return newNode;
}

/* Another approach (One less level deeper - but less clean code):
static void DestroySubTree(Node* _currentNode)
{
    if(_currentNode->m_left)
    {
        DestroySubTree(_currentNode->m_left);
    }

    if(_currentNode->m_right)
    {
        DestroySubTree(_currentNode->m_right);
    }

    DestroySingleNode(_currentNode);
}
*/


static void DestroySubTree(Node* _currentNode)
{
    if(!_currentNode)
    {
        return;
    }

    DestroySubTree(_currentNode->m_left);
    DestroySubTree(_currentNode->m_right);
    DestroySingleNode(_currentNode);
}


static void DestroySingleNode(Node* _node)
{
    free(_node);
}

static Node* FindPlaceForNodeByData(Node* _node, int _data)
{
    if(_node->m_data == _data)
    {
        return NULL;
    }

    if(_node->m_data > _data)
    {
        if(_node->m_left)
        {
            return FindPlaceForNodeByData(_node->m_left, _data);
        }
        else /* No left son: Found the correct place to insert (left son of current node) */
        {
            return _node;
        }
    }
    else /* _node->m_data < _data */
    {
        if(_node->m_right)
        {
            return FindPlaceForNodeByData(_node->m_right, _data);
        }
        else /* No right son: Found the correct place to insert (right son of current node) */
        {
            return _node;
        }
    }
}

static Node* FindNodeByData(Node* _node, int _data)
{
    if(!_node)
    {
        return NULL;
    }

    if(_node->m_data == _data)
    {
        return _node;
    }

    if(_node->m_data > _data)
    {
        return FindNodeByData(_node->m_left, _data);
    }
    else /* _node->m_data < _data */
    {
        return FindNodeByData(_node->m_right, _data);
    }
}


static int IsFullTree(Node* _node)
{
    if(!_node->m_left && !_node->m_right)
    {
        return 1;
    }

    if(!_node->m_left ||  !_node->m_right)
    {
        return 0;
    }

    return IsFullTree(_node->m_left) && IsFullTree(_node->m_right);
}


static int AreSimilarTrees(Node* _firstTreeNode, Node* _secondTreeNode)
{
    if(!_firstTreeNode && !_secondTreeNode)
    {
        return 1;
    }

    if(!_firstTreeNode || !_secondTreeNode)
    {
        return 0;
    }

    return AreSimilarTrees(_firstTreeNode->m_left, _secondTreeNode->m_left) && AreSimilarTrees(_firstTreeNode->m_right, _secondTreeNode->m_right);
}


static int CheckTreeLevel(Node* _node)
{
    if(!_node) /* Reached to a leaf - height = 0 */
    {
        return -1;
    }

    return 1 + MAX(CheckTreeLevel(_node->m_left), CheckTreeLevel(_node->m_right));
}


static void MirrorTree(Node* _node)
{
    if(!_node)
    {
        return;
    }

    MirrorTree(_node->m_left);
    MirrorTree(_node->m_right);

    SwapNodes(&(_node->m_left), &(_node->m_right));
}


static int IsPerfectTree(Node* _node)
{
    /* TODO */

    return 0;
}


static void SwapNodes(Node** _firstNode, Node** _secondNode)
{
    Node* tempNode = *_firstNode;
    *_firstNode = *_secondNode;
    *_secondNode = tempNode;
}


/* Test Functions: */


