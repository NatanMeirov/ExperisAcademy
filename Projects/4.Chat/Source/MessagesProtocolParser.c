/**
 * @file MessagesProtocolParser.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A messages protocol parser module that uses for parsing a given object, to stream of bytes (message)
 * Every message will be parsed using an agreed (known) protocol, and will be unique by certain tag
 * @version 1.0
 * @date 2021-04-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint16_t */
#include <string.h> /* memcpy, strlen */
#include "../Include/MessagesProtocolParser.h"


/* Defines: */

/* An enum to mark who is the cursor (who to change its location) */
typedef enum CursorIndicator { DESTINATION, SOURCE } CursorIndicator;

/* An abstract Request class to be used as a base for most request classes */
typedef struct TwoStringsRequest
{
    char m_firstString[CHARACTERS_LIMIT];
    char m_secondString[CHARACTERS_LIMIT];
} TwoStringsRequest;

/* An abstract Response class to be used as a base for all the response classes */
typedef struct Response
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
} Response;

/* An abstract Response class to be used as a base join and create group response classes */
typedef struct CreateAndJoinGroupResponse
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
    char m_multicastIPAddress[IP_LENGTH];
    int m_port;
} CreateAndJoinGroupResponse;

/* Static Functions Declarations: */

/* *** Every parsing function will return the current location of the cursor in the buffer (the NEXT EMPTY PLACE in the buffer) *** */

static Byte* CopyBytesAndSetCursorOnNextLocation(Byte* _destination, Byte* _source, size_t _bytesToCopy, CursorIndicator _cursorIndicator);

static Byte* PackTwoStringsRequestMessage(TwoStringsRequest* _twoStringsRequest, Byte* _buffer);
static Byte* PackLogoutRequestMessage(LogoutRequest* _logoutRequest, Byte* _buffer);
static Byte* PackResponseMessage(Response* _response, Byte* _buffer);
static Byte* PackIPAddress(char* _ipAddress, Byte* _buffer, size_t _bytesToCopy); /* refer to some Responses objects */

static Byte* UnPackTwoStringsRequestMessage(TwoStringsRequest* _twoStringsRequest, Byte* _buffer);
static Byte* UnPackLogoutRequestMessage(LogoutRequest* _logoutRequest, Byte* _buffer);
static Byte* UnPackResponseMessage(Response* _response, Byte* _buffer);
static Byte* UnPackIPAddress(char* _ipAddress, Byte* _buffer, size_t _bytesToCopy); /* refer to some Response objects */


/* ------------------------------------- Main API Functions -------------------------------------- */

size_t PackMessageByTag(void* _messageObject, Byte* _buffer, ProtocolParsingTag _tag)
{
    if(!_messageObject || !_buffer)
    {
        return 0;
    }

    /* Saves the tag as the first Header byte */
    *_buffer = (Byte)_tag;

    switch(_tag)
    {
    case SIGN_UP_REQUEST:
    case LOGIN_REQUEST:
    case CREATE_GROUP_REQUEST:
    case JOIN_GROUP_REQUEST:
    case LEAVE_GROUP_REQUEST:
        PackTwoStringsRequestMessage((TwoStringsRequest*)_messageObject, _buffer);
        break;

    case LOGOUT_REQUEST:
        PackLogoutRequestMessage((LogoutRequest*)_messageObject, _buffer);
        break;

    case SIGN_UP_RESPONSE:
    case LOGIN_RESPONSE:
    case LOGOUT_RESPONSE:
    case CREATE_GROUP_RESPONSE:
    case JOIN_GROUP_RESPONSE:
    case LEAVE_GROUP_RESPONSE:
        PackResponseMessage((Response*)_messageObject, _buffer);
        break;

    default:
        return 0;
    }

    /* TODO: add a Cipher module and use an encrypt message function  */

    return (size_t)(*(_buffer + 1) + 2); /* The total message size */
}


