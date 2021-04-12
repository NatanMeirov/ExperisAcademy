/**
 * @file TaskTimer.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Task Timer module to manage Tasks objects times
 * @version 1.0
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __TASKTIMER_H__
#define __TASKTIMER_H__

#define _XOPEN_SOURCE 600 /* to define clockid_t */


/* Includes: */

#include <stddef.h> /* size_t */
#include <time.h> /* clockid_t, struct timespec */


/* Defines: */

typedef struct TaskTimer TaskTimer;


/*-------------------------------------- Main API Functions ---------------------------------------*/

/**
 * @brief Creates a TaskTimer, initialized to an empty timer
 * @param[in] _clockMode: A clockid_t mode, to use as a time resolution clock of the Task execution
 * @param[in] _executeEveryPeriodTimeInMiliseconds: A period time in miliseconds, that the Task should be executed again after the Task ends
 * @return TaskTimer* - on success / NULL - on failure
 *
 * @warning Make sure to update the next execution time BEFORE executing each Task! (Use: TaskTimerUpdateNextExecutionTime)
 */
TaskTimer* TaskTimerCreate(clockid_t _clockMode, size_t _executeTaskEveryPeriodTimeInMiliseconds);


/**
 * @brief Destroys a given TaskTimer, NULLs the TaskTimer's pointer
 * @param[in] _timer: TaskTimer to be destroyed
 * @return None
 */
void TaskTimerDestroy(TaskTimer** _timer);


/**
 * @brief Updates the next execution time of the given TaskTimer
 * @param[in] _timer: A TaskTimer to update its next execution time, based on the current time
 * @return int - 1, on success / 0, on failure
 */
int TaskTimerUpdateNextExecutionTime(TaskTimer* _timer);


/**
 * @brief Compares between two given TaskTimers of two Tasks, checks if the first Task should be executed before the second (other) Task, or not
 * @param[in] _firstTaskTimer: The timer of the first Task to check if it should be executed before the other
 * @param[in] _secondTaskTimer: The timer of the second (other) Task to check and compare with the first Task
 * @return int - 0, if the first Task should be executed before the second (other) Task
               / 1, if the second Task should be executed before the first Task
 *
 * @warning The function does not validate if the given pointers are NULL
 */
int TaskTimerExecutionTimeCompartor(const TaskTimer* _firstTaskTimer, const TaskTimer* _secondTaskTimer);


/**
 * @brief Checks if the time to execute the Task is reached, if not, waits (blocks the Task) till the execution time of the Task is finally reached
 * @param[in] _timer: A timer to check if wait (block) is required before executing
 * @return None
 */
void TaskTimerWaitTillIsExecutionTime(TaskTimer* _timer);

/*----------------------------------- End of Main API Functions -----------------------------------*/

#endif /* #ifndef __TASKTIMER_H__ */