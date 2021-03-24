#include <stdio.h>
#include <stdlib.h>

#define BYTE 8
#define INT_BITS_COUNT 32

size_t BitCount(const unsigned int _num);
unsigned char InvertByte(const unsigned char _byte);
char* ByteToBinary(const unsigned char _byte);
void InvertAndPrintByte(const unsigned char _byte);
unsigned char BitsRotateToRight(unsigned char _byte, size_t _bitsNumToRotate);
size_t IdenticalBitPairsCount(unsigned int _integer);


int main(void)
{
    printf("%ld\n", BitCount(0xffffffff)); /* 32-bit int */
    InvertAndPrintByte(1); /* 11111110 */
    printf("%s\n", ByteToBinary(BitsRotateToRight(5, 3))); /* 00000101 => 1010000 */
    printf("%ld\n", IdenticalBitPairsCount(1));
    
    return 0;
}


size_t BitCount(const unsigned int _num)
{
    size_t i, counter = 0;
    unsigned int temp = _num; /* No need const when Call By Value - so can process the original _num */

    while(temp) /* While temp > 0 */
    {
        if(temp & 1) /* == 1 */
        {
            counter++;
        }

        /* Same: counter += temp & 1  (if its 0 - same counter, if 1 => counter += 1 ) */

        temp >>= 1;
    }

    return counter;
}


unsigned char InvertByte(const unsigned char _byte)
{
    return ~_byte;
}


char* ByteToBinary(const unsigned char _byte) /* Change to regular BinaryPrint */
{
    int i;
    char* bin = malloc(BYTE + 1); /* +1 for '\0' */
    unsigned char temp;

    bin[BYTE] = '\0';

    for(i = BYTE - 1, temp = 1; i >= 0; i--, temp <<= 1)
    {
        bin[i] = ((_byte & temp) == temp) ? '1' : '0';
    }

    return bin;
}


void InvertAndPrintByte(const unsigned char _byte)
{
    unsigned char invertedByte = InvertByte(_byte);
    char* binaryRepresentation = ByteToBinary(invertedByte);
    printf("%s\n", binaryRepresentation);
    free(binaryRepresentation);
}


unsigned char BitsRotateToRight(unsigned char _byte, size_t _bitsNumToRotate)
{
    size_t i, rotates = _bitsNumToRotate % BYTE; /* x % 8 == 0 rotates - its a full rotate, 9 % 8 == 1 - single bit rotate, ... */
    unsigned char bit;

    for(i = 0; i < rotates; i++)
    {
        bit = _byte & 1; /* bit is LSB */
        _byte >>= 1;
        if(bit) /* if bit is 0 - no need to change the MSB */
        {
            _byte |= (bit << BYTE - 1); /* MSB is 1 if bit is 1 */
        }
    }

    return _byte;
}


size_t IdenticalBitPairsCount(unsigned int _integer)
{
    size_t i, counter = 0;

    for(i = 0; i < (INT_BITS_COUNT / 2); i++)
    {
        if((_integer & 1) == ((_integer >> 1) & 1))
        {
            counter++;
        }

        _integer >>= 2;
    }

    return counter;
}