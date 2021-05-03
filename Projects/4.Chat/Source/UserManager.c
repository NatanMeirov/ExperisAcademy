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
#include "../Include/DictStringKeyUtils.h"

/* Defines: */

#define INITIAL_DICTIONARY_LENGTH 250
#define DISCONNECT 0
#define CONNECT 1

struct UserManager
{
    HashMap* m_usersDictionary;
};


/* Static Functions Declarations: */

static void DestroySingleUserInformation(void* _userInformation);
static UserManagerStatus MapHashMapResultToUserManagerStatus(HashMapResult _result);
static UserManagerStatus MapUserInformationResultToUserManagerStatus(UserInformationResult _result);


/* --------------------------------------- Main API Functions ----------------------------------------*/

UserManager* UserManagerCreate()
{
    UserManager* userManager = NULL;

    userManager = malloc(sizeof(UserManager));
    if(!userManager)
    {
        return NULL;
    }

    userManager->m_usersDictionary = HashMapCreate(INITIAL_DICTIONARY_LENGTH, &StringKeyHash, &AreEqualStringKeys);
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


UserManagerStatus UserManagerAddUser(UserManager* _userManager, const char* _username, const char* _password, int _isConnected)
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
        return USERMANAGER_ALLOCATION_FAILED; /* Could be an error because of a not valid string length (more then 100) */
    }

    ptrToUsernameAsKey = (char*)UserInformationGetUsername(user);

    status = HashMapInsert(_userManager->m_usersDictionary, ptrToUsernameAsKey, user);

    if(status == HASHMAP_ALLOCATION_ERROR || status == HASHMAP_KEY_DUPLICATE_ERROR)
    {
        UserInformationDestroy(&user);
    }

    return MapHashMapResultToUserManagerStatus(status);
}


UserManagerStatus UserManagerCheckIfUserIsExistsAndIfPasswordIsCorrect(UserManager* _userManager, const char* _username, const char* _password)
{
    void* user = NULL;

    if(!_userManager || !_username || !_password)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return MapHashMapResultToUserManagerStatus(HASHMAP_KEY_NOT_FOUND_ERROR);
    }

    if(strcmp(UserInformationGetPassword((UserInformation*)user), _password) == 0)
    {
        return USERMANAGER_SUCCESS;
    }

    return USERMANAGER_WRONG_PASSWORD;
}


int UserManagerCheckIfUserIsConnected(UserManager* _userManager, const char* _username)
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


UserManagerStatus UserManagerSetUserAsConnected(UserManager* _userManager, const char* _username)
{
    void* user = NULL;

    if(!_userManager || !_username)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return MapHashMapResultToUserManagerStatus(HASHMAP_KEY_NOT_FOUND_ERROR);
    }

    return MapUserInformationResultToUserManagerStatus(UserInformationSetIsConnected(user, CONNECT));
}


UserManagerStatus UserManagerSetUserAsDisconnected(UserManager* _userManager, const char* _username)
{
    void* user = NULL;

    if(!_userManager || !_username)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return MapHashMapResultToUserManagerStatus(HASHMAP_KEY_NOT_FOUND_ERROR);
    }

    return MapUserInformationResultToUserManagerStatus(UserInformationSetIsConnected(user, DISCONNECT));
}


UserManagerStatus UserManagerAddGroupToUser(UserManager* _userManager, const char* _username, const char* _groupName)
{
    void* user = NULL;
    UserInformationResult status;

    if(!_userManager || !_username || !_groupName)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return MapHashMapResultToUserManagerStatus(HASHMAP_KEY_NOT_FOUND_ERROR);
    }

    return MapUserInformationResultToUserManagerStatus(UserInformationAddGroup(user, _groupName));
}


UserManagerStatus UserManagerRemoveGroupFromUser(UserManager* _userManager, const char* _username, const char* _groupName)
{
    void* user = NULL;
    UserInformationResult status;

    if(!_userManager || !_username || !_groupName)
    {
        return USERMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_userManager->m_usersDictionary, _username, &user) == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return MapHashMapResultToUserManagerStatus(HASHMAP_KEY_NOT_FOUND_ERROR);
    }

    return MapUserInformationResultToUserManagerStatus(UserInformationRemoveGroup(user, _groupName));
}

/* ----------------------------------- End of Main API Functions -------------------------------------*/


/* Static Functions Implementations: */


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
    else if(_result == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return USERMANAGER_USERNAME_NOT_FOUND;
    }

    /* else: _result == HASHMAP_SUCCESS */

    return USERMANAGER_SUCCESS;
}


static UserManagerStatus MapUserInformationResultToUserManagerStatus(UserInformationResult _result)
{
    if(_result == USERINFORMATION_ALLOCATION_FAILED)
    {
        return USERMANAGER_ALLOCATION_FAILED;
    }
    else if(_result == USERINFORMATION_GROUP_NOT_FOUND)
    {
        return USERMANAGER_GROUP_OF_USER_NOT_FOUND;
    }
    else if(_result == USERINFORMATION_GROUP_ALREADY_EXISTS)
    {
        return USERMANAGER_GROUP_OF_USER_ALREADY_EXISTS;
    }

    /* else: _result == USERINFORMATION_SUCCESS */

    return USERMANAGER_SUCCESS;
}