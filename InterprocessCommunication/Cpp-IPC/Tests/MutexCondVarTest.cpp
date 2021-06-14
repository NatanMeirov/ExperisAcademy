#include "stddef.h" /* size_t */
#include "unistd.h" /* sleep */
#include "pthread.h"
#include "stdio.h"
#include "../Mutex.hpp"
#include "../ConditionalVariable.hpp"


#define UNUSED(param) ((void)(param))
#define THREADS_COUNT 2
#define REPEATED_MISSIONS_TIMES 1
#define PRINT_TASK(string) { printf(string); sleep(1); }
bool g_hasPrinted = false;
nm::Mutex g_lock;
nm::ConditionalVariable g_condVar;


void* FirstThreadTask(void* _notInUse)
{
    g_lock.Lock();
    size_t i;
    UNUSED(_notInUse);

    for(i = 0; i < REPEATED_MISSIONS_TIMES; ++i)
    {
        PRINT_TASK("Hello ");
    }

    g_hasPrinted = true;
    g_condVar.Signal();
    g_lock.Unlock();
    return NULL;
}


void* SecondThreadTask(void* _notInUse)
{
    g_lock.Lock();
    size_t i;
    UNUSED(_notInUse);

    if(!g_hasPrinted) {
        g_condVar.Wait(g_lock);
    }

    for(i = 0; i < REPEATED_MISSIONS_TIMES; ++i)
    {
        PRINT_TASK("World!\n");
    }

    g_lock.Unlock();

    return NULL;
}


void TasksRunner(void)
{
    size_t i;
    pthread_t threadsPool[2];

    pthread_create(&threadsPool[0], NULL, &SecondThreadTask, NULL);
    sleep(3); // To force blocking through the Mutex
    pthread_create(&threadsPool[1], NULL, &FirstThreadTask, NULL);

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