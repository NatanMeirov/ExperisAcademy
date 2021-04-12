#define _XOPEN_SOURCE 600 /* to define clockid_t */

#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include <time.h> /* clockid_t */
#include "../Include/TaskTimer.h"

static void Test_Create_TaskTimer(void);
static void Test_Destroy_TaskTimer(void);
static void Test_TaskTimerUpdateNextExecutionTime_TaskTimer(void);
static void Test_TaskTimerExecutionTimeCompartor_TaskTimer(void);
static void Test_TaskTimerWaitTillIsExecutionTime_TaskTimer(void);


int main(void)
{
    Test_Create_TaskTimer();
    Test_Destroy_TaskTimer();
    Test_TaskTimerUpdateNextExecutionTime_TaskTimer();
    Test_TaskTimerExecutionTimeCompartor_TaskTimer();
    Test_TaskTimerWaitTillIsExecutionTime_TaskTimer();

    return 0;
}

static void Test_Create_TaskTimer(void)
{
    TaskTimer* timer = TaskTimerCreate(CLOCK_REALTIME, 10000);
    TaskTimer* timer2 = TaskTimerCreate(CLOCK_MONOTONIC, 100000);
    TaskTimer* timer3 = TaskTimerCreate(CLOCK_REALTIME_COARSE, 1000000);

    if(timer && timer2 && timer3)
    {
        printf("Test_Create_TaskTimer: Passed!\n");
    }
    else
    {
        printf("Test_Create_TaskTimer: Failed...\n");
    }

    TaskTimerDestroy(&timer);
    TaskTimerDestroy(&timer2);
    TaskTimerDestroy(&timer3);
}

static void Test_Destroy_TaskTimer(void)
{
    TaskTimer* timer = TaskTimerCreate(CLOCK_REALTIME, 10000);
    TaskTimer* timer2 = TaskTimerCreate(CLOCK_MONOTONIC, 100000);
    TaskTimer* timer3 = TaskTimerCreate(CLOCK_REALTIME_COARSE, 1000000);
    TaskTimer* null = NULL;

    TaskTimerDestroy(&timer);
    TaskTimerDestroy(&timer2);
    TaskTimerDestroy(&timer3);

    if(!timer && !timer2 && !timer3)
    {
        TaskTimerDestroy(NULL); /* Should do nothing */
        TaskTimerDestroy(&null); /* Should do nothing as well */
        TaskTimerDestroy(&timer); /* Should do nothing as well */

        printf("Test_Destroy_TaskTimer: Passed!\n");
    }
    else
    {
        printf("Test_Destroy_TaskTimer: Failed...\n");
    }
}


static void Test_TaskTimerUpdateNextExecutionTime_TaskTimer(void)
{
    TaskTimer* timer = TaskTimerCreate(CLOCK_REALTIME, 10000);
    TaskTimer* timer2 = TaskTimerCreate(CLOCK_MONOTONIC, 100000);
    TaskTimer* timer3 = TaskTimerCreate(CLOCK_REALTIME_COARSE, 1000000);

    if(TaskTimerUpdateNextExecutionTime(timer) && TaskTimerUpdateNextExecutionTime(timer2) && TaskTimerUpdateNextExecutionTime(timer3))
    {
        printf("Test_TaskTimerUpdateNextExecutionTime_TaskTimer: Passed!\n");
    }
    else
    {
        printf("Test_TaskTimerUpdateNextExecutionTime_TaskTimer: Failed...\n");
    }

    TaskTimerDestroy(&timer);
    TaskTimerDestroy(&timer2);
    TaskTimerDestroy(&timer3);
}


static void Test_TaskTimerExecutionTimeCompartor_TaskTimer(void)
{
    TaskTimer* timer1 = TaskTimerCreate(CLOCK_REALTIME, 10000);
    TaskTimer* timer1T = TaskTimerCreate(CLOCK_REALTIME, 20000);

    TaskTimer* timer2 = TaskTimerCreate(CLOCK_MONOTONIC, 100000);
    TaskTimer* timer2T = TaskTimerCreate(CLOCK_MONOTONIC, 200000);

    TaskTimer* timer3 = TaskTimerCreate(CLOCK_REALTIME_COARSE, 100);
    TaskTimer* timer3T = TaskTimerCreate(CLOCK_REALTIME_COARSE, 2000000);


    TaskTimerUpdateNextExecutionTime(timer1);
    TaskTimerUpdateNextExecutionTime(timer2);
    TaskTimerUpdateNextExecutionTime(timer3);
    TaskTimerUpdateNextExecutionTime(timer1T);
    TaskTimerUpdateNextExecutionTime(timer2T);
    TaskTimerUpdateNextExecutionTime(timer3T);


    if(TaskTimerExecutionTimeCompartor(timer1, timer1T) == 0 /* timer1 first */
    && TaskTimerExecutionTimeCompartor(timer2T, timer2) == 1 /* timer2 first */
    && TaskTimerExecutionTimeCompartor(timer3, timer3T) == 0 /* timer3 first */)
    {
        printf("Test_TaskTimerExecutionTimeCompartor_TaskTimer: Passed!\n");
    }
    else
    {
        printf("Test_TaskTimerExecutionTimeCompartor_TaskTimer: Failed...\n");
    }

    TaskTimerDestroy(&timer1);
    TaskTimerDestroy(&timer2);
    TaskTimerDestroy(&timer3);
    TaskTimerDestroy(&timer1T);
    TaskTimerDestroy(&timer2T);
    TaskTimerDestroy(&timer3T);
}


static void Test_TaskTimerWaitTillIsExecutionTime_TaskTimer(void)
{
    TaskTimer* timer1 = TaskTimerCreate(CLOCK_REALTIME, 5000);
    TaskTimer* timer2 = TaskTimerCreate(CLOCK_MONOTONIC, 10000);
    TaskTimer* timer3 = TaskTimerCreate(CLOCK_REALTIME_COARSE, 15000);


    TaskTimerUpdateNextExecutionTime(timer1);
    TaskTimerUpdateNextExecutionTime(timer2);
    TaskTimerUpdateNextExecutionTime(timer3);


    printf("Check (manually) if every timer is blocked (sleep) for ~5 seconds:\n");

    TaskTimerWaitTillIsExecutionTime(timer1);
    printf("-> timer1's signal\n");

    TaskTimerWaitTillIsExecutionTime(timer2);
    printf("-> timer2's signal\n");

    TaskTimerWaitTillIsExecutionTime(timer3);
    printf("-> timer3's signal\n");

    TaskTimerDestroy(&timer1);
    TaskTimerDestroy(&timer2);
    TaskTimerDestroy(&timer3);
}