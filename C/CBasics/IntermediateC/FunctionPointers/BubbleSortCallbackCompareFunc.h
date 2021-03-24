#ifndef __BUBBLESORTCALLBACKCOMPAREFUNC_h__
#define __BUBBLESORTCALLBACKCOMPAREFUNC_h__


typedef int (*callbackCompareFunction)(int _firstNumber, int _secondNumber);

/* 
- Description: Sorts a given int array by invoking a given callback compare function. Returns a status - 1 for success, else 0 if error has occured (NULL pointer).
*/
int BubbleSort(int* _array, int _sizeOfArray, callbackCompareFunction _callback);

#endif /* #ifndef __BUBBLESORTCALLBACKCOMPAREFUNC_h__ */
