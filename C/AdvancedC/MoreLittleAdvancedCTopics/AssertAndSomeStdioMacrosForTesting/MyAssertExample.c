#include <stdio.h> /* printf */
#include <stdlib.h> /* abort */

/* Assertion */
#define ASSERT(X) (X) ? 0 : (fprintf(stderr, "Assertion Failed in expression: (" #X "), in file: " __FILE__ \
", on line %d, created on " __DATE__\
"...\n", __LINE__));\
abort()\

/* Reg format: */
#define ASSERT1(X) (X) ? 0 : (fprintf(stderr, "Assertion Failed in expression: (%s), in file: %s, on line %d, created on %s...\n", #X, __FILE__, __LINE__, __DATE__));\
abort()\


int main(void)
{
    int* ptr = NULL;
    ASSERT(ptr);
    printf("After assert\n"); /* Should not be printed at all */

    return 0;
}