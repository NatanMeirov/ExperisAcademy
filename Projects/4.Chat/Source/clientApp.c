#include <stdio.h> /* sprintf, fopen, fread, fclose */
#include <stdlib.h> /* system */
#include <string.h> /*strchr, strcpy*/
#include <stdlib.h> /*malloc, free*/
#include "../Include/clientApp.h"
#include "../Include/UI.h"
#include "../Include/clientNet.h"
#include "../Include/MessagesProtocolParser.h"
#include "../Include/GenericLinkedList.h"
#include "../Include/LinkedListIterator.h"
#include "../Include/LinkedListIteratorFunctions.h"


#define SUCCESS 1
#define FAILED 0
#define ERROR -1
#define TRUE 1

#define BUFFER_SIZE 4096
#define MAX_VALID_LEN 100



typedef enum StartMenuOption
{
    EXIT = 0,
    REGISTER,
    LOGIN
}StartMenuOption;

typedef enum SecondaryMenuOption
{
    LOG_OUT = 0,
    CREATE_GROUP,
    SIGN_UP_FOR_GROUP,
    DISCONNECT_FROM_GROUP
}SecondaryMenuOption;


typedef struct InternalClientApp
{
    char m_clientName[MAX_VALID_LEN];
    Client* m_client;
    LinkedList* m_pidsList;
}InternalClientApp;


typedef struct PIDsGroup
{
    char m_groupName[MAX_VALID_LEN];
    int m_pidChatReceive;
    int m_pidChatSend;
}PIDsGroup;



/* static function */
static int StartMenu(InternalClientApp* _clientAppObj);
static int Register(InternalClientApp* _clientAppObj);
static int IsValid(char* _clientdetail);
static int Login(InternalClientApp* _clientAppObj);
static void EnterUsernameAndPassword(void* _obj, ProtocolParsingTag _tag);
static int SendAndReceiveMessage(InternalClientApp* _clientAppObj, Byte* _buffer, int _sizePacking);
static int SecondaryMenu(InternalClientApp* _clientAppObj);
static void EnterGroupName(void* _obj, ProtocolParsingTag _tag);
static int CreateGroup(InternalClientApp* _clientAppObj);
static int JoinForGroup(InternalClientApp* _clientAppObj);
static int  DisconnectFromGroup(InternalClientApp* _clientAppObj);
static int LogoutFromUsername(InternalClientApp* _clientAppObj);
static int CompGroupName(void* _element, void* _context);
static void ElementDestroy(void* _element);
static int CloseAllUserGroupsChating(InternalClientApp* _clientAppObj);
/* Chat launcher: */
static void ChatLauncher(char* _ip, int _port, char* _nameOfUser, char* _nameOfChatGroup);


/* API */
void RunClientApp(char* _serverIP, int _serverPort)
{
    int resultMenu = SUCCESS;
    InternalClientApp clientAppObj;
    Client* client = ClientCreate(_serverPort, _serverIP);
    if(client == NULL)
    {
        UIPrintMessage("Unable to connect right now. Please try again later\n");
        return;
    }
    clientAppObj.m_client = client;
    clientAppObj.m_pidsList = LinkedListCreate();
    if(clientAppObj.m_pidsList == NULL)
    {
        ClientDestroy(&client);
        return;
    }
    while(resultMenu != ERROR)
    {
        resultMenu = StartMenu(&clientAppObj);
        if(resultMenu == ERROR)
        {
            LinkedListDestroy(&(clientAppObj.m_pidsList), NULL); /*no element here*/
            ClientDestroy(&client);
            return;
        }

        resultMenu = SecondaryMenu(&clientAppObj);
    }
    LinkedListDestroy(&(clientAppObj.m_pidsList), &ElementDestroy);
    ClientDestroy(&client);
}



