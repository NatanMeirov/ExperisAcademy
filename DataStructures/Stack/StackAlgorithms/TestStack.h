/* TestStack header file */

#ifndef __TESTSTACK_h__
#define __TESTSTACK_h__

#include "Stack.h"

int* GetStackInnerVector(Stack* _stack);
int GetStackMagicNumber(Stack* _stack);
void SetStackMagicNumber(Stack* _stack, int _newMagicNumber);

#endif /* #ifndef __TESTSTACK_h__ */
