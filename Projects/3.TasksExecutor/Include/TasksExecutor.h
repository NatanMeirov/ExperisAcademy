/**
 * @file TasksExecutor.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Tasks Executor to schedule, manage and execute given tasks (functions) by period time, in a correct wanted order
 * @version 1.0
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __TASKSEXECUTOR_H__
#define __TASKSEXECUTOR_H__

#define _XOPEN_SOURCE 600 /* to define clockid_t */


/* Includes: */

#include <stddef.h> /* size_t */
#include <time.h> /* clockid_t, struct timespec */


/* Defines: */

typedef struct TasksExecutor TasksExecutor;


/*-------------------------------------- Main API Functions ---------------------------------------*/

/**
 * @brief Creates a TasksExecutor that can schedule, manage and execute given tasks (functions) by period time, in a correct wanted order
 * @param[in] _nameOfExecutor: The name of the TasksExecutor
 * @param[in] _clockMode: A clockid_t mode that will be used as a time indicator used for scheduling,
                          3 clockid_t modes are supported: CLOCK_REALTIME, CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC
 * @return TasksExecutor* - on success / NULL - on failure
 */
TasksExecutor* TasksExecutorCreate(const char* _nameOfExecutor, clockid_t _clockMode);


/**
 * @brief Destroys and deallocate a given TasksExecutor, NULLs the TasksExecutor's pointer
 * @param[in] _executor: A TasksExecutor to destroy
 * @return None
 */
void TasksExecutorDestroy(TasksExecutor** _executor);


/**
 * @brief Adds a new task to a given TaskExecutor to be executed using TasksExecutorRun function in a wanted order
 * @param[in] _executor: A TaskExecutor to add a task to execute
 * @param[in] _taskFunction: A task function to be executed, cannot be NULL
 * @param[in] _taskContext: The context to pass to the task's function, can be NULL
 * @param[in] _executeEveryPeriodTimeInMiliseconds: A period time in miliseconds, that the Task should be executed again after the Task ends, cannot be 0
 * @return int - 1, on success / 0, on failure (optional errors: given pointer/function pointer is NULL, wrong param value, allocation failed)
 *
 * @warning if _executeEveryPeriodTimeInMiliseconds is 0: function will fail and return 0
 */
int TasksExecutorAdd(TasksExecutor* _executor, int (*_taskFunction)(void *), void* _taskContext, size_t _executeEveryPeriodTimeInMiliseconds);


/**
 * @brief Runs (triggers) TasksExecutor's tasks by their correct period time, added by TasksExecutorAdd function
 * @details The TasksExecutorRun function can stop only in two ways:
            1) The TasksExecutor had executed all its tasks, and finished them all
            2) TasksExecutorPause function was invoked using a previously added task
 * @param[in] _executor: A TasksExecutor to run and execute its given tasks
 * @return size_t - number of execution cycles (user's task functions that had been executed already, on success
                    / MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
                    (optional errors: given pointer is NULL, allocation failed, no tasks to execute)
 */
size_t TasksExecutorRun(TasksExecutor* _executor);


/**
 * @brief Pauses a given TaskExecutor from its running, MUST be called from a task added before by TasksExecutorAdd function
 * @param[in] _executor: A TaskExecutor to pause
 * @return size_t - number of remained pending tasks to be executed by the TasksExecutor, on success
                    / MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
                    (optional errors: given pointer is NULL)
 */
size_t TasksExecutorPause(TasksExecutor* _executor);

/*----------------------------------- End of Main API Functions -----------------------------------*/

#endif /* #ifndef __TASKSEXECUTOR_H__ */