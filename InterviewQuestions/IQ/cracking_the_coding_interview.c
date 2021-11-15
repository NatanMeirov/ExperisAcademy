#include <stddef.h>
#include <stdio.h>


#define LETTERS_IN_ENGLISH 26
typedef unsigned int ArrayType;


/* Static Functions */
static void InitLettersCountingArray(const char* _str, ArrayType* _countingArray)
{
    while(*_str != '\0')
    {
        unsigned int charNumericVal = (*_str) - 'a';
        ++_countingArray[charNumericVal];

        ++_str;
    }
}


static int AreEqualCountingArrays(ArrayType* _countingArray1, ArrayType* _countingArray2, size_t _arraysSize)
{
    size_t i;
    for(i = 0; i < _arraysSize; ++i)
    {
        if(_countingArray1[i] != _countingArray2[i])
        {
            return 0;
        }
    }

    return 1;
}


/* Main IQ */


/* For ASCII lowercase 'a' to 'z' characters */
int IsUniqueStr(const char* _str)
{
    unsigned long existingCharsIndicator = 0;

    if(!_str)
    {
        return 0;
    }

    while(*_str != '\0')
    {
        unsigned long currentCharVal = (*_str) - 'a';
        if((existingCharsIndicator & (1UL << currentCharVal)) > 0)
        {
            return 0; /* The character has already seen before */
        }
        existingCharsIndicator |= 1UL << currentCharVal;

        ++_str;
    }

    return 1;
}


/* For ASCII lowercase 'a' to 'z' characters */
int IsPermutationOf(const char* _str1, const char* _str2)
{
    ArrayType lettersCountingArrayOfStr1[LETTERS_IN_ENGLISH] = {0};
    ArrayType lettersCountingArrayOfStr2[LETTERS_IN_ENGLISH] = {0};

    if(!_str1 || !_str2)
    {
        return 0;
    }

    /*
    - Alloc 2 additional strings on the heap and copy the 2 given strings
    - transform (map) the new allocated strings from uppercase to lowercase
    - dealloc the additional strings
    */

    InitLettersCountingArray(_str1, lettersCountingArrayOfStr1);
    InitLettersCountingArray(_str2, lettersCountingArrayOfStr2);

    return AreEqualCountingArrays(lettersCountingArrayOfStr1, lettersCountingArrayOfStr2, LETTERS_IN_ENGLISH);
}


/* TESTS */

void Test_IsUniqueStr(void)
{
    printf("1 == ");
    printf("%d\n", IsUniqueStr("abcdefghijklmnopqrstuvwxyz"));

    printf("0 == ");
    printf("%d\n", IsUniqueStr("abcdefghijklmnopqrstuvwxyza"));

    printf("0 == ");
    printf("%d\n", IsUniqueStr("abcdabcdaaa"));

    printf("1 == ");
    printf("%d\n", IsUniqueStr("a"));

    printf("1 == ");
    printf("%d\n", IsUniqueStr(""));

    printf("0 == ");
    printf("%d\n", IsUniqueStr(NULL));
}


void Test_IsPermutationOf(void)
{
    printf("1 == ");
    printf("%d\n", IsPermutationOf("good", "oodg"));

    printf("0 == ");
    printf("%d\n", IsPermutationOf("hero", "hello"));

    printf("1 == ");
    printf("%d\n", IsPermutationOf("", ""));

    printf("0 == ");
    printf("%d\n", IsPermutationOf(NULL, "some"));

    printf("0 == ");
    printf("%d\n", IsPermutationOf("other", NULL));

    printf("1 == ");
    printf("%d\n", IsPermutationOf("error", "error"));

    printf("1 == ");
    printf("%d\n", IsPermutationOf("name", "eman"));

    printf("1 == ");
    printf("%d\n", IsPermutationOf("aaab", "abaa"));
}


int main(void)
{
    printf("============================================\n");
    Test_IsUniqueStr();
    printf("============================================\n");
    Test_IsPermutationOf();
    printf("============================================\n");

    return 0;
}
