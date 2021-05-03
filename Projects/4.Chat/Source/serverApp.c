#include <stdlib.h> /*malloc, free*/
#include <string.h> /* strcpy */
#include "../Include/serverApp.h"
#include "../Include/UserManager.h"
#include "../Include/GroupManager.h"
#include "../Include/MessagesProtocolParser.h"
#include "../Include/serverDRV.h"

/* ************* DEBUG ****************** */
#include <stdio.h>
/* ************* DEBUG ****************** */

#define SERVER_PORT 5555
#define BUFFER_SIZE 4096
#define IP_LEN 16
#define CHAT_APP_PORT 12345 /* Port to be used for the chats */

struct ServerApp
{
    UserManager* m_users;
    GroupManager* m_groups;
};


/* static function*/
static void SingleNewClient(Server* _server, int _clientSocket, void* _appContext);
static void SingleGotMessage(Server* _server, void* _message, int _SizeMessage, int _clientSock, void* _appContext);
static void SingleCloseClient(Server* _server, int _clientSocket, void* _appContext);
static int SingleFail(ServerResult _error, void* _appContext);
static void SendResponseMessage(Server* _server, int _clientSock, ResponseResult _responseResult, Byte* _buffer, char* _responseMessage, char* _context, ProtocolParsingTag _tag);



ServerApp* ServerAppCreate()
{
    ServerApp* serverApp;
    GroupManager* groups;
    UserManager* users = UserManagerCreate();
    if(users == NULL)
    {
        return NULL;
    }
    groups = GroupManagerCreate();
    if(groups == NULL)
    {
        UserManagerDestroy(&users);
        return NULL;
    }

   serverApp = (ServerApp*)malloc(sizeof(ServerApp));
   if(serverApp == NULL)
   {
       GroupManagerDestroy(&groups);
       UserManagerDestroy(&users);
       return NULL;
   }
   serverApp->m_users = users;
   serverApp->m_groups = groups;
   return serverApp;
}




void ServerAppDestroy(ServerApp** _serverApp)
{
    if(_serverApp != NULL && *_serverApp != NULL)
    {
       GroupManagerDestroy(&(*_serverApp)->m_groups);
       UserManagerDestroy(&(*_serverApp)->m_users);

       free(*_serverApp);
       *_serverApp = NULL;
    }
}


void ServerAppRun(ServerApp* _serverApp)
{
    Server* serverNet;
    AppFunction appFunction;
    if(_serverApp == NULL)
    {
        return;
    }
    appFunction.m_newClientFunction = NULL;
    appFunction.m_gotMessageFunction = SingleGotMessage;
    appFunction.m_closeClientFunction = NULL;
    appFunction.m_failFunction = NULL;
    serverNet = ServerCreate(&appFunction, 10, SERVER_PORT, 1023, _serverApp);
    if(serverNet == NULL)
    {
        return;
    }
    ServerRun(serverNet);
    ServerDestroy(&serverNet);
}