/* static function */
static int StartMenu(InternalClientApp* _clientAppObj)
{
    int result;
    int choice;
    while(TRUE)
    {
        UIShowMenu("Hello, Chat Login or Register\n\n1. Register\n2. Login\n0. Exit\n\n\nEnter your choice:  ");
        UIGetChoice(&choice);

        switch(choice)
        {
        case REGISTER:
            if( (result = Register(_clientAppObj)) == FAILED)
            {
                UIPrintMessage("Registration failed\n\n");
            }
            else if(result == ERROR)
            {
                return ERROR;
            }
            break;

        case LOGIN:
            if(Login(_clientAppObj) == SUCCESS)
            {
                return SUCCESS;
            }
            UIPrintMessage("Login failed\n\n");
            break;
        case EXIT:
            UIPrintMessage("Goodbye!!!\n");
            return ERROR; /*Get out of the whole system*/

        default:
            UIPrintMessage("\nYou have selected an invalid value, Please select again:\t");
            break;
        }
    }
    return ERROR;
}


static int Register(InternalClientApp* _clientAppObj)
{
    int packingSize;
    char buffer[BUFFER_SIZE];
    SignUpRequest signUpRequestObj;
    SignUpResponse signUpResponseObj;

    EnterUsernameAndPassword(&signUpRequestObj, SIGN_UP_REQUEST);
    strcpy(_clientAppObj->m_clientName, signUpRequestObj.m_usernameToSignUp);

    packingSize = PackMessageByTag(&signUpRequestObj, (Byte*)buffer, SIGN_UP_REQUEST);
    if(packingSize == 0)
    {
        return FAILED;
    }

    if( SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, packingSize) == FAILED)
    {
        return FAILED;
    }

    if( UnPackMessageByTag(&signUpResponseObj, (Byte*)buffer, SIGN_UP_RESPONSE) == 0)
    {
        LogoutFromUsername(_clientAppObj);
        return FAILED;
    }

    if(signUpResponseObj.m_responseResult == RESPONSE_OK)
    {
        UIPrintMessage("\nSuccessfully registered!\nPlease Login\n");
        return SUCCESS;
    }
    else if(signUpResponseObj.m_responseResult == RESPONSE_MEMORY_ALLOCATION_FAILED)
    {
        UIPrintMessage("\nConnection to system failed\n");
        return ERROR;
    }
    else if(signUpResponseObj.m_responseResult == RESPONSE_USERNAME_ALREADY_EXISTS)
    {
        UIPrintMessage("\nThis client already exists in the system. Make a login\n");
        return FAILED;
    }
    else
    {
        UIPrintMessage("Failed to connect to the system...\n"); /*The message was not fully received on the server*/
        return ERROR;
    }
}


static int Login(InternalClientApp* _clientAppObj)
{
    int sizePacking;
    LoginRequest LoginRequestObj;
    LoginResponse loginResponseObj;
    char buffer[BUFFER_SIZE];

    EnterUsernameAndPassword(&LoginRequestObj, LOGIN_REQUEST);
    strcpy(_clientAppObj->m_clientName, LoginRequestObj.m_usernameToLogin);

    sizePacking = PackMessageByTag(&LoginRequestObj, (Byte*)buffer, LOGIN_REQUEST);
    if(sizePacking == 0)
    {
        return FAILED;
    }

    if( SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, sizePacking) == FAILED)
    {
        return FAILED;
    }

    if( UnPackMessageByTag(&loginResponseObj, (Byte*)buffer, LOGIN_RESPONSE) == 0)
    {
        LogoutFromUsername(_clientAppObj);
        return FAILED;
    }


    if(loginResponseObj.m_responseResult == RESPONSE_OK)
    {
        UIPrintMessage("Logged in successfully\n");
        return SUCCESS;
    }
    else if(loginResponseObj.m_responseResult == RESPONSE_MEMORY_ALLOCATION_FAILED)
    {
        UIPrintMessage("Failed to connect to the system...\n");
        return ERROR;
    }
    else if(loginResponseObj.m_responseResult == RESPONSE_USER_ALREADY_LOGGED_IN)
    {
        UIPrintMessage("User already logged in...\n");
        return FAILED;
    }
    else if(loginResponseObj.m_responseResult == RESPONSE_USERNAME_NOT_EXISTS)
    {
        UIPrintMessage("Unrecognized username - please check your spelling or sign up... \n");
        return FAILED;
    }
    else if(loginResponseObj.m_responseResult == RESPONSE_WRONG_PASSWORD)
    {
        UIPrintMessage("Wrong password\n");
        return FAILED;
    }
    else
    {
        UIPrintMessage("Failed to connect to the system...\n"); /*The message was not fully received on the server*/
        return ERROR;
    }
}


