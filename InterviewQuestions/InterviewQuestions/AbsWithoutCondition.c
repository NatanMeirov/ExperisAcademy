#include <stdio.h> /* printf */

#define BITS_IN_BYTE 8
#define BITS_IN_INT (sizeof(int) * BITS_IN_BYTE)

int AbsWithoutCondition(int _num)
{
    int musk = _num >> (BITS_IN_INT - 1); /* 000..000 or 111..111 */
    int result = (_num ^ musk);

    return result + (musk & 1);
}


int main(void)
{
    printf("%d\n", AbsWithoutCondition(-4)); /* -> 4 */
    printf("%d\n", AbsWithoutCondition(55)); /* -> 55 */
}