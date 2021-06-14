/* Note: MUST compile with -lpthread (to notify the linker that we are using pthread library!) */

#include "stddef.h" /* size_t */
#include "unistd.h" /* sleep */
#include "pthread.h"
#include "stdio.h"


#define UNUSED(param) ((void)(param))
#define THREADS_COUNT 2
#define REPEATED_MISSIONS_TIMES 10
#define PRINT_TASK(string) { printf(string); sleep(1); }


void* FirstThreadTask(void* _notInUse)
{
    size_t i;
    UNUSED(_notInUse);

    for(i = 0; i < REPEATED_MISSIONS_TIMES; ++i)
    {
        PRINT_TASK("Hello\n");
    }

    return NULL;
}


void* SecondThreadTask(void* _notInUse)
{
    size_t i;
    UNUSED(_notInUse);

    for(i = 0; i < REPEATED_MISSIONS_TIMES; ++i)
    {
        PRINT_TASK("World\n");
    }

    return NULL;
}


void TasksRunner(void)
{
    size_t i;
    pthread_t threadsPool[THREADS_COUNT];

    for(i = 0; i < THREADS_COUNT; ++i)
    {
        if(i % THREADS_COUNT == 0)
        {
            pthread_create(&threadsPool[i], NULL, &FirstThreadTask, NULL);
        }
        else
        {
            pthread_create(&threadsPool[i], NULL, &SecondThreadTask, NULL);
        }
    }

    /* Join: */
    for(i = 0; i < THREADS_COUNT; ++i)
    {
        pthread_join(threadsPool[i], NULL);
    }
}


int main(void)
{
    TasksRunner();

    return 0;
}