static void SingleGotMessage(Server* _server, void* _message, int _SizeMessage, int _clientSock, void* _appContext)
{
    Byte buffer[BUFFER_SIZE];
    char multicastAddress[IP_LEN];
    size_t bytesToSend;
    ProtocolParsingTag tagType;
    SignUpRequest objSignUpRequest;
    LoginRequest objLoginRequest;
    LogoutRequest objLogoutRequest;
    CreateGroupRequest objCreateGroupRequest;
    JoinGroupRequest objJoinGroupRequest;
    LeaveGroupRequest objLeaveGroupRequest;

    UserManagerStatus statusUserManagerResult;
    GroupManagerResult statusGroupManagerResult;
    tagType = GetMessageTag((Byte*)_message);
    switch (tagType)
    {
        case SIGN_UP_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", NULL,SIGN_UP_RESPONSE);
            }

            UnPackMessageByTag((SignUpRequest*)&objSignUpRequest, _message, SIGN_UP_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Sign Up Request: %s, %s\n", objSignUpRequest.m_usernameToSignUp, objSignUpRequest.m_password);
            /* ************* DEBUG ****************** */

            if((statusUserManagerResult = UserManagerAddUser(((ServerApp*)_appContext)->m_users, objSignUpRequest.m_usernameToSignUp, objSignUpRequest.m_password, 0)) == USERMANAGER_USERNAME_ALREADY_EXISTS) /* Sign Up request WILL NOT CONNECT THE USER TO THE SYSTEM, till he will Login */
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USERNAME_ALREADY_EXISTS, buffer, "UserName already exists", NULL, SIGN_UP_RESPONSE);
            }
            else if(statusUserManagerResult == USERMANAGER_ALLOCATION_FAILED)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_MEMORY_ALLOCATION_FAILED, buffer, "System error", NULL, SIGN_UP_RESPONSE);
            }
            else
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "The user has registered successfully", NULL, SIGN_UP_RESPONSE);
            }
            break;

        case LOGIN_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", NULL, LOGIN_RESPONSE);
            }

            UnPackMessageByTag((LoginRequest*)&objLoginRequest, _message, LOGIN_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Login Request: %s, %s\n", objLoginRequest.m_usernameToLogin, objLoginRequest.m_password);
            /* ************* DEBUG ****************** */

            if( (statusUserManagerResult = UserManagerCheckIfUserIsExistsAndIfPasswordIsCorrect(((ServerApp*)_appContext)->m_users, objLoginRequest.m_usernameToLogin, objLoginRequest.m_password)) == USERMANAGER_USERNAME_NOT_FOUND)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USERNAME_NOT_EXISTS, buffer, "username not exists", NULL, LOGIN_RESPONSE);
            }
            else if(statusUserManagerResult == USERMANAGER_WRONG_PASSWORD)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_WRONG_PASSWORD, buffer, "wrong password", NULL, LOGIN_RESPONSE);
            }
            else if(UserManagerCheckIfUserIsConnected(((ServerApp*)_appContext)->m_users, objLoginRequest.m_usernameToLogin) == 1)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USER_ALREADY_LOGGED_IN, buffer, "user already logged in", NULL, LOGIN_RESPONSE);
            }
            else
            {
                UserManagerSetUserAsConnected(((ServerApp*)_appContext)->m_users, objLoginRequest.m_usernameToLogin);
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "user logged in successfully", NULL, LOGIN_RESPONSE);
            }
            break;

        case LOGOUT_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", NULL, LOGOUT_RESPONSE);
            }
            UnPackMessageByTag((LogoutRequest*)&objLogoutRequest, _message, LOGOUT_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a LogOut Request: %s\n", objLogoutRequest.m_usernameToLogout);
            /* ************* DEBUG ****************** */

            UserManagerSetUserAsDisconnected(((ServerApp*)_appContext)->m_users, objLogoutRequest.m_usernameToLogout);
            
            SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "user logged out successfully", NULL, LOGOUT_RESPONSE);
            break;

        case CREATE_GROUP_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", NULL, CREATE_GROUP_RESPONSE);
            }
            UnPackMessageByTag((CreateGroupRequest*)&objCreateGroupRequest, _message, CREATE_GROUP_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Create Group Request: %s, %s\n", objCreateGroupRequest.m_groupNameToCreate, objCreateGroupRequest.m_usernameOfGroupCreator);
            /* ************* DEBUG ****************** */

            if((statusGroupManagerResult = GroupManagerMakeNewGroup(((ServerApp*)_appContext)->m_groups, objCreateGroupRequest.m_groupNameToCreate)) == GROUPMANAGER_GROUP_ALREADY_EXISTS)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_GROUP_NAME_ALREADY_EXISTS, buffer, "Group already exists", NULL, CREATE_GROUP_RESPONSE);
            }
            else if(statusGroupManagerResult == GROUPMANAGER_ALLOCATION_FAILED)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_MEMORY_ALLOCATION_FAILED, buffer, "System error", NULL, CREATE_GROUP_RESPONSE);
            }
            else if(statusGroupManagerResult == GROUPMANAGER_ALL_GROUPS_ARE_IN_USE)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_ALL_AVAILABLE_GROUPS_ARE_IN_USE, buffer, "All groups are already in use", NULL, CREATE_GROUP_RESPONSE);
            }
            else if((UserManagerAddGroupToUser(((ServerApp*)_appContext)->m_users, objCreateGroupRequest.m_usernameOfGroupCreator, objCreateGroupRequest.m_groupNameToCreate)) == USERMANAGER_ALLOCATION_FAILED)
            {
                GroupManagerRemoveUserFromGroup(((ServerApp*)_appContext)->m_groups, objCreateGroupRequest.m_groupNameToCreate, objCreateGroupRequest.m_usernameOfGroupCreator);
                SendResponseMessage(_server, _clientSock, RESPONSE_MEMORY_ALLOCATION_FAILED, buffer, "System error", NULL, CREATE_GROUP_RESPONSE);
            }
            else /* Succeed to create a new group */
            {
                GroupManagerAddUserToGroup(((ServerApp*)_appContext)->m_groups, objCreateGroupRequest.m_groupNameToCreate, objCreateGroupRequest.m_usernameOfGroupCreator);
                strcpy(multicastAddress, GroupManagerGetGroupMulticastIpAddress(((ServerApp*)_appContext)->m_groups, objCreateGroupRequest.m_groupNameToCreate));
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "A new group has been successfully created", multicastAddress, CREATE_GROUP_RESPONSE);
            }
            break;

        case JOIN_GROUP_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", NULL, JOIN_GROUP_RESPONSE);
            }
            UnPackMessageByTag((JoinGroupRequest*)&objJoinGroupRequest, _message, JOIN_GROUP_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Join Group Request: %s, %s\n", objJoinGroupRequest.m_groupNameToJoin, objJoinGroupRequest.m_usernameToJoin);
            /* ************* DEBUG ****************** */

            if((statusUserManagerResult = UserManagerAddGroupToUser(((ServerApp*)_appContext)->m_users, objJoinGroupRequest.m_usernameToJoin, objJoinGroupRequest.m_groupNameToJoin)) == USERMANAGER_ALLOCATION_FAILED)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_MEMORY_ALLOCATION_FAILED, buffer, "System error", NULL, JOIN_GROUP_RESPONSE);
            }
            else if(statusUserManagerResult == USERMANAGER_GROUP_OF_USER_ALREADY_EXISTS)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USER_ALREADY_JOINED_GROUP, buffer, "User already joined the group", NULL, JOIN_GROUP_RESPONSE);
            }
            else if(GroupManagerAddUserToGroup(((ServerApp*)_appContext)->m_groups, objJoinGroupRequest.m_groupNameToJoin, objJoinGroupRequest.m_usernameToJoin) == GROUPMANAGER_GROUP_NOT_FOUND)
            {
                UserManagerRemoveGroupFromUser(((ServerApp*)_appContext)->m_users, objJoinGroupRequest.m_usernameToJoin, objJoinGroupRequest.m_groupNameToJoin);
                SendResponseMessage(_server, _clientSock, RESPONSE_GROUP_NAME_NOT_EXISTS, buffer, "Group name not exists", NULL, JOIN_GROUP_RESPONSE);
            }
            else
            {
                strcpy(multicastAddress, GroupManagerGetGroupMulticastIpAddress(((ServerApp*)_appContext)->m_groups, objJoinGroupRequest.m_groupNameToJoin));
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "Successfully joined the group", multicastAddress, JOIN_GROUP_RESPONSE);
            }

            break;

        case LEAVE_GROUP_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", NULL, LEAVE_GROUP_RESPONSE);
            }
            UnPackMessageByTag((LeaveGroupRequest*)&objLeaveGroupRequest, _message, LEAVE_GROUP_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Leave Group Request: %s, %s\n", objLeaveGroupRequest.m_groupNameToLeave, objLeaveGroupRequest.m_usernameToLeave);
            /* ************* DEBUG ****************** */

            if(UserManagerRemoveGroupFromUser(((ServerApp*)_appContext)->m_users, objLeaveGroupRequest.m_usernameToLeave, objLeaveGroupRequest.m_groupNameToLeave) == USERMANAGER_GROUP_OF_USER_NOT_FOUND)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_GROUP_NAME_NOT_EXISTS, buffer, "You do not exist in this group", NULL, LEAVE_GROUP_RESPONSE);
            }
            else
            {
                GroupManagerRemoveUserFromGroup(((ServerApp*)_appContext)->m_groups, objLeaveGroupRequest.m_groupNameToLeave, objLeaveGroupRequest.m_usernameToLeave);
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "You have been successfully removed from the group", NULL, LEAVE_GROUP_RESPONSE);
            }
            break;

        default:
            break;
    }
}


