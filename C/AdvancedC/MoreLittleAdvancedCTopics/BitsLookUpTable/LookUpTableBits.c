#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */

/* ------------------------------------ */
/* Bitewise Operations: */
#define SET_BIT(variable, bit) ((variable) |= (1 << (bit)))
#define CLEAR_BIT(variable, bit) ((variable) &= ~(1 << (bit)))
#define TOGGLE_BIT(variable, bit) ((variable) ^= (1 << (bit)))
#define TEST_BIT(testedBit, variable, bit) ((testedBit) = (variable) & (1 << (bit)))
/* ------------------------------------ */

#define SINGLE_BYTE_DECIMAL_VALUE 256
#define BYTE 8


/*
size_t OnBitsCount(unsigned char _ch)
{
    size_t i, counter = 0;

    while(_ch) // While _ch > 0
    {
        if(_ch & 1) // == 1
        {
            counter++;
        }

        // Same: counter += temp & 1  (if its 0 - same counter, if 1 => counter += 1 )

        _ch >>= 1;
    }

    return counter;
}
*/

void BuildLookUpTable(char* _lookUpTable)
{
    int characterValue = 0, temp = 0;

    for(characterValue = 0; characterValue < SINGLE_BYTE_DECIMAL_VALUE; characterValue++)
    {
        temp = characterValue;
        while(temp)
        {
            _lookUpTable[characterValue] += (temp & 1);
            temp >>= 1;
        }
    }
}


size_t Char1ByteOnBitsCountUsingLookUpTable(unsigned char _ch)
{
    static char flag; /* static variable - only on this function scope but will be IN THE DATA AND NOT IN THE STACK, and will be initialized with 0 automatically */
    static char lookUpTable[SINGLE_BYTE_DECIMAL_VALUE];

    if(!flag)
    {
        BuildLookUpTable(lookUpTable);
        flag = 1;
    }

    return (size_t)lookUpTable[_ch];
}


size_t Int4BytesOnBitsCountUsingLookUpTable(unsigned int _num)
{
    static char flag; /* static variable - only on this function scope but will be IN THE DATA AND NOT IN THE STACK, and will be initialized with 0 automatically */
    static char lookUpTable[SINGLE_BYTE_DECIMAL_VALUE];
    size_t sum = 0;

    if(!flag)
    {
        BuildLookUpTable(lookUpTable);
        flag = 1;
    }

    while(_num)
    {
        sum += lookUpTable[(_num & (SINGLE_BYTE_DECIMAL_VALUE - 1))];
        _num >>= BYTE;
    }

    return sum;
}


int main(void)
{
    printf("bits count: %ld\n", Char1ByteOnBitsCountUsingLookUpTable(55));
    printf("bits count: %ld\n", Char1ByteOnBitsCountUsingLookUpTable(0));
    printf("bits count: %ld\n", Char1ByteOnBitsCountUsingLookUpTable(5));
    printf("bits count: %ld\n", Char1ByteOnBitsCountUsingLookUpTable(2));
    printf("bits count: %ld\n", Char1ByteOnBitsCountUsingLookUpTable(0xff));
    printf("-------------------\n");
    printf("bits count: %ld\n", Int4BytesOnBitsCountUsingLookUpTable(0xff+1)); /* 1 */
    printf("bits count: %ld\n", Int4BytesOnBitsCountUsingLookUpTable(0xff)); /* 8 */
    printf("bits count: %ld\n", Int4BytesOnBitsCountUsingLookUpTable(0xffff)); /* 16 */
    printf("bits count: %ld\n", Int4BytesOnBitsCountUsingLookUpTable(0xffffff)); /* 24 */
    printf("bits count: %ld\n", Int4BytesOnBitsCountUsingLookUpTable(0xffffffff)); /* 32 */
    printf("-------------------\n");

    return 0;
}