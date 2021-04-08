#include <stdio.h> /* printf */

/* packing () - tells the compiler NOT TO ALIGN THE BYTES OF A STRUCT.
Every data transfer to the CPU is 8 bytes (== 64 bits) in one time (single "operation"), so the compiler
does an alignment to let the CPU to "read" everything that transfers to its BUS (== stream) in one time 
and not in 2 calls... => better performance! */

struct SomeStruct
{
    char ch1; /* 1 byte ---> + 7 bytes of alignment */
    double dNum; /* 8 bytes */
    char ch2; /* 1 byte ---> + 7 bytes of alignment */
};

/* less performance... but will be size of == 10! (Generally should be used in Embedded Systems because HardWare usually sends data on fixed bus without alignment...) */
#pragma pack (push, 1)
struct PackedStruct
{
    char ch1;
    double dNum;
    char ch2;
};
#pragma pack (pop)


int main(void)
{
    printf("%ld\n", sizeof(struct SomeStruct));
    printf("%ld\n", sizeof(struct PackedStruct));

    return 0;
}