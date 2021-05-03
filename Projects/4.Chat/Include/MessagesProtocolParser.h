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

#ifndef __MESSAGESPROTOCOLPARSER_H__
#define __MESSAGESPROTOCOLPARSER_H__


/* Includes: */

#include <stddef.h> /* size_t */


/* Defines: */

#define BIG_BUFFER_LENGTH 3800
#define CHARACTERS_LIMIT 256
#define IP_LENGTH 16

typedef unsigned char Byte;

typedef enum ProtocolParsingTag
{
    SIGN_UP_REQUEST = 1,
    SIGN_UP_RESPONSE,
    LOGIN_REQUEST,
    LOGIN_RESPONSE,
    LOGOUT_REQUEST,
    LOGOUT_RESPONSE,
    CREATE_GROUP_REQUEST,
    CREATE_GROUP_RESPONSE,
    JOIN_GROUP_REQUEST,
    JOIN_GROUP_RESPONSE,
    LEAVE_GROUP_REQUEST,
    LEAVE_GROUP_RESPONSE,
    ERROR_TAG = 99 /* An error tag to mark if there is an error with a given message */
} ProtocolParsingTag;

typedef enum ResponseResult
{
    RESPONSE_OK, /* Fit all responses */
    RESPONSE_MEMORY_ALLOCATION_FAILED, /* Fit all responses */
    RESPONSE_FAILED_TO_RECEIVE_COMPLETE_MESSAGE, /* Fit all responses */
    RESPONSE_USER_ALREADY_LOGGED_IN, /* Login */
    RESPONSE_USERNAME_ALREADY_EXISTS, /* Sign Up */
    RESPONSE_USERNAME_NOT_EXISTS, /* Login */
    RESPONSE_WRONG_PASSWORD, /* Login */
    RESPONSE_GROUP_NAME_ALREADY_EXISTS, /* Create Group */
    RESPONSE_USER_ALREADY_JOINED_GROUP, /* Join Group */
    RESPONSE_GROUP_NAME_NOT_EXISTS, /* Join Group (there is not a group with this name), Leave Group (the user is not in this group) */
    RESPONSE_ALL_AVAILABLE_GROUPS_ARE_IN_USE
} ResponseResult;

/* Sign Up: */

typedef struct SignUpRequest /* SIGN_UP_REQUEST tag */
{
    char m_usernameToSignUp[CHARACTERS_LIMIT]; /* Unique key */
    char m_password[CHARACTERS_LIMIT];
} SignUpRequest;

typedef struct SignUpResponse /* SIGN_UP_RESPONSE tag */
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
} SignUpResponse;

/* Log In: */

typedef struct LoginRequest /* LOGIN_REQUEST tag */
{
    char m_usernameToLogin[CHARACTERS_LIMIT]; /* Unique key */
    char m_password[CHARACTERS_LIMIT];
} LoginRequest;

typedef struct LoginResponse /* LOGIN_RESPONSE tag */
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
} LoginResponse;

/* Log Out: */

typedef struct LogoutRequest /* LOGOUT_REQUEST tag */
{
    char m_usernameToLogout[CHARACTERS_LIMIT];
} LogoutRequest;

typedef struct LogoutResponse /* LOGOUT_RESPONSE tag */
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
} LogoutResponse;

/* Create Group: */

typedef struct CreateGroupRequest /* CREATE_GROUP_REQUEST tag */
{
    char m_groupNameToCreate[CHARACTERS_LIMIT];
    char m_usernameOfGroupCreator[CHARACTERS_LIMIT];
} CreateGroupRequest;

typedef struct CreateGroupResponse /* CREATE_GROUP_RESPONSE tag */
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
    char m_multicastIPAddress[IP_LENGTH];
    int m_port;
} CreateGroupResponse;

/* Join Group: */

typedef struct JoinGroupRequest /* JOIN_GROUP_REQUEST tag */
{
    char m_groupNameToJoin[CHARACTERS_LIMIT];
    char m_usernameToJoin[CHARACTERS_LIMIT];
} JoinGroupRequest;