static void EnterUsernameAndPassword(void* _obj, ProtocolParsingTag _tag)
{
    char clientName[MAX_VALID_LEN];
    char clientPassword[MAX_VALID_LEN];

    UIPrintMessage("Please enter a username without spaces, up to 100 characters\n");

    while(UIGetDetail(clientName, MAX_VALID_LEN) != 1)
    {
        UIPrintMessage("Incorrect username\nPlease enter a username without spaces, up to 100 characters\n");
    }

    UIPrintMessage("Please enter a password without spaces, up to 100 characters\n");

    while(UIGetDetail(clientPassword, MAX_VALID_LEN) != 1)
    {
        UIPrintMessage("Incorrect password\nPlease enter a password without spaces, up to 100 characters\n");

    }
    switch (_tag)
    {
    case SIGN_UP_REQUEST:
        strcpy(((SignUpRequest*)_obj)->m_usernameToSignUp, clientName);
        strcpy(((SignUpRequest*)_obj)->m_password, clientPassword);
        break;
    case LOGIN_REQUEST:
        strcpy(((LoginRequest*)_obj)->m_usernameToLogin, clientName);
        strcpy(((LoginRequest*)_obj)->m_password, clientPassword);
        break;

    default:
        break;
    }

}


static void EnterGroupName(void* _obj, ProtocolParsingTag _tag)
{
    char groupName[MAX_VALID_LEN];

    UIPrintMessage("Please enter a group name without spaces, up to 100 characters\n");

    while(UIGetDetail(groupName, MAX_VALID_LEN) != 1)
    {
        UIPrintMessage("Incorrect group name\nPlease enter again a group name without spaces, up to 100 characters\n");
    }

    switch (_tag)
    {
    case JOIN_GROUP_REQUEST:
        strcpy(((JoinGroupRequest*)_obj)->m_groupNameToJoin, groupName);
        break;

    case LEAVE_GROUP_REQUEST:
        strcpy(((LeaveGroupRequest*)_obj)->m_groupNameToLeave, groupName);
        break;

    case CREATE_GROUP_REQUEST:
        strcpy(((CreateGroupRequest*)_obj)->m_groupNameToCreate, groupName);
        break;

    default:
        break;
    }
}


static int SendAndReceiveMessage(InternalClientApp* _clientAppObj, Byte* _buffer, int _sizePacking)
{
    int receiveMessageSize, isCorrectSize, currentMessageSize, currentCopyBytes = 0;
    char tempBuffer[BUFFER_SIZE];

    if(ClientSendMessage(_clientAppObj->m_client, _buffer, _sizePacking) != CLIENT_SUCCESS)
    {
        return FAILED;
    }

    receiveMessageSize = ClientReceiveMessage(_clientAppObj->m_client, _buffer, BUFFER_SIZE);
    if(receiveMessageSize == 0) /*server close connection*/
    {
        return ERROR;
    }
    else if(receiveMessageSize < 0)
    {
        return FAILED;
    }
    isCorrectSize = IsCorrectMessageSize((Byte*)_buffer, receiveMessageSize);
    if(0 == isCorrectSize)
    {
        memcpy(tempBuffer + currentCopyBytes, _buffer, receiveMessageSize);
        currentCopyBytes += receiveMessageSize;
        while(0 == isCorrectSize)
        {
            receiveMessageSize = ClientReceiveMessage(_clientAppObj->m_client, _buffer, BUFFER_SIZE);
            if(receiveMessageSize == 0) /*server close connection*/
            {
                return ERROR;
            }
            else if(receiveMessageSize < 0)
            {
                return FAILED;
            }
            memcpy(tempBuffer + currentCopyBytes, _buffer, receiveMessageSize);
            currentCopyBytes += receiveMessageSize;
            isCorrectSize = IsCorrectMessageSize((Byte*)tempBuffer, currentCopyBytes);
        }
        memcpy(_buffer, tempBuffer, currentCopyBytes);
    }
    return SUCCESS;
}


