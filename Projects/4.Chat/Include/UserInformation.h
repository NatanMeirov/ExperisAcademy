/**
 * @file UserInformation.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An User Information object, to store the user's data in
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __USERINFORMATION_H__
#define __USERINFORMATION_H__


/* Defines: */

#define MAX_LEN 100

typedef struct UserInformation UserInformation;


/* ---------------------------------- Main API Functions ------------------------------------- */

/* Validating that maximum length of username and password are not more than MAX_LEN */
UserInformation* UserInformationCreate(char* _username, char* _password, int _isConnected);

void UserInformationDestroy(UserInformation** _user);

const char* UserInformationGetUsername(UserInformation* _user);

const char* UserInformationGetPassword(UserInformation* _user);

int UserInformationGetIsConnected(UserInformation* _user);

/* 1, on success / 0, on failure */
int UserInformationSetIsConnected(UserInformation* _user, int _isConnected);

/* 1, on success / 0, on failure */
int UserInformationAddGroup(UserInformation* _user, char* _groupName);

/* 1, on success / 0, on failure */
int UserInformationRemoveGroup(UserInformation* _user, char* _groupName);

/* --------------------------------End of Main API Functions ---------------------------------- */

#endif /* #ifndef __USERINFORMATION_H__ */