typedef struct JoinGroupResponse /* JOIN_GROUP_RESPONSE tag */
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH]; /* To support showing the user all available groups to join */
    char m_multicastIPAddress[IP_LENGTH];
    int m_port;
} JoinGroupResponse;

/* Leave Group: */

typedef struct LeaveGroupRequest /* LEAVE_GROUP_REQUEST tag */
{
    char m_groupNameToLeave[CHARACTERS_LIMIT];
    char m_usernameToLeave[CHARACTERS_LIMIT];
} LeaveGroupRequest;

typedef struct LeaveGroupResponse /* LEAVE_GROUP_RESPONSE tag */
{
    ResponseResult m_responseResult;
    char m_responseMessage[BIG_BUFFER_LENGTH];
} LeaveGroupResponse;

/* ------------------------------------- Main API Functions -------------------------------------- */

/**
 * @brief An util function used to pack a given message object, recognized by a specific tag, to a stream of bytes - parsed through an agreed protocol
 * @details The function will read the message object (recognized by a unique tag), and copy its content after parsing it - into the buffer of Bytes
 *          The buffer will contain:
 *          - Header:
 *              * 1 Byte: Tag - to mark the tag of the message
 *              * 1 Byte: Length - to specify the length of the message in bytes (WITHOUT counting the extension 2 bytes of the Header)
 *          - Message content:
 *              * {Length} Bytes: The content of the message (size of Length bytes)
 * @param[in] _messageObject: A message object to pack (parse to a Bytes buffer stream using the agreed protocol), the object must fit the given tag for successful parsing
 * @param[out] _buffer: A buffer to store the Bytes stream (message) in (and return it by reference), the buffer must be initialized and big enough to store the bytes stream in
 * @param[in] _tag: A special tag used by the function to specify how to pack the message
 * @return size_t - total size of message (including the headers), on success / 0, on failure
 */
size_t PackMessageByTag(void* _messageObject, Byte* _buffer, ProtocolParsingTag _tag);

/**
 * @brief An util function used to unpack a given stream of bytes, recognized by a specific tag, to a message object - parsed through an agreed protocol
 * @details The function will read the message object (recognized by a unique tag), and copy its content after parsing it - into the buffer of Bytes
 *          The buffer will contain:
 *          - Header:
 *              * 1 Byte: Tag - to mark the tag of the message
 *              * 1 Byte: Length - to specify the length of the message in bytes (WITHOUT counting the extension 2 bytes of the Header)
 *          - Message content:
 *              * {Length} Bytes: The content of the message (size of Length bytes)
 * @param[out] _messageObject: A message object to store the parsed bytes stream from the buffer, using the agreed protocol, the object must be initialized and fit the given tag for successful parsing
 * @param[in] _buffer: A buffer that stores a Bytes stream (message) to unpack and parse its content
 * @param[in] _tag: A special tag used by the function to specify how to pack the message
 * @return int - 1, on success / 0, on failure
 *
 * @warning The UnPack function assumes that the message has a correct size, to verify that - please use the IsCorrectMessageSize API function first
 */
int UnPackMessageByTag(void* _messageObject, Byte* _buffer, ProtocolParsingTag _tag);


/**
 * @brief Gets the tag of a given message (buffer)
 * @param _buffer: A message to get its tag from
 * @return ProtocolParsingTag - the message's tag, on success / ERROR_TAG, on failure
 */
ProtocolParsingTag GetMessageTag(Byte* _buffer);


/**
 * @brief Checks if the size of the buffer (message) is equal to the given size
 * @param _buffer: A message to check
 * @param _messageSizeInBytes: The desired message size (to check if there is a match with the given message's size)
 * @return int - 1, if true / 0, if false
 */
int IsCorrectMessageSize(Byte* _buffer, size_t _messageSizeInBytes);

/* ---------------------------------- End of Main API Functions ----------------------------------- */

#endif /* #ifndef __MESSAGESPROTOCOLPARSER_H__ */