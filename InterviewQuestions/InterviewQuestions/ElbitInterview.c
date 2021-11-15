#define N 10
#include <stddef.h> /* size_t */
#include <limits.h> // CHAR_BIT
#include <string.h> /* memcpy */


static void CopyMemoryData(void* _destination, void* _source, size_t _bytesToCopy);


int f2(int _a[N][N])
{
    int hasCorrectCondition = 1; /* boolean flag */
    size_t currentRow = 0, currentColumn;

    for(currentColumn = 0; currentColumn < N; ++currentColumn, currentRow = 0)
    {
        while(currentRow != (N - 1))
        {
            if(_a[currentRow][currentColumn] < _a[currentRow + 1][currentColumn])
            {
                hasCorrectCondition = 1;
            }
            else
            {
                hasCorrectCondition = 0;
                break; /* Breaks from the inner loop */
            }

            ++currentRow;
        }

        if(currentRow == (N - 1)) /* Last row */
        {
            if(hasCorrectCondition)
            {
                break; /* True - the matrix has a correct condition (breaks from the outer loop) */
            }
        }
    }

    return hasCorrectCondition;
}


/* Formatting the input array IN-PLACE! */
/* _inputArray size MUST be 8 by convention! */
void PrinterProxy(int* _inputArray, int* _indexesMap)
{
    size_t i;
    int helperArray[CHAR_BIT] = {0};

    CopyMemoryData(helperArray, _inputArray, sizeof(int) * CHAR_BIT);

    for(i = 0; i < CHAR_BIT; ++i)
    {
        _inputArray[_indexesMap[i]] = helperArray[i];
    }
}


static void CopyMemoryData(void* _destination, void* _source, size_t _bytesToCopy)
{
    memcpy(_destination, _source, _bytesToCopy);
}