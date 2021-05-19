/**
 * @file GroupManager.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Group Manager module that would be used for inner management of all the groups in the chat application
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp, strcpy, strcat */
#include <stdio.h> /* sprintf */
#include "../Include/GroupManager.h"
#include "../Include/ChatGroup.h"
#include "../Include/GenericHashMap.h"
#include "../Include/GenericQueue.h"
#include "../Include/DictStringKeyUtils.h"


/* Defines: */

#define MAX_MULTICAST_IP_ADDRESS_LENGTH 12 /* Including the '\0' */
#define DICTIONARY_INITIAL_CAPACITY 40 /* Maximum: 100 opened groups at the same time */
#define MAXIMUM_AVAILABLE_MULTICAST_IP_ADDRESSES 100

struct GroupManager
{
    HashMap* m_activeGroupsDictionary;
    Queue* m_multicastAddressesPool; /* Multicast: "224.0.0.{1-100}" */
};


/* Static Functions Declarations: */

static void DestroySingleMulticastIpAddress(void* _ipAddress);
static void DestroySingleChatGroup(void* _chatGroup);
static int InitialMulticastIpAddressesPool(Queue* _ipPool);
static GroupManagerResult MapQueueResultToGroupManagerResult(QueueResult _result);
static GroupManagerResult MapChatGroupResultToGroupManagerResult(ChatGroupResult _result);
static GroupManagerResult MapHashMapResultToGroupManagerResult(HashMapResult _result);
static void DestroyGroupAndReturnIpAddressToPool(ChatGroup** _groupToDestroy, Queue* _ipPool);


/* --------------------------------------- Main API Functions ----------------------------------------*/

GroupManager* GroupManagerCreate()
{
    GroupManager* groupManager = malloc(sizeof(GroupManager));
    if(!groupManager)
    {
        return NULL;
    }

    groupManager->m_activeGroupsDictionary = HashMapCreate(DICTIONARY_INITIAL_CAPACITY, &StringKeyHash, &AreEqualStringKeys);
    if(!groupManager->m_activeGroupsDictionary)
    {
    	free(groupManager);

        return NULL;
    }

    groupManager->m_multicastAddressesPool = QueueCreate(MAXIMUM_AVAILABLE_MULTICAST_IP_ADDRESSES);
    if(!groupManager->m_multicastAddressesPool)
    {
        HashMapDestroy(&groupManager->m_activeGroupsDictionary, NULL, NULL);

        free(groupManager);

        return NULL;
    }

    if(!InitialMulticastIpAddressesPool(groupManager->m_multicastAddressesPool))
    {
        HashMapDestroy(&groupManager->m_activeGroupsDictionary, NULL, NULL);
        QueueDestroy(&groupManager->m_multicastAddressesPool, &DestroySingleMulticastIpAddress);

        free(groupManager);

        return NULL;
    }

    return groupManager;
}


void GroupManagerDestroy(GroupManager** _groupManager)
{
    if(_groupManager && *_groupManager)
    {
        HashMapDestroy(&(*_groupManager)->m_activeGroupsDictionary, NULL, &DestroySingleChatGroup);
        QueueDestroy(&(*_groupManager)->m_multicastAddressesPool, &DestroySingleMulticastIpAddress);

        free(*_groupManager);
        *_groupManager = NULL;
    }
}


GroupManagerResult GroupManagerMakeNewGroup(GroupManager* _groupManager, const char* _groupName)
{
    ChatGroup* newChatGroup = NULL;
    char* ipAddress = NULL;
    char* ptrToGroupNameAsKey = NULL;
    ChatGroup* tempCheckGroup = NULL;
    QueueResult queueResultCode;
    HashMapResult hashMapResultCode;
    ChatGroupResult chatGroupResultCode;

    if(!_groupManager || !_groupName)
    {
        return GROUPMANAGER_NOT_INITIALIZED;
    }

    if(HashMapFind(_groupManager->m_activeGroupsDictionary,_groupName, (void**)&tempCheckGroup) != HASHMAP_KEY_NOT_FOUND_ERROR) /* Found an existing group with the same name */
    {
        return GROUPMANAGER_GROUP_ALREADY_EXISTS;
    }

    if(QueueIsEmpty(_groupManager->m_multicastAddressesPool))
    {
        return GROUPMANAGER_ALL_GROUPS_ARE_IN_USE;
    }

    if((queueResultCode = QueueRemove(_groupManager->m_multicastAddressesPool, (void**)&ipAddress)) != QUEUE_SUCCESS)
    {
        return MapQueueResultToGroupManagerResult(queueResultCode);
    }

    newChatGroup = ChatGroupCreate(_groupName, ipAddress);
    if(!newChatGroup)
    {
        QueueInsert(_groupManager->m_multicastAddressesPool, ipAddress);

        return GROUPMANAGER_ALLOCATION_FAILED; /* Could be an error because of a not valid string length (more then 100) */
    }

    ptrToGroupNameAsKey = (char*)ChatGroupGetGroupName(newChatGroup); /* To get its heap allocated address (and not use the given stack allocated _groupName address) */

    if((hashMapResultCode = HashMapInsert(_groupManager->m_activeGroupsDictionary, ptrToGroupNameAsKey, newChatGroup)) != HASHMAP_SUCCESS)
    {
        DestroyGroupAndReturnIpAddressToPool(&newChatGroup, _groupManager->m_multicastAddressesPool);
    }

    return MapHashMapResultToGroupManagerResult(hashMapResultCode);
}


