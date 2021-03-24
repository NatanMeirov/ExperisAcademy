#include <unistd.h> /* sleep */
#include <stdio.h> /* printf */

#define LOOPS 30

int globalVar = 0;

int main(void)
{
    int localVar = 0;
    size_t i;

    int pid = fork();

    if(pid == -1)
    {
        /* Error handling */
        printf("Error has occurred while trying to create new proccess...\nExiting...");
        return 1; /* Error */
    }else if(pid != 0)
    {
        /* Parent proccess */
        for(i = 0; i < LOOPS; i++)
        {
            printf("----------------------------------------\n");
            printf("In Parent:\n");
            globalVar++;
            printf("globalVar: %d\n", globalVar);
            localVar++;
            printf("localVar: %d\n", localVar);
            printf("----------------------------------------\n");
            sleep(1);
        }
    }
    else /* pid == 0 */
    {
        /* Child proccess */
        for(i = 0; i < LOOPS; i++)
        {
            printf("----------------------------------------\n");
            printf("In Child:\n");
            globalVar--;
            printf("globalVar: %d\n", globalVar);
            localVar--;
            printf("localVar: %d\n", localVar);
            printf("----------------------------------------\n");
            sleep(1);
        }
    }

    return 0;
}