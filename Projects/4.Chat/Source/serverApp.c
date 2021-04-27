
#include <stdlib.h> /*malloc, free*/
#include <string.h> /* strcpy */
#include "../Include/serverApp.h"
#include "../Include/UserManager.h"
/*#include "../Include/GroupManager.h"*/
#include "../Include/MessagesProtocolParser.h"
#include "../Include/serverDRV.h"

/* ************* DEBUG ****************** */
#include <stdio.h>
/* ************* DEBUG ****************** */

#define SERVER_PORT 5555
#define BUFFER_SIZE 4096

struct ServerApp
{
    UserManager* m_users;
    /*GroupManager* m_groups;*/
};


/* static function*/
static void SingleNewClient(Server* _server, int _clientSocket, void* _appContext);
static void SingleGotMessage(Server* _server, void* _message, int _SizeMessage, int _clientSock, void* _appContext);
static void SingleCloseClient(Server* _server, int _clientSocket, void* _appContext);
static int SingleFail(ServerResult _error, void* _appContext);
static void SendResponseMessage(Server* _server, int _clientSock, ResponseResult _responseResult, Byte* _buffer, char* _responseMessage, ProtocolParsingTag _tag);



ServerApp* ServerAppCreate()
{
    ServerApp* serverApp;
    /*GroupManager* groups;*/
    UserManager* users = UserManagerCreate();
    if(users == NULL)
    {
        return NULL;
    }
    /*groups = GroupManagerCreate();
    if(groups == NULL)
    {
        UserManagerDestroy(&users);
        return NULL;
    }
    */
   serverApp = (ServerApp*)malloc(sizeof(ServerApp));
   if(serverApp == NULL)
   {
       /*GroupManagerDestroy(&groups);*/
       UserManagerDestroy(&users);
       return NULL;
   }
   serverApp->m_users = users;
   /*serverApp->m_groups = groups;*/
   return serverApp;
}




void ServerAppDestroy(ServerApp** _serverApp)
{
    if(_serverApp != NULL && *_serverApp != NULL)
    {
       /*GroupManagerDestroy(&(*_serverApp)->m_groups);*/
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
    appFunction.m_newClientFunction = SingleNewClient;
    appFunction.m_gotMessageFunction = SingleGotMessage;
    appFunction.m_closeClientFunction = SingleCloseClient;
    appFunction.m_failFunction = SingleFail;
    serverNet = ServerCreate(&appFunction, 10, SERVER_PORT, 1023, _serverApp);
    if(serverNet == NULL)
    {
        return;
    }
    ServerRun(serverNet);
    ServerDestroy(&serverNet);
}





static void SingleNewClient(Server* _server, int _clientSocket, void* _appContext)
{

    /*ServerResult ServerSendMessage(Server* _server, size_t _clientSocket, void* _message, size_t _messageSize);*/
}

static void SingleGotMessage(Server* _server, void* _message, int _SizeMessage, int _clientSock, void* _appContext)
{
    Byte buffer[BUFFER_SIZE];
    size_t bytesToSend;
    ProtocolParsingTag tagType;
    SignUpRequest objSignUpRequest;
    LoginRequest objLoginRequest;
    UserManagerStatus statusResult;
    tagType = GetMessageTag((Byte*)_message);
    switch (tagType)
    {
        case SIGN_UP_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", SIGN_UP_RESPONSE);
            }

            UnPackMessageByTag((SignUpRequest*)&objSignUpRequest, _message, SIGN_UP_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Sign Up Request: %s, %s\n", objSignUpRequest.m_usernameToSignUp, objSignUpRequest.m_password);
            /* ************* DEBUG ****************** */

            if(( statusResult = UserManagerAddUser(((ServerApp*)_appContext)->m_users, objSignUpRequest.m_usernameToSignUp, objSignUpRequest.m_password, 1)) == USERMANAGER_USERNAME_ALREADY_EXISTS)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USERNAME_ALREADY_EXISTS, buffer, "UserName already exists", SIGN_UP_RESPONSE);
            }
            else if(statusResult == USERMANAGER_ALLOCATION_FAILED)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_MEMORY_ALLOCATION_FAILED, buffer, "System error", SIGN_UP_RESPONSE);
            }
            else
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "The user has registered successfully", SIGN_UP_RESPONSE);
            }

            break;

        case LOGIN_REQUEST:
            if(IsCorrectMessageSize(_message, _SizeMessage) == 0)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, buffer, "Partial message received", LOGIN_RESPONSE);
            }

            UnPackMessageByTag((LoginRequest*)&objLoginRequest, _message, LOGIN_REQUEST);

            /* ************* DEBUG ****************** */
            printf("The Server got a Login Request: %s, %s\n", objLoginRequest.m_usernameToLogin, objLoginRequest.m_password);
            /* ************* DEBUG ****************** */

            if( (statusResult = UserManagerCheckIfUserIsExistsAndIfPasswordIsCorrect(((ServerApp*)_appContext)->m_users, objLoginRequest.m_usernameToLogin, objLoginRequest.m_password)) == USERMANAGER_USERNAME_NOT_FOUND)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USERNAME_NOT_EXISTS, buffer, "username not exists", LOGIN_RESPONSE);
            }
            else if(statusResult == /*USERMANAGER_WRONG_PASSWORD*/1)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_WRONG_PASSWORD, buffer, "wrong password", LOGIN_RESPONSE);
            }
            else if(UserManagerCheckIfUserIsConnected(((ServerApp*)_appContext)->m_users, objLoginRequest.m_usernameToLogin) == 1)
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_USER_ALREADY_LOGGED_IN, buffer, "user already logged in", LOGIN_RESPONSE);
            }
            else
            {
                SendResponseMessage(_server, _clientSock, RESPONSE_OK, buffer, "user logged in successfully", LOGIN_RESPONSE);
            }
            break;

        default:
            break;
    }
}

static void SingleCloseClient(Server* _server, int _clientSocket, void* _appContext)
{

}

/*if return value 0 server stop
	if is 1 continu*/
static int SingleFail(ServerResult _error, void* _appContext)
{
    return 1;
}


static void SendResponseMessage(Server* _server, int _clientSock, ResponseResult _responseResult, Byte* _buffer, char* _responseMessage, ProtocolParsingTag _tag)
{
    void* obj = NULL;
    /* ADD ALL OBJ RESPONSE*/
    SignUpResponse objSignUpResponse;
    LoginResponse objLoginResponse;
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

    default:
        return;
    }

    bytesToSend = PackMessageByTag(obj, _buffer, _tag);
    ServerSendMessage(_server, _clientSock, _buffer, bytesToSend);
}