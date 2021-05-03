/**
 * @file UserManager.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A User Manager module that would be used for inner management of collection of UserInformation objects
 * @version 1.0
 * @date 2021-04-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#include "../Include/GenericLinkedList.h"

/* Defines: */

typedef struct UserManager UserManager;

typedef enum UserManagerStatus
{
    USERMANAGER_SUCCESS = 0,
    USERMANAGER_NOT_INITIALIZED,
    USERMANAGER_ALLOCATION_FAILED,
    USERMANAGER_USERNAME_NOT_FOUND,
    USERMANAGER_WRONG_PASSWORD,
    USERMANAGER_USERNAME_ALREADY_EXISTS,
    USERMANAGER_GROUP_OF_USER_NOT_FOUND,
    USERMANAGER_GROUP_OF_USER_ALREADY_EXISTS
} UserManagerStatus;

/* --------------------------------------- Main API Functions ----------------------------------------*/

/* NULL, on failure */
UserManager* UserManagerCreate();

void UserManagerDestroy(UserManager** _userManager);

UserManagerStatus UserManagerAddUser(UserManager* _userManager, const char* _username, const char* _password, int _isConnected);

UserManagerStatus UserManagerCheckIfUserIsExistsAndIfPasswordIsCorrect(UserManager* _userManager, const char* _username, const char* _password);

/* return 1 if connected, else 0 / If _userManager or _username are NULL, or if user is not found - returned value will be -1 */
int UserManagerCheckIfUserIsConnected(UserManager* _userManager, const char* _username);

UserManagerStatus UserManagerSetUserAsConnected(UserManager* _userManager, const char* _username);

UserManagerStatus UserManagerSetUserAsDisconnected(UserManager* _userManager, const char* _username);

UserManagerStatus UserManagerAddGroupToUser(UserManager* _userManager, const char* _username, const char* _groupName);

UserManagerStatus UserManagerRemoveGroupFromUser(UserManager* _userManager, const char* _username, const char* _groupName);

/* ----------------------------------- End of Main API Functions -------------------------------------*/

#endif /* #ifndef __USERMANAGER_H__ */