int UnPackMessageByTag(void* _messageObject, Byte* _buffer, ProtocolParsingTag _tag)
{
    if(!_messageObject || !_buffer)
    {
        return 0;
    }

    /* TODO: add a Cipher module and use a decrypt message function  */

    switch(_tag)
    {
    case SIGN_UP_REQUEST:
    case LOGIN_REQUEST:
    case CREATE_GROUP_REQUEST:
    case JOIN_GROUP_REQUEST:
    case LEAVE_GROUP_REQUEST:
        UnPackTwoStringsRequestMessage((TwoStringsRequest*)_messageObject, _buffer);
        break;

    case LOGOUT_REQUEST:
        UnPackLogoutRequestMessage((LogoutRequest*)_messageObject, _buffer);
        break;

    case SIGN_UP_RESPONSE:
    case LOGIN_RESPONSE:
    case LOGOUT_RESPONSE:
    case CREATE_GROUP_RESPONSE:
    case JOIN_GROUP_RESPONSE:
    case LEAVE_GROUP_RESPONSE:
        UnPackResponseMessage((Response*)_messageObject, _buffer);
        break;

    default:
        return 0;
    }

    return 1;
}


ProtocolParsingTag GetMessageTag(Byte* _buffer)
{
    if(!_buffer)
    {
        return ERROR_TAG;
    }

    return (ProtocolParsingTag)*_buffer;
}


int IsCorrectMessageSize(Byte* _buffer, size_t _messageSizeInBytes)
{
    return (size_t)(*(_buffer + 1) + 2) == _messageSizeInBytes;
}

/* ---------------------------------- End of Main API Functions ----------------------------------- */


/* Static Functions Implementation: */

static Byte* CopyBytesAndSetCursorOnNextLocation(Byte* _destination, Byte* _source, size_t _bytesToCopy, CursorIndicator _cursorIndicator)
{
    memcpy(_destination, _source, _bytesToCopy);

    /* The indicator used to know who is the cursor to locate */
    if(_cursorIndicator == DESTINATION)
    {
        return _destination + _bytesToCopy;
    }

    /* Else: cursor is SOURCE */
    return _source + _bytesToCopy;
}


/* Packing Functions: */

static Byte* PackTwoStringsRequestMessage(TwoStringsRequest* _twoStringsRequest, Byte* _buffer)
{
    Byte* cursor = _buffer + 1;
    size_t firstStringLength = strlen(_twoStringsRequest->m_firstString) + 1, secondStringLength = strlen(_twoStringsRequest->m_secondString) + 1; /* +1 stands for adding '\0' char */

    /* Initialize the second header as the total Length of the message */
    *cursor = (Byte)(firstStringLength + 1 + secondStringLength + 1); /* +1 (X2) stands for an inner metadata about each string */
    cursor++;

    /* First string packing */
    *cursor = (Byte)(firstStringLength); /* metadata of first string length */
    cursor++;
    cursor = CopyBytesAndSetCursorOnNextLocation(cursor, (Byte*)_twoStringsRequest->m_firstString, firstStringLength, DESTINATION);

    /* Second string packing */
    *cursor = (Byte)(secondStringLength); /* metadata of second string length */
    cursor++;

    return CopyBytesAndSetCursorOnNextLocation(cursor, (Byte*)_twoStringsRequest->m_secondString, secondStringLength, DESTINATION);
}


static Byte* PackLogoutRequestMessage(LogoutRequest* _logoutRequest, Byte* _buffer)
{
    Byte* cursor = _buffer + 1;
    size_t sizeOfUsername = strlen(_logoutRequest->m_usernameToLogout) + 1; /* +1 for '\0' */

    /* Total LENGTH */
    *cursor = (Byte)(sizeOfUsername + 1); /* +1 for metadata */
    cursor++;

    /* Username packing */
    *cursor = (Byte)(sizeOfUsername);
    cursor++;

    return CopyBytesAndSetCursorOnNextLocation(cursor, (Byte*)_logoutRequest->m_usernameToLogout, sizeOfUsername, DESTINATION);
}


