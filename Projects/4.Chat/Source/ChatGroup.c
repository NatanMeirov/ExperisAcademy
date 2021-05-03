/**
 * @file ChatGroup.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An Chat Group object, to store the group's data in
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strlen, strcpy */
#include "../Include/ChatGroup.h"
/* (Can be used in the future)
#include "../Include/GenericLinkedList.h"
#include "../Include/LinkedListIterator.h"
#include "../Include/LinkedListIteratorFunctions.h"
*/


/* Defines: */

#define UNUSED(x) ((void)(x))

struct ChatGroup
{
    char m_nameOfGroup[MAX_GROUP_NAME_LENGTH];
    char* m_multicastIpAddress;
    size_t m_activeUsersCountInGroup;
    /* LinkedList* m_activeUsersInGroupList; (Can be added in the future) */
};


/* ---------------------------------- Main API Functions ------------------------------------- */

ChatGroup* ChatGroupCreate(const char* _nameOfGroup, char* _multicastIpAddress)
{
    ChatGroup* group = NULL;

    if(!_nameOfGroup || !_multicastIpAddress)
    {
        return NULL;
    }

    if(strlen(_nameOfGroup) + 1 > MAX_GROUP_NAME_LENGTH || strlen(_multicastIpAddress) > MAXIMUM_IPV4_LENGTH) /* MAXIMUM_IPV4_LENGTH is including the '\0' so no need to add +1 */
    {
        return NULL;
    }

    group = malloc(sizeof(ChatGroup));
    if(!group)
    {
        return NULL;
    }

    group->m_multicastIpAddress = _multicastIpAddress; /* Saving the multicast ip address' pointer */

    strcpy(group->m_nameOfGroup, _nameOfGroup);
    group->m_activeUsersCountInGroup = 0;

    /* group->m_activeUsersInGroupList = LinkedListCreate();
    if(!group->m_activeUsersInGroupList)
    {
        free(group);

        return NULL;
    }
    */

    return group;
}


void ChatGroupDestroy(ChatGroup** _group, char** _ipAddressToReturnBack)
{
    if(_group && *_group)
    {
        *_ipAddressToReturnBack = (*_group)->m_multicastIpAddress; /* Used for returning the ip back to the pool */

        free(*_group);
        *_group = NULL;
    }
}


const char* ChatGroupGetMulticastIpAddress(ChatGroup* _group)
{
    if(!_group)
    {
        return NULL;
    }

    return _group->m_multicastIpAddress;
}


const char* ChatGroupGetGroupName(ChatGroup* _group)
{
    if(!_group)
    {
        return NULL;
    }

    return _group->m_nameOfGroup;
}


ChatGroupResult ChatGroupRemoveUserFromGroup(ChatGroup* _group, const char* _user)
{
    if(!_group || !_user)
    {
        return CHATGROUP_NOT_INITIALIZED;
    }

    UNUSED(_user); /* For now - not using it */
    /* In future implementation: Check for user in the list, and only if found - Remove / else error */

    if(!_group->m_activeUsersCountInGroup)
    {
        return CHATGROUP_GROUP_IS_EMPTY;
    }

    _group->m_activeUsersCountInGroup--;

    return CHATGROUP_SUCCESS;
}


ChatGroupResult ChatGroupAddUserToGroup(ChatGroup* _group, const char* _user)
{
    if(!_group || !_user)
    {
        return CHATGROUP_NOT_INITIALIZED;
    }

    UNUSED(_user); /* For now - not using it */
    /* In future implementation: Check for user in the list, and only if not found - Add / else error */

    _group->m_activeUsersCountInGroup++;

    return CHATGROUP_SUCCESS;
}


size_t ChatGroupGetActiveUsersCount(ChatGroup* _group)
{
    if(!_group)
    {
        return MAX_SIZE_T;
    }

    return _group->m_activeUsersCountInGroup;
}

/* --------------------------------End of Main API Functions ---------------------------------- */
