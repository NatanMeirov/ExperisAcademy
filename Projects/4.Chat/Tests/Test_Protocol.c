/**
 * @file Test_Protocol.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Protocol Test's
 * @version 1.0
 * @date 2021-04-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <string.h> /* strcpy */
#include "../Include/MessagesProtocolParser.h"


/* Defines: */

#define LENGTH 100
#define BUFFER_LENGTH 4096
#define BYTES_TO_PRINT 25


/* Static Functions Declarations: */

static void PrintBuffer(Byte* _buffer, size_t _bytesToPrint);

/* Tests: */
static void Test_SignUpAndLoginAndAllGroupRequests_PackandUnPack(void);
static void Test_Response_PackAndUnPack(void);
static void Test_GroupResponseWithIP_PackAndUnPack(void);



/* Main: */
int main(void)
{
    Test_SignUpAndLoginAndAllGroupRequests_PackandUnPack();
    Test_Response_PackAndUnPack();
    Test_GroupResponseWithIP_PackAndUnPack();

    return 0;
}


/* Static Functions Implementations: */

static void PrintBuffer(Byte* _buffer, size_t _bytesToPrint)
{
    size_t i;

    for(i = 0; i < _bytesToPrint; i++)
    {
        putchar(_buffer[i]);
    }

    putchar('\n');
}


/* Tests: */

static void Test_SignUpAndLoginAndAllGroupRequests_PackandUnPack(void)
{
    /* Note: the login, sign up and all group requests are handled in the same way in the protocol's code parsing method */
    SignUpRequest packRequestObj;
    SignUpRequest unpackRequestObj;
    Byte buffer[BUFFER_LENGTH];
    size_t sizeOfPackage;

    strcpy(packRequestObj.m_usernameToSignUp, "Natan");
    strcpy(packRequestObj.m_password, "123456");


    /* Packing */
    printf("\nPacking:\n");

    sizeOfPackage = PackMessageByTag((void*)&packRequestObj, buffer, SIGN_UP_REQUEST);
    printf("[LENGTH:] Check if: 17 == %ld\n", sizeOfPackage);
    printf("[TAG:] Check if: 1 == %d\n", GetMessageTag(buffer));
    printf("Check if: %ld == 15\n", (size_t)buffer[1]); /* check LENGTH header (!= total size == LENGTH + 2)*/
    printf("Check if: 1 == %d\n", IsCorrectMessageSize(buffer, 17));

    printf("In the buffer:\t");
    PrintBuffer(buffer, sizeOfPackage);

    /* UnPacking: */
    printf("\nUnPacking:\n");

    printf("Check if: 1 == %d\n", GetMessageTag(buffer));
    printf("Check if: 1 == %d\n", IsCorrectMessageSize(buffer, 17));
    UnPackMessageByTag(&unpackRequestObj, buffer, GetMessageTag(buffer));
    printf("Check if: Natan == %s\n", unpackRequestObj.m_usernameToSignUp);
    printf("Check if: 123456 == %s\n", unpackRequestObj.m_password);
}


static void Test_Response_PackAndUnPack(void)
{
    SignUpResponse packResponseObj, unpackResponseObj;
    size_t sizeOfPackage;
    Byte buffer[BUFFER_LENGTH];

    packResponseObj.m_responseResult = RESPONSE_OK;
    strcpy(packResponseObj.m_responseMessage, "200 OK");

    /* Packing: */
    printf("\nPacking:\n");

    sizeOfPackage = PackMessageByTag((void*)&packResponseObj, buffer, SIGN_UP_RESPONSE);
    printf("[PACKAGE SIZE:] Check if: 12 == %ld\n", sizeOfPackage);
    printf("[TAG:] Check if: 2 == %d\n", GetMessageTag(buffer));
    printf("[HEADER LENGTH:]Check if: %ld == 10\n", (size_t)buffer[1]); /* check LENGTH header (!= total size == LENGTH + 2)*/
    printf("Check if: 1 == %d\n", IsCorrectMessageSize(buffer, 12));

    printf("In the buffer: ");
    PrintBuffer(buffer, sizeOfPackage);

    /* UnPacking: */
    printf("\nUnPacking:\n");

    printf("Check if: 2 == %d\n", GetMessageTag(buffer));
    printf("Check if: 1 == %d\n", IsCorrectMessageSize(buffer, 12));
    UnPackMessageByTag(&unpackResponseObj, buffer, GetMessageTag(buffer));
    printf("[RESPONSE RESULT STATUS] Check if: 0 == %d\n", unpackResponseObj.m_responseResult);
    printf("Check if: 200 OK == %s\n", unpackResponseObj.m_responseMessage);
}


static void Test_GroupResponseWithIP_PackAndUnPack(void)
{
    CreateGroupResponse packGroupResponseObj, unpackGroupResponseObj;
    size_t sizeOfPackage;
    Byte buffer[BUFFER_LENGTH];

    packGroupResponseObj.m_responseResult = RESPONSE_OK;
    strcpy(packGroupResponseObj.m_responseMessage, "200:OK");
    strcpy(packGroupResponseObj.m_multicastIPAddress, "200.11.0.200");

    /* Packing: */
    printf("\nPacking:\n");

    sizeOfPackage = PackMessageByTag((void*)&packGroupResponseObj, buffer, CREATE_GROUP_RESPONSE);
    printf("[PACKAGE SIZE:] Check if: 26 == %ld\n", sizeOfPackage);
    printf("[TAG:] Check if: 8 == %d\n", GetMessageTag(buffer));
    printf("[HEADER LENGTH:]Check if: %ld == 24\n", (size_t)buffer[1]); /* check LENGTH header (!= total size == LENGTH + 2)*/
    printf("Check if: 1 == %d\n", IsCorrectMessageSize(buffer, 26));

    printf("In the buffer: ");
    PrintBuffer(buffer, sizeOfPackage);

    /* UnPacking: */
    printf("\nUnPacking:\n");

    printf("Check if: 8 == %d\n", GetMessageTag(buffer));
    printf("Check if: 1 == %d\n", IsCorrectMessageSize(buffer, 26));
    UnPackMessageByTag(&unpackGroupResponseObj, buffer, GetMessageTag(buffer));
    printf("[RESPONSE RESULT STATUS] Check if: 0 == %d\n", unpackGroupResponseObj.m_responseResult);
    printf("Check if: 200:OK == %s\n", unpackGroupResponseObj.m_responseMessage);
    printf("Check if: 200.11.0.200 == %s\n", unpackGroupResponseObj.m_multicastIPAddress);
}