static int SecondaryMenu(InternalClientApp* _clientAppObj)
{
    int result;
    int choice;
    while(TRUE)
    {
        UIShowMenu("\n1. Create a group\n2. Sign up for a group\n3. Disconnect from group\n0. Log out\n\n\nEnter your choice:  ");
        UIGetChoice(&choice);

        switch(choice)
        {

            case CREATE_GROUP:
                if( (result = CreateGroup(_clientAppObj)) == FAILED)
                {
                    UIPrintMessage("The creation of the new group failed\n");
                }
                else if(result == ERROR)
                {
                    return ERROR;
                }
                break;

            case SIGN_UP_FOR_GROUP:
                if( (result = JoinForGroup(_clientAppObj)) == FAILED)
                {
                    UIPrintMessage("Joining the group failed\n");
                }
                else if(result == ERROR)
                {
                    UIPrintMessage("\nConnection to system failed\n");
                    return ERROR;
                }
                break;

            case DISCONNECT_FROM_GROUP:
                if((result = DisconnectFromGroup(_clientAppObj)) == FAILED)
                {
                    UIPrintMessage("The group leaving is failed\n");
                }
                else if(result == ERROR)
                {
                    UIPrintMessage("\nConnection to system failed\n");
                    return ERROR;
                }
                break;

            case LOG_OUT:
                if(( result = LogoutFromUsername(_clientAppObj)) == FAILED)
                {
                    UIPrintMessage("Failed to logout...");
                }
                else
                {
                    return result; 
                }
                break;

            default:
                UIPrintMessage("\nYou have selected an invalid value, Please select again:\t");
                break;
        }
    }
    return ERROR;
}


