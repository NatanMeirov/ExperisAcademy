/**
 * @file ChatGroup.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An Chat Group object, to store the group's data in
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CHATGROUP_H__
#define __CHATGROUP_H__


/* Includes: */

#include <stddef.h> /* size_t */


/* Defines: */

#define MAXIMUM_IPV4_LENGTH 16
#define MAX_GROUP_NAME_LENGTH 100
#define MAX_SIZE_T ((size_t) -1)


typedef struct ChatGroup ChatGroup;

typedef enum ChatGroupResult
{
    CHATGROUP_SUCCESS,
    CHATGROUP_NOT_INITIALIZED,
    CHATGROUP_ALLOCATION_FAILED,
    CHATGROUP_GROUP_IS_EMPTY
    /*CHATGROUP_USER_NOT_FOUND_IN_GROUP,
    */
} ChatGroupResult;

/* ---------------------------------- Main API Functions ------------------------------------- */

/* NULL on failure (validates that _nameOfGroup will not be > MAX_GROUP_NAME_LENGTH and _multicastIpAddress will not be > MAXIMUM_IPV4_LENGTH) */
ChatGroup* ChatGroupCreate(const char* _nameOfGroup, char* _multicastIpAddress);

void ChatGroupDestroy(ChatGroup** _group, char** _ipAddressToReturnBack);

const char* ChatGroupGetMulticastIpAddress(ChatGroup* _group);

const char* ChatGroupGetGroupName(ChatGroup* _group);

ChatGroupResult ChatGroupRemoveUserFromGroup(ChatGroup* _group, const char* _user);

ChatGroupResult ChatGroupAddUserToGroup(ChatGroup* _group, const char* _user);

/* MAX_SIZE_T, on failure */
size_t ChatGroupGetActiveUsersCount(ChatGroup* _group);

/* int ChatGroupIsUserInGroup(ChatGroup* _group, const char* _user); (Future implementation with users list) */

/* --------------------------------End of Main API Functions ---------------------------------- */

#endif /* #ifndef __CHATGROUP_H__ */