static void SendResponseMessage(Server* _server, int _clientSock, ResponseResult _responseResult, Byte* _buffer, char* _responseMessage, char* _context, ProtocolParsingTag _tag)
{
    void* obj = NULL;

    SignUpResponse objSignUpResponse;
    LoginResponse objLoginResponse;
    LogoutResponse objLogoutResponse;
    CreateGroupResponse objCreateGroupResponse;
    JoinGroupResponse objJoinGroupResponse;
    LeaveGroupResponse objLeaveGroupResponse;
    size_t bytesToSend;

    switch (_tag)
    {
    case SIGN_UP_RESPONSE:
        objSignUpResponse.m_responseResult = _responseResult;
        strcpy(objSignUpResponse.m_responseMessage, _responseMessage);
        obj = &objSignUpResponse;
        break;

    case LOGIN_RESPONSE:
        objLoginResponse.m_responseResult = _responseResult;
        strcpy(objLoginResponse.m_responseMessage, _responseMessage);
        obj = &objLoginResponse;
        break;

    case LOGOUT_RESPONSE:
        objLogoutResponse.m_responseResult = _responseResult;
        strcpy(objLogoutResponse.m_responseMessage, _responseMessage);
        obj = &objLogoutResponse;
        break;

    case CREATE_GROUP_RESPONSE:
        objCreateGroupResponse.m_responseResult = _responseResult;
        strcpy(objCreateGroupResponse.m_responseMessage, _responseMessage);
        if(_context != NULL)
        {
            strcpy(objCreateGroupResponse.m_multicastIPAddress, _context);
        }
        objCreateGroupResponse.m_port = CHAT_APP_PORT;
        obj = &objCreateGroupResponse;
        break;

    case JOIN_GROUP_RESPONSE:
        objJoinGroupResponse.m_responseResult = _responseResult;
        strcpy(objJoinGroupResponse.m_responseMessage, _responseMessage);
        if(_context != NULL)
        {
            strcpy(objJoinGroupResponse.m_multicastIPAddress, _context);
        }
        objJoinGroupResponse.m_port = CHAT_APP_PORT;
        obj = &objJoinGroupResponse;
        break;

    case LEAVE_GROUP_RESPONSE:
        objLeaveGroupResponse.m_responseResult = _responseResult;
        strcpy(objLeaveGroupResponse.m_responseMessage, _responseMessage);
        obj = &objLeaveGroupResponse;
        break;

    default:
        return;
    }

    bytesToSend = PackMessageByTag(obj, _buffer, _tag);
    ServerSendMessage(_server, _clientSock, _buffer, bytesToSend);
}