static int CreateGroup(InternalClientApp* _clientAppObj)
{
    int packingSize;
    char buffer[BUFFER_SIZE];
    CreateGroupRequest createGroupRequestObj;
    CreateGroupResponse createGroupResponseObj;
    FILE* PidFile;
    PIDsGroup* objPIDsGroup;

    objPIDsGroup = (PIDsGroup*)malloc(sizeof(PIDsGroup));
    if(objPIDsGroup == NULL)
    {
        return ERROR;
    }

    EnterGroupName(&createGroupRequestObj, CREATE_GROUP_REQUEST);
    strcpy(createGroupRequestObj.m_usernameOfGroupCreator, _clientAppObj->m_clientName);

    packingSize = PackMessageByTag(&createGroupRequestObj, (Byte*)buffer, CREATE_GROUP_REQUEST);
    if(packingSize == 0)
    {
        return FAILED;
    }

    if( SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, packingSize) == FAILED)
    {
        return FAILED;
    }

    UnPackMessageByTag(&createGroupResponseObj, (Byte*)buffer, CREATE_GROUP_RESPONSE);

    if(createGroupResponseObj.m_responseResult == RESPONSE_OK) /* contains multicast IP*/
    {
        UIPrintMessage("\nYou have successfully created the group\n");

        /*------ Chatting: -----*/
        ChatLauncher(createGroupResponseObj.m_multicastIPAddress, createGroupResponseObj.m_port, _clientAppObj->m_clientName, createGroupRequestObj.m_groupNameToCreate);
        PidFile = fopen("pidSender","r");
        if (PidFile == NULL) /*File not opened*/
        {
            printf("failed open file: pidSender");
            free(objPIDsGroup);
            return ERROR;
        }
        fscanf(PidFile, "%d", &(objPIDsGroup->m_pidChatSend));
        close(PidFile);

        PidFile = fopen("pidReceiver","r");
        if (PidFile == NULL) /*File not opened*/
        {
            printf("failed open file: pidReceiver");
            free(objPIDsGroup);
            return ERROR;
        }
        fscanf(PidFile, "%d", &(objPIDsGroup->m_pidChatReceive));
        close(PidFile);
        strcpy(objPIDsGroup->m_groupName, createGroupRequestObj.m_groupNameToCreate);
        if(LinkedListInsertTail(_clientAppObj->m_pidsList, objPIDsGroup) != LINKEDLIST_SUCCESS)
        {
            free(objPIDsGroup); /*When:  LINKEDLIST_ALLOCATION_ERROR*/
            return ERROR;
        }

        return SUCCESS;
    }
    else if(createGroupResponseObj.m_responseResult == RESPONSE_GROUP_NAME_ALREADY_EXISTS)
    {
        UIPrintMessage("\nA group with the same name already exists\n");
        return FAILED;
    }
    else if(createGroupResponseObj.m_responseResult == RESPONSE_MEMORY_ALLOCATION_FAILED)
    {
        return ERROR;
    }
    else if(createGroupResponseObj.m_responseResult == RESPONSE_ALL_AVAILABLE_GROUPS_ARE_IN_USE)
    {
        UIPrintMessage("\nAll available groups are already in use - try to create a new group later...\n");
        return FAILED;
    }
    else
    {
        /*The message was not fully received on the server*/
        return ERROR;
    }
}




static int JoinForGroup(InternalClientApp* _clientAppObj)
{
    int packingSize;
    char buffer[BUFFER_SIZE];
    JoinGroupRequest joinGroupRequestObj;
    JoinGroupResponse joinGroupResponseObj;
    FILE* PidFile;
    PIDsGroup* objPIDsGroup;

    objPIDsGroup = (PIDsGroup*)malloc(sizeof(PIDsGroup));
    if(objPIDsGroup == NULL)
    {
        return ERROR;
    }

    EnterGroupName(&joinGroupRequestObj, JOIN_GROUP_REQUEST);
    strcpy(joinGroupRequestObj.m_usernameToJoin, _clientAppObj->m_clientName);

    packingSize = PackMessageByTag(&joinGroupRequestObj, (Byte*)buffer, JOIN_GROUP_REQUEST);
    if(packingSize == 0)
    {
        return FAILED;
    }

    if(SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, packingSize) == FAILED)
    {
        return FAILED;
    }

    UnPackMessageByTag(&joinGroupResponseObj, (Byte*)buffer, JOIN_GROUP_RESPONSE);

    if(joinGroupResponseObj.m_responseResult == RESPONSE_OK)
    {
        UIPrintMessage("\nYou have successfully joined the group\n");

        /*------ Chatting: -----*/
        ChatLauncher(joinGroupResponseObj.m_multicastIPAddress, joinGroupResponseObj.m_port, _clientAppObj->m_clientName, joinGroupRequestObj.m_groupNameToJoin);

        PidFile = fopen("pidSender","r");
        if (PidFile == NULL) /*File not opened*/
        {
            printf("failed open file: pidSender");
            free(objPIDsGroup);
            return ERROR;
        }
        fscanf(PidFile, "%d", &(objPIDsGroup->m_pidChatSend));
        close(PidFile);

        PidFile = fopen("pidReceiver","r");
        if (PidFile == NULL) /*File not opened*/
        {
            printf("failed open file: pidReceiver");
            free(objPIDsGroup);
            return ERROR;
        }
        fscanf(PidFile, "%d", &(objPIDsGroup->m_pidChatReceive));
        close(PidFile);
        strcpy(objPIDsGroup->m_groupName, joinGroupRequestObj.m_groupNameToJoin);
        if(LinkedListInsertTail(_clientAppObj->m_pidsList, objPIDsGroup) != LINKEDLIST_SUCCESS)
        {
            free(objPIDsGroup); /*When:  LINKEDLIST_ALLOCATION_ERROR*/
            return ERROR;
        }


        return SUCCESS;
    }
    else if(joinGroupResponseObj.m_responseResult == RESPONSE_GROUP_NAME_NOT_EXISTS)
    {
        UIPrintMessage("\nJoining has not been performed\nThere is no group with the same name as the one received\n");
        return FAILED;
    }
    else if(joinGroupResponseObj.m_responseResult == RESPONSE_USER_ALREADY_JOINED_GROUP) /* In the group */
    {
        UIPrintMessage("\nYou are already connected to the same group\n");
        return FAILED;
    }
    else if(joinGroupResponseObj.m_responseResult == RESPONSE_MEMORY_ALLOCATION_FAILED)
    {
        UIPrintMessage("\nConnection to system failed\n");
        return ERROR;
    }
    else
    {
        UIPrintMessage("Failed to connect to the system...\n"); /*The message was not fully received on the server*/
        return ERROR;
    }
}

