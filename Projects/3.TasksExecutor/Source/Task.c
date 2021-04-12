/**
 * @file Task.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Task module to manage Tasks objects to execute by an Executor object
 * @version 1.0
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#define _XOPEN_SOURCE 600 /* to define clockid_t */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include <time.h> /* clockid_t, struct timespec */
#include "../Include/TaskTimer.h"
#include "../Include/Task.h"


/* Defines: */

struct Task
{
    TaskFunction m_taskFunction;
    void* m_taskContext;
    TaskTimer* m_taskTimer;
};


/* Helper Function Declarations: */

static Task* InitializeNewTask(Task* _task, TaskFunction _taskFunction, void* _taskContext, clockid_t _clockMode, size_t _executeTaskEveryPeriodTimeInMiliseconds);


/*-------------------------------------- Main API Functions ---------------------------------------*/

Task* TaskCreate(TaskFunction _taskFunction, void* _taskContext, clockid_t _clockMode, size_t _executeTaskEveryPeriodTimeInMiliseconds)
{
    Task* newTask = NULL;

    if(!_taskFunction)
    {
        return NULL;
    }

    newTask = malloc(sizeof(Task));
    if(!newTask)
    {
        return NULL;
    }

    return InitializeNewTask(newTask, _taskFunction, _taskContext, _clockMode, _executeTaskEveryPeriodTimeInMiliseconds);
}


void TaskDestroy(Task** _task)
{
    if(_task && *_task)
    {
        if((*_task)->m_taskTimer)
        {
            TaskTimerDestroy(&((*_task)->m_taskTimer));
        }

        free(*_task);
        *_task = NULL;
    }
}


int TaskExecute(Task* _task)
{
    if(!_task)
    {
        return 0;
    }

    TaskTimerWaitTillIsExecutionTime(_task->m_taskTimer);

    return _task->m_taskFunction(_task->m_taskContext);
}


int TaskUpdateNextExecutionTime(Task* _task)
{
    if(!_task)
    {
        return 0; /* Error */
    }

    return TaskTimerUpdateNextExecutionTime(_task->m_taskTimer);
}


int TaskIsExecutionBeforeOtherTask(const Task* _firstTask, const Task* _secondTask)
{
    return TaskTimerExecutionTimeCompartor(_firstTask->m_taskTimer, _secondTask->m_taskTimer);
}


/*----------------------------------- End of Main API Functions -----------------------------------*/


/* Helper Functions: */

static Task* InitializeNewTask(Task* _task, TaskFunction _taskFunction, void* _taskContext, clockid_t _clockMode, size_t _executeTaskEveryPeriodTimeInMiliseconds)
{
    _task->m_taskFunction = _taskFunction;
    _task->m_taskContext = _taskContext;

    _task->m_taskTimer = TaskTimerCreate(_clockMode, _executeTaskEveryPeriodTimeInMiliseconds);
    if(!_task->m_taskTimer)
    {
        return NULL;
    }

    return _task;
}