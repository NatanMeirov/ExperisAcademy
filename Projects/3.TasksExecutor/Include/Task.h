/**
 * @file Task.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Task module to manage Tasks objects to execute by an Executor object
 * @version 1.0
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __TASK_H__
#define __TASK_H__

#define _XOPEN_SOURCE 600 /* to define clockid_t */


/* Includes: */

#include <stddef.h> /* size_t */
#include <time.h> /* clockid_t, struct timespec */


/* Defines: */

typedef struct Task Task;

/**
 * @brief A Task function that will be called when the Task is executed
 * @param[in] _context: A pointer to the context to pass to the Task function, when the function is called
 * @return int - 0, if the Task should stop and not be executed again / 1, if the Task should not stop, and have to be executed again in the next time period
 */
typedef int (*TaskFunction)(void* _context);


/*-------------------------------------- Main API Functions ---------------------------------------*/

/**
 * @brief Creates a new Task to be executed by an Executor, initialized with a function to execute and its context
 * @details The Task should be executed again after a given peroid time (in miliseconds)
 * @param[in] _taskFunction: A Task function that will be called when the Task is executed, cannot be NULL
 * @param[in] _taskContext: A context to pass to the Task function, can be NULL
 * @param[in] _clockMode: A clockid_t mode, to use as a time resolution clock of the Task execution
 * @param[in] _executeEveryPeriodTimeInMiliseconds: A period time in miliseconds, that the Task should be executed again after the Task ends, must be more then 0
 * @return Task* - on success / NULL - on failure
 *
 * @warning Make sure to update the next execution time BEFORE executing each Task! (Use: TaskUpdateNextExecutionTime)
 */
Task* TaskCreate(TaskFunction _taskFunction, void* _taskContext, clockid_t _clockMode, size_t _executeEveryPeriodTimeInMiliseconds);


/**
 * @brief Destroys a given Task, NULLs the Task's pointer
 * @param[in] _task: Task to be destroyed
 * @return None
 *
 * @warning The TaskDestroy does not deallocates the _taskContext param, make sure to deallocate it by yourself if such deallocation is required!
 */
void TaskDestroy(Task** _task);


/**
 * @brief Executes a given Task
 * @param[in] _task: A Task to be executed
 * @return int - 0, if the Task should stop and not be executed again or if error has occurred (given pointer is NULL)
               / 1, if the Task should not stop, and have to be executed again in the next execution time
 */
int TaskExecute(Task* _task);


/**
 * @brief Updates the next execution time of the given task
 * @param[in] _task: A Task to update its next execution time, based on the current time
 * @return int - 1, on success / 0, on failure (optional errors: given pointer is NULL)
 */
int TaskUpdateNextExecutionTime(Task* _task);


/**
 * @brief Compares between two given Tasks, checks if the first Task should be executed before the second (other) Task, or not
 * @param[in] _firstTask: The first Task to check if it should be executed before the other
 * @param[in] _secondTask: The second (other) Task to check and compare with the first Task
 * @return int - 0, if the first Task should be executed before the second (other) Task
               / 1, if the second Task should be executed before the first Task
 *
 * @warning The function does not validate if the given pointers are NULL
 */
int TaskIsExecutionBeforeOtherTask(const Task* _firstTask, const Task* _secondTask);

/*----------------------------------- End of Main API Functions -----------------------------------*/

#endif /* #ifndef __TASK_H__ */