static Byte* PackResponseMessage(Response* _response, Byte* _buffer)
{
    Byte* cursor = _buffer + 1;
    size_t optionalIpAddressLength, responseResultLength = 1, responseMessageLength = strlen(_response->m_responseMessage) + 1; /* + '\0' */

    /* Total LENGTH: */
    *cursor = (Byte)(responseResultLength + 1 /* metadata */ + responseMessageLength + 2 /* metadata - 2 bytes for big length response message */);
    cursor++;

    /* Response code packing */
    *cursor = (Byte)responseResultLength;
    cursor++;
    *cursor = (Byte)_response->m_responseResult;
    cursor++;

    /* Response message packing */
    *(uint16_t*)cursor = (uint16_t)responseMessageLength;
    cursor += 2;
    cursor = CopyBytesAndSetCursorOnNextLocation(cursor, (Byte*)_response->m_responseMessage, responseMessageLength, DESTINATION);

    /* Check the tag to know if add the ip address length to the total LENGTH */
    if(*_buffer == CREATE_GROUP_RESPONSE || *_buffer == JOIN_GROUP_RESPONSE)
    {
        /* Adding the size of the IpAddress + 2 for: '\0' and for inner metadata header, and + 2 bytes for the port number - to the total LENGTH header */
        optionalIpAddressLength = strlen(((CreateAndJoinGroupResponse*)_response)->m_multicastIPAddress) + 1;
        *(_buffer + 1) += (Byte)(optionalIpAddressLength + 1 /* metadata */ + 2 /* port number bytes */);

        /* IP Address packing */
        /* Save the inner metadata about the IP Address: */
        *cursor = (Byte)(optionalIpAddressLength);
        cursor++;

        cursor = PackIPAddress(((CreateAndJoinGroupResponse*)_response)->m_multicastIPAddress, cursor, optionalIpAddressLength);

        /* Port packing */
        *(uint16_t*)cursor = ((CreateAndJoinGroupResponse*)_response)->m_port;

        return cursor + 2;
    }

    return cursor;
}


static Byte* PackIPAddress(char* _ipAddress, Byte* _buffer, size_t _bytesToCopy)
{
    return CopyBytesAndSetCursorOnNextLocation(_buffer, (Byte*)_ipAddress, _bytesToCopy, DESTINATION);
}


/* UnPacking Functions: */

static Byte* UnPackTwoStringsRequestMessage(TwoStringsRequest* _twoStringsRequest, Byte* _buffer)
{
    Byte* cursor = _buffer + 2;
    size_t sizeOfFirstString, sizeOfSecondString;

    /* First string unpacking */
    sizeOfFirstString = (size_t)(*cursor);
    cursor++;
    cursor = CopyBytesAndSetCursorOnNextLocation((Byte*)_twoStringsRequest->m_firstString, cursor, sizeOfFirstString, SOURCE);

    /* Second string unpacking */
    sizeOfSecondString = (size_t)*cursor;
    cursor++;

    return CopyBytesAndSetCursorOnNextLocation((Byte*)_twoStringsRequest->m_secondString, cursor, sizeOfSecondString, SOURCE);
}


static Byte* UnPackLogoutRequestMessage(LogoutRequest* _logoutRequest, Byte* _buffer)
{
    Byte* cursor = _buffer + 2;
    size_t sizeOfUserName;

    /* Username unpacking */
    sizeOfUserName = (size_t)(*cursor);
    cursor++;

    return CopyBytesAndSetCursorOnNextLocation((Byte*)_logoutRequest->m_usernameToLogout, cursor, sizeOfUserName, SOURCE);
}


static Byte* UnPackResponseMessage(Response* _response, Byte* _buffer)
{
    Byte* cursor = _buffer + 2;
    size_t sizeOfResponseMessage, optionalIpAddressLength;

    /* Response result unpacking */
    cursor++; /* Skipping the ResponseResult's metadata */
    _response->m_responseResult = (ResponseResult)(*cursor);
    cursor++;

    /* Response message unpacking */
    sizeOfResponseMessage = (size_t)(*(uint16_t*)cursor);
    cursor += 2;
    cursor = CopyBytesAndSetCursorOnNextLocation((Byte*)_response->m_responseMessage, cursor, sizeOfResponseMessage, SOURCE);

    /* Check the tag to know if add the ip address length to the total LENGTH */
    if(*_buffer == CREATE_GROUP_RESPONSE || *_buffer == JOIN_GROUP_RESPONSE)
    {
        /* Adding the size of the IpAddress + 2 for: '\0' and for inner metadata header - to the total LENGTH header */
        optionalIpAddressLength = (size_t)(*cursor);
        cursor++;

        cursor = UnPackIPAddress(((CreateAndJoinGroupResponse*)_response)->m_multicastIPAddress, cursor, optionalIpAddressLength);

        /* Port unpacking */
        ((CreateAndJoinGroupResponse*)_response)->m_port = (int)(*(uint16_t*)cursor);

        return cursor + 2;
    }

    return cursor;
}


static Byte* UnPackIPAddress(char* _ipAddress, Byte* _buffer, size_t _bytesToCopy)
{
    return CopyBytesAndSetCursorOnNextLocation((Byte*)_ipAddress, _buffer, _bytesToCopy, SOURCE);
}