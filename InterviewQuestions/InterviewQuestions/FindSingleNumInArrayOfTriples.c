/* Note: this question can be answered with 3 or more same numbers and one single num to find - and just CHAGNE THE MODOLU value to the num */

#include <stdio.h> /* printf */


#define BITS_IN_BYTE 8
#define BITS_IN_INT (sizeof(int) * BITS_IN_BYTE)


static void FillBitsArray(int _number, int* bitsArray);


int FindSingleNumInArrayOfTriples(int* arrayOfTriplesAndOneSingleNum, int sizeOfArray)
{
    size_t i;
    int bitsArray[BITS_IN_INT + 1] = {0};
    int resultNum;

    for(i = 0; i < sizeOfArray; ++i)
    {
        FillBitsArray(arrayOfTriplesAndOneSingleNum[i], bitsArray);
    }

    /* Starting from index 1 */
    for(i = 1; i < BITS_IN_INT + 1; ++i)
    {
        bitsArray[i] = (bitsArray[i] % 3);
    }
}


static void FillBitsArray(int _number, int* bitsArray)
{
    size_t counter = 1;
    while(_number)
    {
        bitsArray[counter] += (_number & 1);
        _number >>= 1;
        ++counter;
    }
}