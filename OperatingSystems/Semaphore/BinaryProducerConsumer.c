/* We can use many threads as well, and not only 1 - 1 threads, but we must use the 3 semaphores */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define PRODUCTS_TO_PRODUCE 100
#define PRODUCTS_TO_CONSUME 0

static void* BinaryProducer(void* context);
static void* BinaryConsumer(void* context);

static sem_t g_lock; /* Binary semaphore - used as a lock */
static sem_t g_producerSem;
static sem_t g_consumerSem;

static int g_criticalSection = 0;

int main(void)
{
    pthread_t threads[2];

    sem_init(&g_lock, 0, 1); /* Value is 1 - Binary Semaphore */
    sem_init(&g_producerSem, 0, PRODUCTS_TO_PRODUCE); /* Value is 1 - Binary Semaphore */
    sem_init(&g_consumerSem, 0, PRODUCTS_TO_CONSUME); /* Value is 1 - Binary Semaphore */

    pthread_create(&threads[0], NULL, &BinaryProducer, NULL);
    pthread_create(&threads[1], NULL, &BinaryConsumer, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

static void* BinaryProducer(void* context)
{
    while(1)
    {
        sem_wait(&g_producerSem); /* Wait to see if need more produces (if did not reached to 0), so -1 here */
        sem_wait(&g_lock); /* lock the critical section */

        g_criticalSection++; /* item produced */
        printf("[Producer:] Critical Section: %d\n", g_criticalSection);
        /*sleep(1);*/

        sem_post(&g_lock); /* unlock the critical section */
        sem_post(&g_consumerSem); /* +1 to notify the consumer that there are more products to consume */
    }
}

static void* BinaryConsumer(void* context)
{
    while(1)
    {
        sem_wait(&g_consumerSem); /* Wait to see if need more produces (if did not reached to 0), so -1 here */
        sem_wait(&g_lock); /* lock the critical section */

        g_criticalSection--; /* item consumed */
        printf("[Consumer:] Critical Section: %d\n", g_criticalSection);
        /*sleep(1);*/

        sem_post(&g_lock); /* unlock the critical section */
        sem_post(&g_producerSem); /* +1 to notify the producer that there are more room to produce more products */
    }
}