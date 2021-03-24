/**
 * @file UI.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief UI utils functions module header file
 * @version 0.1
 * @date 2021-03-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __UI__H__
#define __UI__H__


/* Defines: */

#include <stddef.h> /* size_t */

/* --------------------------------- STDOUT UI SUPPORTED FORMATS --------------------------------- */

/* bold/not bold */

#define BOLD_ON "\x1b[1m"
#define BOLD_OFF "\x1b[21m"

/* colors */

#define NORMAL "\x1B[0m"
#define BLACK "\x1b[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define GRAY "\x1b[90m"

/*------------------------------- END OF STDOUT UI SUPPORTED FORMATS -------------------------------*/


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Shows a given message in the User Interface
 * 
 * @param _msg: A string message to show in the User Interface
 * @param _specialFormat: A special format to show the given message in the User Interface with, should be NULL if special format is not needed [works only if the UI system supports these formats]
 */
void SendMessageToUI(const char* _msg, const char* _specialFormat);


/**
 * @brief Gets an input as a string through the User Interface, and put it in a buffer with a length limit (further characters would be ignored)
 * 
 * @param _buffer: The buffer to put the input in
 * @param _maxBufferCapacity: The maximum capacity of the buffer
 */
void GetInputFromUI(char* _buffer, size_t _maxBufferCapacity);


/**
 * @brief Clears and resets the User Interface
 * 
 */
void ClearUI(void);


/**
 * @brief Shows a given error message in the User Interface
 * 
 * @param _errMsg: A string error message to show in the User Interface
 */
void SendErrorToUI(const char* _errMsg);


/**
 * @brief Shows a given warning message in the User Interface
 * 
 * @param _warnMsg: A string warning message to show in the User Interface
 */
void SendWarningToUI(const char* _warnMsg);


/**
 * @brief Shows a given notification message in the User Interface
 * 
 * @param _notifyMsg: A string notification message to show in the User Interface
 */
void SendNotificationToUI(const char* _notifyMsg);


/*--------------------------------- End of Main API Functions -------------------------------------*/


#endif  /* #ifndef __UI__H__ */