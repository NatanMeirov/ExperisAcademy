#include <string.h> /*strchr, strcpy*/
#include "../Include/clientApp.h"
#include "../Include/UI.h"
#include "../Include/clientNet.h"
#include "../Include/MessagesProtocolParser.h"

#define SUCCESS 1
#define FAILED 0
#define VALID 1
#define NOT_VALID 0
#define BUFFER_SIZE 4096

typedef enum StartMenuOption
{
    EXIT = 0,
    REGISTER,
    LOGIN
}StartMenuOption;

/* static function */
static int StartMenu();
static int Register();
static int IsValid(char* _clientdetail);
static int Login(Client* _client);
static void LogOut();



void RunClientApp(char* _serverIP, int _serverPort)
{
    int choice;
    Client* client = ClientCreate(_serverPort, _serverIP);
    if(client == NULL)
    {
        PrintMessage("Unable to connect right now. Please try again later\n");
        return;
    }
    ShowMenu("Hello, Chat Login or Register\n\n1. Register\n2. Login\n0. Exit\n\n\nEnter your choice:  ");
    choice = StartMenu(client);
    if(choice == 0)
    {
        return;
    }
    ShowMenu("\n1. Sign up for a group\n2. Disconnect from group\n3. Create a group\n4. For chatting\n5. Create a group\n0. Log out\n\n\nEnter your choice:  ");
}







/* help function */
static int StartMenu(Client* _client)
{
    int choice = REGISTER;
    while(choice != 0)
    {
        GetChoice(&choice);
        switch(choice)
        {
        case REGISTER:
            if( Register(_client) == SUCCESS)
            {
                return 1;
            }
            break;
        case LOGIN:
            if(Login(_client) == SUCCESS)
            {
                return 1;
            }
            break;
        case EXIT:
            PrintMessage("Goodbye\n");
            return 0;
        default:
            PrintMessage("\nYou have selected an invalid value, Please select again:\t");
            break;
        }
    }
    return 0;
}


static int Register(Client* _client)
{
    int sizePacking, receiveMessageSize, messageSize, unPackResult, currentMessageSize;
    char clientName[100];
    char clientPassword[100];
    char buffer[BUFFER_SIZE];
    char tempBuffer[BUFFER_SIZE];
    SignUpRequest signUpDetails;
    SignUpResponse response;

    /*----   MOVE TO FUNCTION ------*/
    PrintMessage("Please enter a username without spaces, up to 100 characters\n");
    GetDetail(clientName);

    while(IsValid(clientName) != VALID)
    {
        PrintMessage("Incorrect username\n");
        PrintMessage("Please enter a username without spaces, up to 100 characters\n");
        GetDetail(clientName);
    }
    strcpy(signUpDetails.m_usernameToSignUp, clientName);

    PrintMessage("Enter password:\n");

    GetDetail(clientPassword);
    while(IsValid(clientPassword) != VALID)
    {
        PrintMessage("Incorrect password\nPlease enter a password without spaces, up to 100 characters\n");
        GetDetail(clientPassword);
    }
    strcpy(signUpDetails.m_password, clientPassword);

    /*----   ------------    ------*/

    sizePacking = PackMessageByTag(&signUpDetails, (Byte*)buffer, SIGN_UP_REQUEST);
    if(sizePacking == 0)
    {
        PrintMessage("Registration failed\n"); /***/
        return FAILED;
    }
    if(ClientSendMessage(_client, buffer, sizePacking) != CLIENT_SUCCESS)
    {
        PrintMessage("Registration failed\n"); /***/
        return FAILED;
    }
    receiveMessageSize = ClientReceiveMessage(_client, buffer, BUFFER_SIZE);
    if(receiveMessageSize == 0) /*server close connection*/
    {
        ClientDestroy(&_client);
        return FAILED;
    }
    else if(receiveMessageSize < 0)
    {
        return FAILED;
    }
    messageSize = IsCorrectMessageSize((Byte*)buffer, receiveMessageSize);
    while(0 == messageSize)
    {
        memcpy(tempBuffer, buffer, receiveMessageSize);
        currentMessageSize = receiveMessageSize + 1;
        receiveMessageSize = ClientReceiveMessage(_client, buffer, BUFFER_SIZE);
        if(receiveMessageSize == 0) /*server close connection*/
        {
            ClientDestroy(&_client);
            return FAILED;
        }
        else if(receiveMessageSize < 0)
        {
            return FAILED;
        }
        /*FIXME:*/
        currentMessageSize = receiveMessageSize + 1;
        memcpy(tempBuffer, buffer, receiveMessageSize);
        messageSize = IsCorrectMessageSize((Byte*)tempBuffer, receiveMessageSize);

    }
    unPackResult = UnPackMessageByTag(&response, (Byte*)buffer, SIGN_UP_RESPONSE);
    if(unPackResult == 0)
    {
        /*FIXME:*/
        LogOut();
        return FAILED;
    }

    if(response.m_responseResult == RESPONSE_OK)
    {
        PrintMessage("Registration succeeded");
        return SUCCESS;
    }
    else
    {
        PrintMessage("This client already exists in the system. Make a login");
        return FAILED;
    }
}


/*TODO: change get scanf to sgetf and the function isValid*/
static int IsValid(char* _clientdetail)
{
    if(_clientdetail[0] == '\n' || _clientdetail[0] == '\t' || _clientdetail[0] == '\0')
    {
        return NOT_VALID;
    }
    if(strchr(_clientdetail, ' ') != NULL)
    {
        return NOT_VALID;
    }
    return VALID;
}


/*TODO:  function*/
static int Login(Client* _client)
{
    return SUCCESS;
}

static void LogOut()
{

}