const char* GroupManagerGetGroupMulticastIpAddress(GroupManager* _groupManager, const char* _groupName)
{
    ChatGroup* group = NULL;

    if(!_groupManager || !_groupName)
    {
        return NULL;
    }

    if(HashMapFind(_groupManager->m_activeGroupsDictionary, _groupName, (void**)&group))
    {
        return NULL;
    }

    return ChatGroupGetMulticastIpAddress(group);
}


GroupManagerResult GroupManagerAddUserToGroup(GroupManager* _groupManager, const char* _groupName, const char* _user)
{
    HashMapResult result;
    ChatGroup* group = NULL;

    if(!_groupManager || !_groupName)
    {
        return GROUPMANAGER_NOT_INITIALIZED;
    }

    if((result = HashMapFind(_groupManager->m_activeGroupsDictionary, _groupName, (void**)&group)) != HASHMAP_SUCCESS)
    {
        return MapHashMapResultToGroupManagerResult(result);
    }

    return MapChatGroupResultToGroupManagerResult(ChatGroupAddUserToGroup(group, _user));
}


GroupManagerResult GroupManagerRemoveUserFromGroup(GroupManager* _groupManager, const char* _groupName, const char* _user)
{
    HashMapResult result;
    ChatGroup* group = NULL;
    void* keyPtr = NULL; 
    size_t usersInGroup;


    if(!_groupManager || !_groupName || !_user)
    {
        return GROUPMANAGER_NOT_INITIALIZED;
    }

    if((result = HashMapRemove(_groupManager->m_activeGroupsDictionary, _groupName, &keyPtr, (void**)&group)) != HASHMAP_SUCCESS)
    {
        return MapHashMapResultToGroupManagerResult(result);
    }

    usersInGroup = ChatGroupGetActiveUsersCount(group);

    if(!usersInGroup)
    {
        DestroyGroupAndReturnIpAddressToPool(&group, _groupManager->m_multicastAddressesPool);

        return GROUPMANAGER_SUCCESS;
    }

    ChatGroupRemoveUserFromGroup(group, _user); /* Group cannot be empty after checking */
    usersInGroup--; /* To avoid calling the function again */

    if(!usersInGroup)
    {
        DestroyGroupAndReturnIpAddressToPool(&group, _groupManager->m_multicastAddressesPool);
    }

    return GROUPMANAGER_SUCCESS;
}

/* ----------------------------------- End of Main API Functions -------------------------------------*/


/* Static Functions Implementations: */

static void DestroySingleMulticastIpAddress(void* _ipAddress)
{
    free(_ipAddress);
}


static void DestroySingleChatGroup(void* _chatGroup) /* While destroying the HashMap - destroy the ip addresses as well */
{
    char* ipAddress = NULL;
    ChatGroupDestroy((ChatGroup**)&_chatGroup, &ipAddress);
    free(ipAddress);
}


static int InitialMulticastIpAddressesPool(Queue* _ipPool)
{
    int i;
    char numToStringBuffer[20]; /* Did not have a way to check the perfect length... */
    char startingMulticastIpAddressTemplate[MAX_MULTICAST_IP_ADDRESS_LENGTH] = "224.0.0.";
    char* tempIp = NULL;

    for(i = 1; i <= MAXIMUM_AVAILABLE_MULTICAST_IP_ADDRESSES; i++)
    {
        tempIp = malloc(MAX_MULTICAST_IP_ADDRESS_LENGTH);
        if(!tempIp)
        {
            return 0;
        }

        sprintf(numToStringBuffer, "%d", i);

        strcpy(tempIp, startingMulticastIpAddressTemplate);
        strcat(tempIp, numToStringBuffer);

        if(QueueInsert(_ipPool, tempIp) != QUEUE_SUCCESS)
        {
            return 0;
        }
    }

    return 1;
}


static GroupManagerResult MapQueueResultToGroupManagerResult(QueueResult _result)
{
    if(_result == QUEUE_UNDERFLOW_ERROR)
    {
        return GROUPMANAGER_ALL_GROUPS_ARE_IN_USE;
    }

    return GROUPMANAGER_SUCCESS;
}


static GroupManagerResult MapChatGroupResultToGroupManagerResult(ChatGroupResult _result)
{
    if(_result == CHATGROUP_ALLOCATION_FAILED)
    {
        return GROUPMANAGER_ALLOCATION_FAILED;
    }
    else if(_result == CHATGROUP_GROUP_IS_EMPTY)
    {
        return GROUPMANAGER_USER_NOT_FOUND_IN_GROUP;
    }

    return GROUPMANAGER_SUCCESS;
}


static GroupManagerResult MapHashMapResultToGroupManagerResult(HashMapResult _result)
{
    if (_result == HASHMAP_KEY_DUPLICATE_ERROR)
    {
        return GROUPMANAGER_GROUP_ALREADY_EXISTS;
    }
    else if(_result == HASHMAP_ALLOCATION_ERROR)
    {
        return GROUPMANAGER_ALLOCATION_FAILED;
    }
    else if(_result == HASHMAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUPMANAGER_GROUP_NOT_FOUND;
    }

    /* else: _result == HASHMAP_SUCCESS */

    return GROUPMANAGER_SUCCESS;
}


static void DestroyGroupAndReturnIpAddressToPool(ChatGroup** _groupToDestroy, Queue* _ipPool)
{
    char* ipAddress = NULL;

    ChatGroupDestroy(_groupToDestroy, &ipAddress);
    QueueInsert(_ipPool, ipAddress);
}