static int DisconnectFromGroup(InternalClientApp* _clientAppObj)
{
    int packingSize;
    char buffer[BUFFER_SIZE];
    LeaveGroupRequest leaveGroupRequestObj;
    LeaveGroupResponse leaveGroupResponseObj;
    LinkedListIterator result;
    void* removeData;
    LinkedListIterator listEnd = LinkedListIteratorEnd(_clientAppObj->m_pidsList);

    EnterGroupName(&leaveGroupRequestObj, LEAVE_GROUP_REQUEST);
    strcpy(leaveGroupRequestObj.m_usernameToLeave, _clientAppObj->m_clientName);

    packingSize = PackMessageByTag(&leaveGroupRequestObj, (Byte*)buffer, LEAVE_GROUP_REQUEST);
    if(packingSize == 0)
    {
        return FAILED;
    }

    if( SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, packingSize) == FAILED)
    {
        return FAILED;
    }

    UnPackMessageByTag(&leaveGroupResponseObj, (Byte*)buffer, LEAVE_GROUP_RESPONSE);

    if(leaveGroupResponseObj.m_responseResult == RESPONSE_OK)
    {
        UIPrintMessage("\nYou have successfully left the group\n");
        /*close chating*/
        if( ( result = LinkedListIteratorFindFirst(LinkedListIteratorBegin(_clientAppObj->m_pidsList), listEnd, &CompGroupName, leaveGroupRequestObj.m_groupNameToLeave)) == listEnd)
        {
            return ERROR;
        }
        removeData = LinkedListIteratorRemove(result);
        ElementDestroy(removeData);
        
        return SUCCESS;
    }
    else if(leaveGroupResponseObj.m_responseResult == RESPONSE_GROUP_NAME_NOT_EXISTS)
    {
        UIPrintMessage("\nYou do not exist in this group\n");
        return FAILED;
    }
    else
    {
        UIPrintMessage("Failed to connect to the system...\n"); /*The message was not fully received on the server*/
        return ERROR;
    }

}


