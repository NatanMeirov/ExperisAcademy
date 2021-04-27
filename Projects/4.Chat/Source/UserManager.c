/**
 * @file UserManager.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A User Manager module that used for inner management of collection of UserInformation objects
 * @version 1.0
 * @date 2021-04-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp */
#include "../Include/UserManager.h"
#include "../Include/UserInformation.h"
#include "../Include/GenericHashMap.h"

/* DEBUG */
#include <stdio.h>

/* Defines: */

#define INITIAL_DICTIONARY_LENGTH 250
#define EQUAL 1
#define NOT_EQUAL 0

struct UserManager
{
    HashMap* m_usersDictionary;
};


/* Static Functions Declarations: */

static size_t Hash(const void* _usernameAsKey);
static int AreEqualStrings(const void* _firstString, const void* _secondString);
static void DestroySingleUserInformation(void* _userInformation);
static UserManagerStatus MapHashMapResultToUserManagerStatus(HashMapResult _result);

static int DEBUG_PrintFunc(const void* _key, void* _value, void* _context);

/* --------------------------------------- Main API Functions ----------------------------------------*/

UserManager* UserManagerCreate()
{
    UserManager* userManager = NULL;

    userManager = malloc(sizeof(UserManager));
    if(!userManager)
    {
        return NULL;
    }

    userManager->m_usersDictionary = HashMapCreate(INITIAL_DICTIONARY_LENGTH, &Hash, &AreEqualStrings);
    if(!userManager->m_usersDictionary)
    {
        free(userManager);

        return NULL;
    }

    return userManager;
}


void UserManagerDestroy(UserManager** _userManager)
{
    if(_userManager && *_userManager)
    {
        HashMapDestroy(&(*_userManager)->m_usersDictionary, NULL, &DestroySingleUserInformation);

        free(*_userManager);
        *_userManager = NULL;
    }
}


UserManagerStatus UserManagerAddUser(UserManager* _userManager, char* _username, char* _password, int _isConnected)
{
    HashMapResult status;
    UserInformation* user = NULL;
    char* ptrToUsernameAsKey = NULL;

    if(!_userManager || !_username || !_password)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    user = UserInformationCreate(_username, _password, _isConnected);
    if(!user)
    {
        return USERMANAGER_ALLOCATION_FAILED;
    }

    ptrToUsernameAsKey = (char*)UserInformationGetUsername(user);

    status = HashMapInsert(_userManager->m_usersDictionary, ptrToUsernameAsKey, user);

    if(status == HASHMAP_ALLOCATION_ERROR || status == HASHMAP_KEY_DUPLICATE_ERROR)
    {
        UserInformationDestroy(&user);
    }

    return MapHashMapResultToUserManagerStatus(status);
}


UserManagerStatus UserManagerCheckIfUserIsExistsAndIfPasswordIsCorrect(UserManager* _userManager, char* _username, char* _password)
{
    void* user = NULL;

    if(!_userManager || _username || _password)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return USERMANAGER_USERNAME_NOT_FOUND;
    }

    if(strcmp(UserInformationGetPassword((UserInformation*)user), _password) == 0)
    {
        return USERMANAGER_SUCCESS;
    }

    return USERMANAGER_WRONG_PASSWORD;
}


int UserManagerCheckIfUserIsConnected(UserManager* _userManager, char* _username)
{
    void* user = NULL;

    if(!_userManager || !_username)
    {
        return -1;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return -1;
    }

    return UserInformationGetIsConnected((UserInformation*)user);
}

/* ----------------------------------- End of Main API Functions -------------------------------------*/


/* Static Functions Implementations: */


static size_t Hash(const void* _usernameAsKey)
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


static int AreEqualStrings(const void* _firstString, const void* _secondString)
{
    return (strcmp((char*)_firstString, (char*)_secondString) == 0) ? EQUAL : NOT_EQUAL;
}


static void DestroySingleUserInformation(void* _userInformation)
{
    UserInformationDestroy(((UserInformation**)&_userInformation));
}


static UserManagerStatus MapHashMapResultToUserManagerStatus(HashMapResult _result)
{
    if (_result == HASHMAP_KEY_DUPLICATE_ERROR)
    {
        return USERMANAGER_USERNAME_ALREADY_EXISTS;
    }
    else if(_result == HASHMAP_ALLOCATION_ERROR)
    {
        return USERMANAGER_ALLOCATION_FAILED;
    }

    /* else: _result == HASHMAP_SUCCESS */

    return USERMANAGER_SUCCESS;
}



/* Remove: */

static int DEBUG_PrintFunc(const void* _key, void* _value, void* _context)
{
    printf("UsernameAsKey: %s\n", (char*)_key);
    printf("User Info: %s -- %s -- Connected? %d\n\n", UserInformationGetUsername((UserInformation*)_value), UserInformationGetPassword((UserInformation*)_value), UserInformationGetIsConnected((UserInformation*)_value));

    return 1;
}

