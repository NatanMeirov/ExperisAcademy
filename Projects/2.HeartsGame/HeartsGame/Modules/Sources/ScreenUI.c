/**
 * @file UI.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Screen UI utils functions module source file
 * @version 0.1
 * @date 2021-03-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include "../../Modules/Headers/UI.h"
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, fflush */
#include <stdlib.h> /* system */
#include <string.h> /* strlen */


/*-------------------------------------- Main API Functions: ---------------------------------------*/


void SendMessageToUI(const char* _msg, const char* _specialFormat)
{
    if(_msg)
    {
        fflush(stdout);
        if(_specialFormat)
        {
            printf("%s%s%s\n", _specialFormat, _msg, NORMAL);
        }
        else
        {
            printf("%s\n", _msg);
        }
        fflush(stdout);
    }
}


void GetInputFromUI(char* _buffer, size_t _maxBufferCapacity)
{
    if(_buffer)
    {
        fflush(stdin);
        fgets(_buffer, _maxBufferCapacity, stdin);

        while(strlen(_buffer) == 1)
        {
            fgets(_buffer, _maxBufferCapacity, stdin);
        }

        if(_buffer[strlen(_buffer) - 1] == '\n')
        {
            _buffer[strlen(_buffer) - 1] = '\0'; /* Removing the '\n' from the end of the string */
        }
        fflush(stdin);
    }
}


void ClearUI(void)
{
    system("clear");
}


void SendErrorToUI(const char* _errMsg)
{
    if(_errMsg)
    {
        fflush(stdout);
        printf("%sError: %s%s\n", RED, _errMsg, NORMAL);
        fflush(stdout);
    }
}


void SendWarningToUI(const char* _warnMsg)
{
    if(_warnMsg)
    {
        fflush(stdout);
        printf("%sWarning: %s%s\n", YELLOW, _warnMsg, NORMAL);
        fflush(stdout);
    }
}


void SendNotificationToUI(const char* _notifyMsg)
{
    if(_notifyMsg)
    {
        fflush(stdout);
        printf("%sNotification: %s%s\n", BLUE, _notifyMsg, NORMAL);
        fflush(stdout);
    }
}


/*--------------------------------- End of Main API Functions -------------------------------------*/