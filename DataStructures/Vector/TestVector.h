#ifndef __TESTVECTOR_H__
#define __TESTVECTOR_H__

#include "Vector.h"

/* Testing Functions: */
int* GetVectorItemsArray(Vector* _vector);
size_t GetOriginalVectorSize(Vector* _vector);
size_t GetSizeOfVector(Vector* _vector);
size_t GetNumberOfItemsInVector(Vector* _vector);
size_t GetVectorExtensionBlockSize(Vector* _vector);
int GetVectorMagicNumber(Vector* _vector);
void SetVectorMagicNumber(Vector* _vector, int newMagicNumber);
void PrintAllVector(Vector* _vector);

#endif /* #ifndef __TESTVECTOR_H__ */
