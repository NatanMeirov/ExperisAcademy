/**
 * @file DictStringKeyUtils.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An Util functions file, to handle Hash Map callback functions for C-String Keys
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __DICTSTRINGKEYUTILS_H__
#define __DICTSTRINGKEYUTILS_H__


/* Includes: */

#include <stddef.h> /* size_t */


/* ---------------------------------------- Main API Functions ------------------------------------- */

size_t StringKeyHash(const void* _usernameAsKey);

int AreEqualStringKeys(const void* _firstString, const void* _secondString);

/* ------------------------------------- End ofMain API Functions ---------------------------------- */


#endif /* #ifndef __DICTSTRINGKEYUTILS_H__ */