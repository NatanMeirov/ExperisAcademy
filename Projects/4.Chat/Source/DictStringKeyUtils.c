/**
 * @file DictStringKeyUtils.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An Util functions file, to handle Hash Map callback functions for C-String Keys
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <string.h> /* strcmp */


/* Defines: */

#define EQUAL 1
#define NOT_EQUAL 0


/* ---------------------------------------- Main API Functions ------------------------------------- */

size_t StringKeyHash(const void* _usernameAsKey)
{
    size_t number = 0;
    char* cursor = (char*)_usernameAsKey;

    while(*cursor != '\0')
    {
        number += (int)(*((char*)_usernameAsKey));
        cursor++;
    }

    return number;
}


int AreEqualStringKeys(const void* _firstString, const void* _secondString)
{
    return (strcmp((char*)_firstString, (char*)_secondString) == 0) ? EQUAL : NOT_EQUAL;
}

/* ------------------------------------- End ofMain API Functions ---------------------------------- */