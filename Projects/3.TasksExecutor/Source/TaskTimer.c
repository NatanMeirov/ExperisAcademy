/**
 * @file TaskTimer.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Task Timer module to manage Tasks objects times
 * @version 1.0
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#define _XOPEN_SOURCE 600 /* to define clockid_t and usleep */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <time.h> /* clockid_t, struct timespec */
#include <unistd.h> /* usleep */
#include "../Include/TaskTimer.h"


/* Defines: */

#define SECONDS_TO_MILISECONDS(seconds) ((size_t)((seconds) * 1000))
#define MILISECONDS_TO_SECONDS(miliseconds) ((size_t)(((double) miliseconds) / 1000))
#define NANOSECONDS_TO_MILISECONDS(nanoseconds) (((double) (nanoseconds)) / 1000000)
#define MILISECONDS_TO_MICROSECONDS(miliseconds) ((size_t)((miliseconds) * 1000))

struct TaskTimer
{
    size_t m_executeTaskEveryPeriodTimeInMiliseconds;
    clockid_t clockMode;
    struct timespec m_currentTime;
    struct timespec m_taskNextExecutionTime;
};


/* Helper Functions Declarations: */

static void UpdateCurrentTime(TaskTimer* _timer);
static void UpdateNextExecutionTime(TaskTimer* _timer);
static long CalculateGapTimeInMiliseconds(TaskTimer* _timer);


/*-------------------------------------- Main API Functions ---------------------------------------*/

TaskTimer* TaskTimerCreate(clockid_t _clockMode, size_t _executeTaskEveryPeriodTimeInMiliseconds)
{
    TaskTimer* newTaskTimer = malloc(sizeof(TaskTimer));
    if(!newTaskTimer)
    {
        return NULL;
    }

    newTaskTimer->clockMode = _clockMode;

    newTaskTimer->m_executeTaskEveryPeriodTimeInMiliseconds = _executeTaskEveryPeriodTimeInMiliseconds;

    /* Lazy initialization */
    newTaskTimer->m_currentTime.tv_sec = 0;
    newTaskTimer->m_currentTime.tv_nsec = 0;

    newTaskTimer->m_taskNextExecutionTime.tv_sec = 0;
    newTaskTimer->m_taskNextExecutionTime.tv_nsec = 0;

    return newTaskTimer;
}


void TaskTimerDestroy(TaskTimer** _timer)
{
    if(_timer && *_timer)
    {
        free(*_timer);
        *_timer = NULL;
    }
}


int TaskTimerUpdateNextExecutionTime(TaskTimer* _timer)
{
    if(!_timer)
    {
        return 0; /* Error */
    }

    UpdateCurrentTime(_timer);
    UpdateNextExecutionTime(_timer);

    return 1; /* Ok */
}


int TaskTimerExecutionTimeCompartor(const TaskTimer* _firstTaskTimer, const TaskTimer* _secondTaskTimer)
{
    return SECONDS_TO_MILISECONDS(_secondTaskTimer->m_taskNextExecutionTime.tv_sec) + NANOSECONDS_TO_MILISECONDS(_secondTaskTimer->m_taskNextExecutionTime.tv_nsec) < SECONDS_TO_MILISECONDS(_firstTaskTimer->m_taskNextExecutionTime.tv_sec) + NANOSECONDS_TO_MILISECONDS(_firstTaskTimer->m_taskNextExecutionTime.tv_nsec);
}


void TaskTimerWaitTillIsExecutionTime(TaskTimer* _timer)
{
    long gapTime;

    if(!_timer)
    {
        return;
    }

    UpdateCurrentTime(_timer);

    gapTime = CalculateGapTimeInMiliseconds(_timer);

    if(gapTime > 0)
    {
        usleep(MILISECONDS_TO_MICROSECONDS(gapTime));
    }
}

/*----------------------------------- End of Main API Functions -----------------------------------*/


/* Helper Functions: */

static void UpdateCurrentTime(TaskTimer* _timer)
{
    clock_gettime(_timer->clockMode, &_timer->m_currentTime);
}


static void UpdateNextExecutionTime(TaskTimer* _timer)
{
    _timer->m_taskNextExecutionTime.tv_sec = MILISECONDS_TO_SECONDS(SECONDS_TO_MILISECONDS(_timer->m_currentTime.tv_sec) + _timer->m_executeTaskEveryPeriodTimeInMiliseconds);
    _timer->m_taskNextExecutionTime.tv_nsec = _timer->m_currentTime.tv_nsec;
}


static long CalculateGapTimeInMiliseconds(TaskTimer* _timer)
{
    return SECONDS_TO_MILISECONDS(_timer->m_taskNextExecutionTime.tv_sec) + NANOSECONDS_TO_MILISECONDS(_timer->m_taskNextExecutionTime.tv_nsec) - SECONDS_TO_MILISECONDS(_timer->m_currentTime.tv_sec) + NANOSECONDS_TO_MILISECONDS(_timer->m_currentTime.tv_nsec);
}