#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <ctype.h> /* tolower */
#include <string.h> /* strlen */

#define BYTE 8

char* BitwiseCompress_a_To_o_LimitedString(char* _strToCompress);
int ToLowerCaseAndValidateCorrectionOfStringLimits(char* _stringToCheck);
char CompressSingleByteTo4Bits(char _byte);
char Convert8BitToSingleByte(char _first4Bits, char _second4Bits);
char* ByteToBinary(const unsigned char _byte);
void PrintAllBytes(char* _bytesStr);


int main(void)
{
    char str[] = "abco";
    PrintAllBytes(BitwiseCompress_a_To_o_LimitedString(str));
    return 0;
}


char* BitwiseCompress_a_To_o_LimitedString(char* _strToCompress)
{
    size_t i, j;
    char byte1, byte2;

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
        byte1 = CompressSingleByteTo4Bits(_strToCompress[i]);
        byte2 = CompressSingleByteTo4Bits(_strToCompress[i + 1]);

        _strToCompress[j] = Convert8BitToSingleByte(byte1, byte2);
    }
    
    if(_strToCompress[i] != '\0')/* In that case: _strToCompress[i + 1] == '\0' => Need to compress the _strToCompress[i] (because the whole string had odd number of characters - the last had not compressed) */
    {
        byte1 = CompressSingleByteTo4Bits(_strToCompress[i]);
        _strToCompress[j] = Convert8BitToSingleByte(byte1, 0);
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

char Convert8BitToSingleByte(char _first4Bits, char _second4Bits)
{
    return ((_first4Bits << 4) | _second4Bits);
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
