/* String functions header file */

#include <stddef.h> /* size_t */


size_t StrLen(const char* _str);

size_t RecursiveStrLen(const char* _str);

int StrCompare(const char* _str1, const char* _str2);

int RecursiveStrCompare(const char* _str1, const char* _str2);

char* StrCopy(char* _destination, const char* _source);

char* RecursiveStrCopy(char* _destination, const char* _source);

char* StrNCopy(char* _destination, const char* _source, size_t _num);

char* RecursiveStrNCopy(char* _destination, const char* _source, size_t _num);

char* StrConcat(char* _destination, const char* _source);

char* StrSubString(const char* _str, const char* _search);