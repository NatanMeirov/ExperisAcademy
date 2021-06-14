#include <pthread.h>
#include <stdio.h>


int print_hello_has_done = 0; /* the condition flag */
pthread_cond_t convar;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void* printHello(void* _notInUse)
{
	printf("hello ");

	pthread_mutex_lock(&mutex);
	print_hello_has_done = 1;
	pthread_cond_signal(&convar); // Wake up the other thread
	pthread_mutex_unlock(&mutex);

    return NULL;
}

void* printWorld(void* _notInUse)
{
	pthread_mutex_lock(&mutex);

	while(print_hello_has_done == 0){
		pthread_cond_wait(&convar, &mutex);
	}

	printf("world! ");

	pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(){
	pthread_t thread1, thread2, thread3, thread4;
    pthread_create(&thread2, NULL, printWorld, NULL);
    pthread_create(&thread3, NULL, printWorld, NULL);
    pthread_create(&thread4, NULL, printHello, NULL);
    pthread_create(&thread1, NULL, printHello, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

    putchar('\n');

	return 0;
}