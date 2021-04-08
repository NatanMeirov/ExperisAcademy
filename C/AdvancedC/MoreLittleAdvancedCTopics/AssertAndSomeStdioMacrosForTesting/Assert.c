#include <stdio.h> /* NULL */
#include <assert.h> /* assert */

int main(void)
{
    int* ptr = NULL;
    assert(ptr); /* Its a macro */
    printf("After assertion\n");

    return 0;
}