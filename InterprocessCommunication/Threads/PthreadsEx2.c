/* Note: MUST compile with -lpthread (to notify the linker that we are using pthread library!) */

#include "stddef.h" /* size_t */
#include "unistd.h" /* sleep */
#include "pthread.h"
#include "stdio.h"


#define THREADS_COUNT 2
#define REPEATED_MISSIONS_TIMES 10
#define PRINT_TASK(string) { printf("%s\n", string); sleep(1); }


void* ThreadPrintTask(void* _stringToPrint)
{
    size_t i;

    for(i = 0; i < REPEATED_MISSIONS_TIMES; ++i)
    {
        PRINT_TASK((const char*)_stringToPrint);
    }

    return NULL;
}


void TasksRunner(void)
{
    size_t i;
    pthread_t threadsPool[THREADS_COUNT];

    const char* firstname = "Natan";
    const char* lastname = "Meirov";

    for(i = 0; i < THREADS_COUNT; ++i)
    {
        if(i % THREADS_COUNT == 0)
        {
            pthread_create(&threadsPool[i], NULL, &ThreadPrintTask, (void*)firstname);
        }
        else
        {
            pthread_create(&threadsPool[i], NULL, &ThreadPrintTask, (void*)lastname);
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