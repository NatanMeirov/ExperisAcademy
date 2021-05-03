/**
 * @file GroupManager.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Group Manager module that would be used for inner management of all the groups in the chat application
 * @version 1.0
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __GROUPMANAGER_H__
#define __GROUPMANAGER_H__


/* Defines: */

typedef struct GroupManager GroupManager;

typedef enum GroupManagerResult
{
    GROUPMANAGER_SUCCESS,
    GROUPMANAGER_NOT_INITIALIZED,
    GROUPMANAGER_ALLOCATION_FAILED,
    GROUPMANAGER_GROUP_NOT_FOUND,
    GROUPMANAGER_GROUP_ALREADY_EXISTS,
    GROUPMANAGER_ALL_GROUPS_ARE_IN_USE,
    GROUPMANAGER_USER_NOT_FOUND_IN_GROUP
    /* GROUPMANAGER_USER_ALREADY_IN_GROUP (Future implementation - after adding a LinkedList of users in ChatGroup object) */
} GroupManagerResult;


/* ---------------------------------- Main API Functions ------------------------------------- */

/* NULL on failure */
GroupManager* GroupManagerCreate();

void GroupManagerDestroy(GroupManager** _groupManager);

/* Important Note: While user wants to create a group - the server should check first if its an existing group, then make a new group (save the returned IP address string), and then add user to group */
GroupManagerResult GroupManagerMakeNewGroup(GroupManager* _groupManager, const char* _groupName);

const char* GroupManagerGetGroupMulticastIpAddress(GroupManager* _groupManager, const char* _groupName);

GroupManagerResult GroupManagerAddUserToGroup(GroupManager* _groupManager, const char* _groupName, const char* _user);

GroupManagerResult GroupManagerRemoveUserFromGroup(GroupManager* _groupManager, const char* _groupName, const char* _user); /*---> if userCount == 0 (check twice!!!) before and AFTER remove - destroy Group and return address to pool // Static function - clean group if there are 0 members*/

/*GroupManagerResult GroupManagerGetAllGroupsNamesByRef (use big buffer) */

/* --------------------------------End of Main API Functions ---------------------------------- */

#endif /* #ifndef __GROUPMANAGER_H__ */
