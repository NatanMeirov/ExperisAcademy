#include <stdio.h> /* printf */
#include <stdlib.h>

/* On 32-bit: all the variable are transferred through the stack, on 64-bit: 6 first variables are passed by registers and from the 7-th and above through the stack */
void StackSmashFunction(int x1, int x2, int x3, int x4, int x5, int x6, int x7);
void SomeFunction(void);

void Smashing(void)
{
    printf("In Smashing\n");
    exit(0);
}



int main(void)
{
    printf("Begin main()\n");
    SomeFunction();
    printf("End main()\n");

    return 0;
}

void StackSmashFunction(int x1, int x2, int x3, int x4, int x5, int x6, int x7)
{
    void** seventhVariableAddress = (void*)&x7;

    printf("Begin StackSmashFunction()\n");

    printf("seventhVariableAddress Address before offseting: %p\n", seventhVariableAddress);
    /* MUST DO CASTING TO CHAR TO JUMP BACK ONLY ONE BYTE (CHAR) BACK, BECAUSE INT WILL JUMP BACK 4 BYTES BACK... */
    seventhVariableAddress--; /* Jumping to the beginning of the STACK POINTER (offset of pointer) */
    printf("seventhVariableAddress Address after offseting: %p\n", seventhVariableAddress);
    printf("Before change: %p\n", seventhVariableAddress);
    *seventhVariableAddress = Smashing; /* Hacking the stack pointer - changing the base pointer return */
    printf("After change: %p\n", *seventhVariableAddress);


    printf("End StackSmashFunction()\n");
}

void SomeFunction(void)
{
    int array[7] = {1, 2, 3, 4, 5, 6, 7};

    printf("Begin SomeFunc()\n");

    StackSmashFunction(array[0], array[1], array[2], array[3], array[4], array[5], array[6]);

    printf("End SomeFunc()\n");
}

