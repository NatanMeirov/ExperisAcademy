/**
 * @file GenericVector.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Vector data type that stores pointer to user provided elements of generic type
 * The Vector is heap allocated and can grow and shrink on demand
 * In version 2.0: Performance improvements
 * @version 2.0
 * @date 2021-03-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */


 /* Includes: */

#include "Vector.hpp"
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, realloc, free */


/* Defines: */

#define SHRINKING_FACTOR 2


/* Validation Function Declarations: */

static Vector::Result ValidateNotOutOfBoundsIndex(const Vector* a_vector, size_t _index);
static Vector::Result ValidateNotOverflowAfterAdd(const Vector* a_vector);


/* Helper Function Declaration: */

static Vector::Result TryAddMoreMemoryToVector(Vector* a_vector);
static void AddNewItemToEndOfVector(Vector* a_vector, void* _itemToAdd);
static Vector::Result TryShrinkVector(Vector* a_vector);
static Vector::Result ShrinkVector(Vector* a_vector);
static void DeleteLastItemFromEndOfVector(Vector* a_vector, void** _itemToDelete);


/* -------------------------------------- Main API Functions ------------------------------------ */

Vector::Vector(size_t a_initialCapacity, size_t a_extensionBlockSize, void (*a_elementDestroyer)(void* a_element))
: m_vectorItems((void**)malloc(a_initialCapacity * sizeof(void*)))
, m_originalCapacity(a_initialCapacity)
, m_vectorCapacity(a_initialCapacity)
, m_extensionBlockSize(a_extensionBlockSize)
, m_sizeOfVector(0)
,m_elementDestroyer(a_elementDestroyer) {
    if((!a_initialCapacity && !a_extensionBlockSize) || !this->m_vectorItems) {
        exit(1);
    }
}


Vector::~Vector() {
    // Handling each element (if deallocation is needed)
    if(this->m_elementDestroyer) {
        for(size_t i = 0; i < this->m_sizeOfVector; i++) {
            this->m_elementDestroyer(this->m_vectorItems[i]);
        }
    }

    free(this->m_vectorItems);
    this->m_vectorItems = NULL;
}


Vector::Result Vector::Append(void* a_element) {
    Vector::Result statusCode;
    if((statusCode = ValidateNotOverflowAfterAdd(this)) != Vector::VECTOR_SUCCESS) {
		return statusCode;
	}

    if(this->m_sizeOfVector == this->m_vectorCapacity) {
        /* Vector reached its maximum capacity */
		if((statusCode = TryAddMoreMemoryToVector(this)) != Vector::VECTOR_SUCCESS) {
			return statusCode;
		}
		/* Else: reallocation completed successfully - ready to Append the item to the vector */
	}

	AddNewItemToEndOfVector(this, a_element);

	return Vector::VECTOR_SUCCESS;
}


Vector::Result Vector::Remove(void** a_pElement) {
    if(!a_pElement) {
		return Vector::VECTOR_UNINITIALIZED_ERROR;
	}

    if(this->m_sizeOfVector == 0) {
		return Vector::VECTOR_UNDERFLOW_ERROR;
	}

	DeleteLastItemFromEndOfVector(this, a_pElement);

	TryShrinkVector(this); /* Currently, do nothing if an error has occurred while tried to reallocate memory */

	return Vector::VECTOR_SUCCESS;
}


Vector::Result Vector::Get(size_t _index, void** a_pElement) {
    if(!a_pElement) {
		return Vector::VECTOR_UNINITIALIZED_ERROR;
	}

    Vector::Result  statusCode;
    if((statusCode = ValidateNotOutOfBoundsIndex(this, _index)) != Vector::VECTOR_SUCCESS) {
		return statusCode;
	}

	*a_pElement = this->m_vectorItems[_index];

	return VECTOR_SUCCESS;
}


Vector::Result Vector::Set(size_t _index, void* a_element) {
    Vector::Result statusCode;
    if((statusCode = ValidateNotOutOfBoundsIndex(this, _index)) != Vector::VECTOR_SUCCESS) {
		return statusCode;
	}

	this->m_vectorItems[_index] = a_element;

	return Vector::VECTOR_SUCCESS;
}


size_t Vector::Size() {
    return this->m_sizeOfVector;
}


size_t Vector::Capacity() {
    return this->m_vectorCapacity;
}


size_t Vector::ForEach(Vector::VectorElementAction a_actionCallbackFunction, void* a_context) {
    if(!a_actionCallbackFunction) {
        return 0;
    }

    size_t i;
    for(i = 0; i < this->m_sizeOfVector; i++) {
        if( a_actionCallbackFunction(this->m_vectorItems[i], i, a_context) == 0) {
            break;
        }
    }

    return i;
}

/* ----------------------------------- End of Main API Functions -------------------------------- */


/* Validation Functions: */

static Vector::Result ValidateNotOverflowAfterAdd(const Vector* a_vector) {
	return (a_vector->m_vectorCapacity + a_vector->m_extensionBlockSize < a_vector->m_sizeOfVector + 1) ? Vector::VECTOR_OVERFLOW_ERROR : Vector::VECTOR_SUCCESS;
}


static Vector::Result ValidateNotOutOfBoundsIndex(const Vector* a_vector, size_t _index) {
	return (_index > a_vector->m_sizeOfVector - 1) ? Vector::VECTOR_INDEX_OUT_OF_BOUNDS_ERROR : Vector::VECTOR_SUCCESS;
}


/* Helper Functions: */


static Vector::Result TryAddMoreMemoryToVector(Vector* a_vector) {
	void** temp = (void**)realloc(a_vector->m_vectorItems, ((a_vector->m_vectorCapacity + a_vector->m_extensionBlockSize) * sizeof(void*)));

	if(!temp) {
		return Vector::VECTOR_ALLOCATION_ERROR;
	}

	a_vector->m_vectorItems = temp;
	a_vector->m_vectorCapacity += a_vector->m_extensionBlockSize;

	return Vector::VECTOR_SUCCESS;
}


static void AddNewItemToEndOfVector(Vector* a_vector, void* _itemToAdd) {
	a_vector->m_vectorItems[a_vector->m_sizeOfVector] = _itemToAdd;
	a_vector->m_sizeOfVector++;
}


static void DeleteLastItemFromEndOfVector(Vector* a_vector, void** _itemToDelete) {
	a_vector->m_sizeOfVector--;
	*_itemToDelete = a_vector->m_vectorItems[a_vector->m_sizeOfVector];
	a_vector->m_vectorItems[a_vector->m_sizeOfVector] = NULL;
}


static Vector::Result TryShrinkVector(Vector* a_vector) {
	if(a_vector->m_vectorCapacity > a_vector->m_originalCapacity) {
		if(a_vector->m_vectorCapacity - a_vector->m_sizeOfVector /* Free places in vector */ > a_vector->m_extensionBlockSize * SHRINKING_FACTOR) {
			return ShrinkVector(a_vector);
		}
	}

	return Vector::VECTOR_SUCCESS; /* Ok in general - Not an error */
}


static Vector::Result ShrinkVector(Vector* a_vector) {
	void** temp = (void**)realloc(a_vector->m_vectorItems, ((a_vector->m_vectorCapacity - a_vector->m_extensionBlockSize) * sizeof(void*)));

	if(!temp) {
		return Vector::VECTOR_ALLOCATION_ERROR;
	}

	a_vector->m_vectorItems = temp;
	a_vector->m_vectorCapacity -= a_vector->m_extensionBlockSize;

	return Vector::VECTOR_SUCCESS;
}