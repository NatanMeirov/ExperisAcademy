/* ADT Errors header file */

#ifndef __ATDDEFS_H__
#define __ATDDEFS_H__

/* Defines: */
typedef enum
{/* General ADT Errors: */
	ERR_OK                   = 0,
	ERR_GENERAL,
	ERR_NOT_INITIALIZED,
	ERR_ALLOCATION_FAILED,
	ERR_REALLOCATION_FAILED,
	ERR_UNDERFLOW,
	ERR_OVERFLOW,
	ERR_WRONG_INDEX,
	ERR_WRONG_SIZE
} ADTErr;


/*
- Description: Returns an error message as string [can be printed or be writen in a log file]

- Input: [_errNum:] the error number (ADTErr Enum)

- Output: An error message string.

- Errors: No Errors.
*/
const char* HandleError(ADTErr _errNum);


#endif /* #ifndef __ATDDEFS_H__ */
