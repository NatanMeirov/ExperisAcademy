/* Zahi (& Natan) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strlen, strcpy */
#include "../Include/UserInformation.h"
#include "../Include/GenericLinkedList.h"
#include "../Include/LinkedListIterator.h"
#include "../Include/LinkedListIteratorFunctions.h"

struct UserInformation
{
    char m_username[MAX_LEN];
    char m_password[MAX_LEN];
    int m_isConnected;
    LinkedList* m_joinedGroups;
};

static void DestroySingleGroupName(void* _groupName);
static int FindMatchedGroupNames(void* _elementToCheck,void* _groupName);


/* API: */

UserInformation* UserInformationCreate(const char* _username, const char* _password, int _isConnected)
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

    userInfo->m_joinedGroups = LinkedListCreate();
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
        LinkedListDestroy(&(*_user)->m_joinedGroups, &DestroySingleGroupName);

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


UserInformationResult UserInformationSetIsConnected(UserInformation* _user, int _isConnected)
{
    if (_user == NULL)
    {
        return USERINFORMATION_NOT_INITIALIZED;
    }

    _user->m_isConnected = _isConnected;

    return USERINFORMATION_SUCCESS;
}


UserInformationResult UserInformationAddGroup(UserInformation* _user, const char* _groupName)
{
    char* group = NULL;
    LinkedListResult result;
    LinkedListIterator iterator = NULL;
    LinkedListIterator endIterator = NULL;

    if (_user == NULL || _groupName == NULL)
    {
        return USERINFORMATION_NOT_INITIALIZED;
    }

    endIterator = LinkedListIteratorEnd(_user->m_joinedGroups);

    if(LinkedListIteratorFindFirst(LinkedListIteratorBegin(_user->m_joinedGroups), endIterator, &FindMatchedGroupNames, (void*)_groupName) != endIterator) /* Found the same groupName in the LinkedList of groups */
    {
        return USERINFORMATION_GROUP_ALREADY_EXISTS;
    }

    group = (char*)malloc(strlen(_groupName) + 1);
    if (group == NULL)
    {
        return USERINFORMATION_ALLOCATION_FAILED;
    }

    strcpy(group, _groupName);

    result = LinkedListInsertTail(_user->m_joinedGroups, group);

    if(result != LINKEDLIST_SUCCESS)
    {
        free(group);

        return USERINFORMATION_ALLOCATION_FAILED;
    }

    return USERINFORMATION_SUCCESS;
}


UserInformationResult UserInformationRemoveGroup(UserInformation* _user, const char* _groupName)
{
    char* groupPtr = NULL;
    LinkedListIterator* findGroupNameIterator = NULL;
    LinkedListIterator* endIterator = NULL;


    if (_user == NULL || _groupName == NULL)
    {
        return USERINFORMATION_NOT_INITIALIZED;
    }

    endIterator = LinkedListIteratorEnd(_user->m_joinedGroups);

    findGroupNameIterator = LinkedListIteratorFindFirst(LinkedListIteratorBegin(_user->m_joinedGroups), endIterator, &FindMatchedGroupNames, (void*)_groupName);

    if(findGroupNameIterator == endIterator)
    {
        return USERINFORMATION_GROUP_NOT_FOUND;
    }

    groupPtr = LinkedListIteratorRemove(findGroupNameIterator); /* Gets the allocated groupname string */
    free(groupPtr);

    return USERINFORMATION_SUCCESS;
}


/* Static */

static int FindMatchedGroupNames(void* _elementToCheck, void* _groupName)
{
    return (strcmp((char*)_elementToCheck ,(char*)_groupName) == 0) ? 1 : 0;
}

static void DestroySingleGroupName(void* _groupName)
{
    free(_groupName);
}