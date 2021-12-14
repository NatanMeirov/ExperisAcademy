#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define LETTERS_COUNT 26
#define DIGITS_COUNT 10


#define LETTERS_COUNT 26
#define DIGITS_COUNT 10

char * reformat(char * s){
    int i = 0, j = 0;
    int lettersCountingArr[LETTERS_COUNT] = {0};
    int digitsCountingArr[DIGITS_COUNT] = {0};
    int lettersCounter = 0;
    int digitsCounter = 0;
    int difBetweenDigitsAndLettersCounts;
    int totalStrSize;
    char c;
    char* reformatedS;
    char* cursor;
    
    while(*s != '\0')
    {
        c = *s;
        if(c - '0' >= 0 && c - '0' <= 9)
        {
            ++digitsCountingArr[c - '0'];
            ++digitsCounter;
        }
        else
        {
            ++lettersCountingArr[c - 'a'];
            ++lettersCounter;
        }
        
        ++s;
    }
    
    difBetweenDigitsAndLettersCounts = digitsCounter - lettersCounter;
    if(difBetweenDigitsAndLettersCounts != 1
    && difBetweenDigitsAndLettersCounts != -1
    && difBetweenDigitsAndLettersCounts != 0)
    {
        reformatedS = malloc(1);
        *reformatedS = '\0';
        return reformatedS;
    }
    
    
    totalStrSize = digitsCounter + lettersCounter;
    reformatedS = malloc(totalStrSize + 1);
    cursor = reformatedS;
    while(totalStrSize-- > 0)
    {
        for(; i < LETTERS_COUNT; ++i)
        {
            if(lettersCountingArr[i] > 0)
            {
                break;
            }
        }
        
        *cursor++ = i + 'a';
        if(i != LETTERS_COUNT)
        {
            --lettersCountingArr[i];    
        }
        
        
        for(; j < DIGITS_COUNT; ++j)
        {
            if(digitsCountingArr[j] > 0)
            {
                break;
            }
        }
        
        *cursor++ = j + '0';
        if(j != DIGITS_COUNT)
        {
            --digitsCountingArr[j];
        }
        
    }
    
    *cursor = '\0';
    return reformatedS;
}


int main(void)
{
    printf("%s", reformat("a0b1c2"));
    return 0;
}
