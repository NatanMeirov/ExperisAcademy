#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create, pthread_exit, pthread_join  */

#define NUM_OF_THREADS 1000
#define LOOPS 10000 /* Will make race condition */

void* ThreadFunction(void* _threadIndex);

int g_counter = 0;

int main(void)
{
    int status;
    size_t i;
    pthread_t threadsPool[NUM_OF_THREADS];

    for(i = 1; i <= NUM_OF_THREADS; i++)
    {
        status = pthread_create(&threadsPool[i - 1], NULL, &ThreadFunction, (void*) i);
        if(status)
        {
            printf("Error: failed to create thread %ld\n", i);
            return 1;
        }
    }

    for(i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_join(threadsPool[i], NULL);
    }

    return 0;
}

void* ThreadFunction(void* _threadIndex)
{
    size_t i;

    for(i = 0; i < LOOPS; i++)
    {
        g_counter++;
        printf("In thread %ld - Local: %ld, Global: %d\n", (size_t)_threadIndex, i, g_counter);
    }

    pthread_exit(NULL);
}