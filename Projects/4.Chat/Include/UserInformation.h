/**
 * @file UserInformation.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An User Information object, to store the user's data in
 * @version 1.0
 * @date 2021-04-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __USERINFORMATION_H__
#define __USERINFORMATION_H__


/* Defines: */

#define MAX_LEN 100

typedef struct UserInformation UserInformation;

typedef int Bool;

typedef enum UserInformationResult
{
    USERINFORMATION_SUCCESS,
    USERINFORMATION_NOT_INITIALIZED,
    USERINFORMATION_ALLOCATION_FAILED,
    USERINFORMATION_GROUP_NOT_FOUND,
    USERINFORMATION_GROUP_ALREADY_EXISTS
} UserInformationResult;

/* ---------------------------------- Main API Functions ------------------------------------- */

/* NULL on failure (validates that _username and _password will not be > MAX_LEN) */
UserInformation* UserInformationCreate(const char* _username, const char* _password, int _isConnected);

void UserInformationDestroy(UserInformation** _user);

/* NULL on failure */
const char* UserInformationGetUsername(UserInformation* _user);

/* NULL on failure */
const char* UserInformationGetPassword(UserInformation* _user);

/* TRUE (1) or FALSE (0) */
Bool UserInformationGetIsConnected(UserInformation* _user);

/* USERINFORMATION_SUCCESS, USERINFORMATION_NOT_INITIALIZED */
UserInformationResult UserInformationSetIsConnected(UserInformation* _user, int _isConnected);

/* USERINFORMATION_SUCCESS, USERINFORMATION_NOT_INITIALIZED, USERINFORMATION_ALLOCATION_FAILED */
UserInformationResult UserInformationAddGroup(UserInformation* _user, const char* _groupName);

/* USERINFORMATION_SUCCESS, USERINFORMATION_NOT_INITIALIZED, USERINFORMATION_GROUP_NOT_FOUND */
UserInformationResult UserInformationRemoveGroup(UserInformation* _user, const char* _groupName);


/* --------------------------------End of Main API Functions ---------------------------------- */

#endif /* #ifndef __USERINFORMATION_H__ */