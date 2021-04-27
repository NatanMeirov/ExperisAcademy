#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strlen, strcpy */
#include "../Include/doubleLinkedList.h"
#include "../Include/UserInformation.h"


struct UserInformation
{
    char m_username[MAX_LEN];
    char m_password[MAX_LEN];
    int m_isConnected;
    List* m_joinedGroups;
};


static void DestroySingleGroupName(void* _groupName);


UserInformation* UserInformationCreate(char* _username, char* _password, int _isConnected)
{
    UserInformation* userInfo = NULL;

    if(!_username || !_password)
    {
        return NULL;
    }

    if((strlen(_username) + 1) > MAX_LEN || (strlen(_password) + 1) > MAX_LEN)
    {
        return NULL;
    }

    userInfo = (UserInformation*)malloc(sizeof(UserInformation));
    if (userInfo == NULL)
    {
        return NULL;
    }

    userInfo->m_isConnected = _isConnected;
    strcpy(userInfo->m_username, _username);
    strcpy(userInfo->m_password, _password);

    userInfo->m_joinedGroups = ListCreate();
    if(!userInfo->m_joinedGroups)
    {
        free(userInfo);

        return NULL;
    }

    return userInfo;
}


void UserInformationDestroy(UserInformation** _user)
{
    if (_user != NULL && *_user != NULL)
    {
        ListDestroy(&(*_user)->m_joinedGroups, &DestroySingleGroupName);

        free(*_user);
        *_user = NULL;
    }
}


const char* UserInformationGetUsername(UserInformation* _user)
{
    if (_user == NULL)
    {
        return NULL;
    }

    return _user->m_username;
}


const char* UserInformationGetPassword(UserInformation* _user)
{
    if (_user == NULL)
    {
        return NULL;
    }

    return _user->m_password;
}


int UserInformationGetIsConnected(UserInformation* _user)
{
    if (_user == NULL)
    {
        return 0;
    }

    return _user->m_isConnected;
}


int UserInformationSetIsConnected(UserInformation* _user, int _isConnected)
{
    if (_user == NULL)
    {
        return 0;
    }

    _user->m_isConnected = _isConnected;

    return 1;
}


int UserInformationAddGroup(UserInformation* _user, char* _groupName)
{
    char* group = NULL;
    int result;

    if (_user == NULL || _groupName == NULL)
    {
        return 0;
    }

    group = (char*)malloc(strlen(_groupName) + 1);
    if (group == NULL)
    {
        return 0;
    }

    strcpy(group, _groupName);

    result = ListPushTail(_user->m_joinedGroups , group);

    if (result != LIST_SUCCESS)
    {
        free(group);

        return 0;
    }

    return 1;
}


/*int UserInformationRemoveGroup(UserInformation* _user, char* _groupName)
{
    if (_user == NULL || _groupName == NULL)
    {
        return 0;
    } 
} */


static void DestroySingleGroupName(void* _groupName)
{
    free(_groupName);
}