#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <ctype.h> /* tolower */
#include <string.h> /* strlen */

#define BYTE 8

typedef struct CompressedByte
{
    unsigned char m_low4Bits : 4;
    unsigned char m_high4Bits : 4;
} CompressedByte;

char* BitwiseCompress_a_To_o_LimitedString(char* _strToCompress);
int ToLowerCaseAndValidateCorrectionOfStringLimits(char* _stringToCheck);
char CompressSingleByteTo4Bits(char _byte);
CompressedByte Convert8BitToSingleByte(char _first4Bits, char _second4Bits);
char* ByteToBinary(const unsigned char _byte);
void PrintAllBytes(char* _bytesStr);


int main(void)
{
    char str[] = "oooooooooooooooooooooooo";
    PrintAllBytes(BitwiseCompress_a_To_o_LimitedString(str));
    return 0;
}


char* BitwiseCompress_a_To_o_LimitedString(char* _strToCompress)
{
    size_t i, j;
    char byte1, byte2;
    CompressedByte* compressedByte = NULL;

    if(!_strToCompress)
    {
        return NULL;
    }

    if(!(ToLowerCaseAndValidateCorrectionOfStringLimits(_strToCompress)))
    {
        return NULL;
    }

    for (i = 0, j = 0; _strToCompress[i] != '\0' && _strToCompress[i + 1] != '\0'; j++, i += 2)
    {
        compressedByte = (CompressedByte*)&_strToCompress[j];
        compressedByte->m_high4Bits = CompressSingleByteTo4Bits(_strToCompress[i]);
        compressedByte->m_low4Bits = CompressSingleByteTo4Bits(_strToCompress[i + 1]);
    }
    
    if(_strToCompress[i] != '\0')/* In that case: _strToCompress[i + 1] == '\0' => Need to compress the _strToCompress[i] (because the whole string had odd number of characters - the last had not compressed) */
    {
        compressedByte = (CompressedByte*)&_strToCompress[j];
        compressedByte->m_high4Bits = CompressSingleByteTo4Bits(_strToCompress[i]);
        compressedByte->m_low4Bits = 0;
        j++;
    }

    _strToCompress[j] = '\0';

    return _strToCompress;
}

int ToLowerCaseAndValidateCorrectionOfStringLimits(char* _stringToCheck)
{
    size_t i;
    int isCorrect = 1;

    for(i = 0; _stringToCheck[i] != '\0'; i++) /* Validates correction of string's letters */
    {
        _stringToCheck[i] = tolower(_stringToCheck[i]);

        if(_stringToCheck[i] < 'a' || _stringToCheck[i] > 'o')
        {
            isCorrect = 0;
        }
    }

    return isCorrect;
}

char CompressSingleByteTo4Bits(char _byte)
{
    return _byte - 'a' + 1;
}

CompressedByte Convert8BitToSingleByte(char _first4Bits, char _second4Bits)
{
    CompressedByte compressedByte;

    compressedByte.m_high4Bits = _first4Bits;
    compressedByte.m_low4Bits = _second4Bits;

    return compressedByte;
}


char* ByteToBinary(const unsigned char _byte)
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

void PrintAllBytes(char* _bytesStr)
{
    size_t i;
    char* binaryStr = NULL;

    if(!_bytesStr)
    {
        return;
    }

    for(i = 0; _bytesStr[i] != '\0'; i++)
    {
        binaryStr = ByteToBinary(_bytesStr[i]);
        printf("%s", binaryStr);
        free(binaryStr);
    }
    putchar('\n');
}
