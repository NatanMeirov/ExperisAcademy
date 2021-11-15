#include <limits.h> /* CHAR_BIT */
#include <stdio.h> /* printf */

#define NUM_TO_CHECK_IF_DIVIDED_BY 7
#define INT_BITS sizeof(int) * CHAR_BIT

static int FindMaxDividedNumThatTheSameDigitIncludedIn(void);
static int FindMaxPositiveInt(void);
static int HasDigitFoundIn(int _numberToCheck, int _digitToFindIn);


static int FindMaxDividedNumThatTheSameDigitIncludedIn(void)
{
    int isFoundCorrectNumber = 0;
    int currentIntegerToCheck = FindMaxPositiveInt();
    printf("%d\n", currentIntegerToCheck);

    while(!isFoundCorrectNumber)
    {
        if(currentIntegerToCheck % NUM_TO_CHECK_IF_DIVIDED_BY == 0)
        {
            if(HasDigitFoundIn(currentIntegerToCheck, NUM_TO_CHECK_IF_DIVIDED_BY))
            {
                isFoundCorrectNumber = 1; /* True */
            }
        }

        --currentIntegerToCheck;
    }

    return currentIntegerToCheck;
}


static int FindMaxPositiveInt(void)
{
    int maxPositiveInteger = ~((int)0); /* = -1 */
    return maxPositiveInteger & ~(((int)1 << (INT_BITS - 1)));
}


static int HasDigitFoundIn(int _numberToCheck, int _digitToFindIn)
{
    int currentDigitToCheck;

    while(_numberToCheck > 0)
    {
        currentDigitToCheck = _numberToCheck % 10;
        if(_digitToFindIn == currentDigitToCheck)
        {
            return 1; /* True */
        }

        _numberToCheck /= 10;
    }

    return 0; /* False */
}


int main(void)
{
    printf("%d\n",FindMaxDividedNumThatTheSameDigitIncludedIn());
    return 0;
}


// Free() implementation:

void* g_allocPtr;

void Free(void* _ptr)
{
    *(void**)_ptr = g_allocPtr;
    g_allocPtr = _ptr;
}