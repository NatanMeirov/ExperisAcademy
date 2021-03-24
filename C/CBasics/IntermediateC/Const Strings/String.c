/* String functions source file */

#include <stddef.h> /* size_t */
#include "String.h"

size_t StrLen(const char* _str)
{
    size_t counter = 0;

    while(*_str++ != '\0')
    {
        counter++;
    }

    return counter;
}

size_t RecursiveStrLen(const char* _str)
{
    if(*_str == '\0')
    {
        return 0;
    }

    return 1 + RecursiveStrLen(_str + 1);
}

int StrCompare(const char* _str1, const char* _str2)
{
    while(*_str1 != '\0' && *_str2 != '\0')
    {
        if(*_str1 != *_str2)
        {
            if(*_str1 > *_str2)
            {
                return 1;
            }
            else /* if *_str1 < *_str2 */
            {
                return -1;
            }
        }

        _str1++;
        _str2++;
    }

    if(*_str1 != '\0' && *_str2 == '\0')
    {
        return 1;
    }

    if(*_str1 == '\0' && *_str2 != '\0')
    {
        return -1;
    }

    return 0; /* Strings are equal */
}

int RecursiveStrCompare(const char* _str1, const char* _str2)
{
    if(*_str1 == '\0' && *_str2 == '\0')
    {
        return 0; /* Strings are equal */
    }

    if(*_str1 != '\0' && *_str2 == '\0')
    {
        return 1;
    }

    if(*_str1 == '\0' && *_str2 != '\0')
    {
        return -1;
    }

    if(*_str1 > *_str2)
    {
        return 1;
    }

    if(*_str1 < *_str2)
    {
        return -1;
    }

    return RecursiveStrCompare(_str1 + 1, _str2 + 1);
}

char* StrCopy(char* _destination, const char* _source)
{
    while(*_source != '\0')
    {
        *_destination = *_source;
        _destination++;
        _source++;
    }

    return _destination;
}

char* RecursiveStrCopy(char* _destination, const char* _source)
{
    if(*_source == '\0')
    {
        return _destination;
    }

    *_destination = *_source;

    return RecursiveStrCopy(_destination + 1, _source + 1);
}

char* StrNCopy(char* _destination, const char* _source, size_t _num)
{
    size_t i;

    for(i = 0; i < _num && _source[i] != '\0'; i++)
    {
        *_destination = *_source;
        _destination++;
        _source++;
    }

    return _destination;
}

char* RecursiveStrNCopy(char* _destination, const char* _source, size_t _num)
{
    if(_num == 0 || *_source == '\0')
    {
        return _destination;
    }

    *_destination = *_source;

    return RecursiveStrNCopy(_destination + 1, _source + 1, _num - 1);
}

char* StrConcat(char* _destination, const char* _source)
{
    _destination += StrLen(_destination); /* Place of the '\0' */

    while(*_source != '\0')
    {
        *_destination = *_source;
        _destination++;
        _source++;
    }

    return _destination;
}

char* StrSubString(const char* _str, const char* _search)
{
    /* TODO: shouldn't it be a true or false answer? need to ask the lecturer. */
}