static int LogoutFromUsername(InternalClientApp* _clientAppObj)
{
    int packingSize;
    char buffer[BUFFER_SIZE];
    LogoutRequest logoutRequestObj;
    LogoutResponse logoutResponseObj;
    LinkedListIterator listEnd = LinkedListIteratorEnd(_clientAppObj->m_pidsList);

    strcpy(logoutRequestObj.m_usernameToLogout, _clientAppObj->m_clientName);

    packingSize = PackMessageByTag(&logoutRequestObj, (Byte*)buffer, LOGOUT_REQUEST);
    if(packingSize == 0)
    {
        return FAILED;
    }

    if( SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, packingSize) == FAILED)
    {
        return FAILED;
    }

    UnPackMessageByTag(&logoutResponseObj, (Byte*)buffer, LOGOUT_RESPONSE);

    if(logoutResponseObj.m_responseResult == RESPONSE_OK)
    {
        UIPrintMessage("\nYou have successfully logged out\n");
        /*close All a chat*/

        if(CloseAllUserGroupsChating(_clientAppObj) != SUCCESS)
        {
            return ERROR;
        }

        return SUCCESS;
    }
    else if(logoutResponseObj.m_responseResult == RESPONSE_MEMORY_ALLOCATION_FAILED)
    {
        UIPrintMessage("\nConnection to system failed\n");
        return ERROR;

    }
    else
    {
        UIPrintMessage("Failed to connect to the system...\n"); /*The message was not fully received on the server*/
        return ERROR;
    }
}

static int CompGroupName(void* _element, void* _context)
{
    if(strcmp(((PIDsGroup*)_element)->m_groupName, (char*)_context) == 0) /*equal*/
    {
        return 1;
    }
    return 0;
}


static int CloseAllUserGroupsChating(InternalClientApp* _clientAppObj)
{
    int packingSize;
    char buffer[BUFFER_SIZE];
    LinkedListIterator begin = LinkedListIteratorBegin(_clientAppObj->m_pidsList);
    LinkedListIterator end = LinkedListIteratorEnd(_clientAppObj->m_pidsList);
    LinkedListIterator temp;
    LeaveGroupRequest leaveGroupRequestObj;
    LeaveGroupResponse leaveGroupResponseObj;
    void* groupRemove;
    while (begin != end)
    {
        temp = LinkedListIteratorNext(begin);
        groupRemove = LinkedListIteratorRemove(begin);
        strcpy(leaveGroupRequestObj.m_groupNameToLeave, ((PIDsGroup*)groupRemove)->m_groupName);
        strcpy(leaveGroupRequestObj.m_usernameToLeave, _clientAppObj->m_clientName);

        /* leave group*/
        packingSize = PackMessageByTag(&leaveGroupRequestObj, (Byte*)buffer, LEAVE_GROUP_REQUEST);
        if(packingSize == 0)
        {
            return ERROR;
        }

        if( SendAndReceiveMessage(_clientAppObj, (Byte*)buffer, packingSize) == FAILED)
        {
            return ERROR;
        }

        UnPackMessageByTag(&leaveGroupResponseObj, (Byte*)buffer, LEAVE_GROUP_RESPONSE);

        if(leaveGroupResponseObj.m_responseResult != RESPONSE_OK)
        {
            UIPrintMessage("Failed to connect to the system...\n"); /*The message was not fully received on the server*/
            return ERROR;
        }


        ElementDestroy(groupRemove);
        begin = temp;
    }
    return SUCCESS;
}


static void ElementDestroy(void* _element)
{
    kill(((PIDsGroup*)_element)->m_pidChatReceive, 9);
    kill(((PIDsGroup*)_element)->m_pidChatSend, 9);
    free(_element);
}

/* Chat launcher: */

static void ChatLauncher(char* _ip, int _port, char* _nameOfUser, char* _nameOfChatGroup)
{
    char commandLaunchChatSender[1024];
    char commandLaunchChatReceiver[1024];

    sprintf(commandLaunchChatSender, "%s %d %s %s %s", "gnome-terminal --geometry 30x20+400+300 -- ./ChatSender.out", _port, _ip, _nameOfUser, _nameOfChatGroup);
    sprintf(commandLaunchChatReceiver, "%s %d %s %s", "gnome-terminal --geometry 30x20+900+300 -- ./ChatReceiver.out",_port , _ip, _nameOfChatGroup);

    system(commandLaunchChatSender);
    system(commandLaunchChatReceiver);
}