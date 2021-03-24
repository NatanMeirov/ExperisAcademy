/* Status Codes: */
#define OK 0
#define NULL_PTR_ERROR -10


/*
- Description: Reversing a string in place
- Input: [_str:] a string (as a pointer to char)
- Output: Status code - OK, NULL_PTR_ERROR
- Errors: NULL_PTR_ERROR
*/
int ReverseString(char* _str);


/*
- Description: Converting an ASCII string to an integer (returning the converted number through an integer pointer)
- Input: [_str:] a string (as a pointer to char), [_convertedNumber:] a pointer to an integer (to store the converted number value)
- Output: Status code - OK, NULL_PTR_ERROR
- Errors: NULL_PTR_ERROR
*/
int AtoI(char* _str, int* _convertedNumber);


/*
- Description: Converting an integer to an ASCII string (returning the converted string through a char pointer)
- Input: [_num:] an integer, [_buffer] a char buffer with fixed length (to store the converted string)
- Output: Status code - OK, NULL_PTR_ERROR
- Errors: NULL_PTR_ERROR
*/
int ItoA(int _num, char* _buffer);


/*
- Description: Squeezing two strings, returning the squeezed string in the first string param
- Input: [_str1:] a string to squeeze, [_str2] a string to use as letters set to squeeze from the first string
- Output: Status code - OK, NULL_PTR_ERROR
- Errors: NULL_PTR_ERROR
*/
int StringsSqueeze(char* _str1, char* _str2);



/*
- Description: Finds the location (start index) of the second string in the first string (searchs for an inner string in the first string) 
- Input: [_str:] a string to check, [_innerStr] an inner string to search inside the other string, [_locationIndex:] a pointer to an integer to store the location in it, if not found, returns -1 in this pointer
- Output: Status code - OK, NULL_PTR_ERROR
- Errors: NULL_PTR_ERROR
*/
int FindInnerStringLocation(char* _str, char* _innerStr, int* _locationIndex);


