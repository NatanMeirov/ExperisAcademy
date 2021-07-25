#include <cstddef> // size_t
#include <cassert> // assert
#include <algorithm> // std::is_sorted


void RotateArrayRight(int* a_array, size_t a_arraySize, size_t a_rotates)
{
    assert(a_array);
    assert(a_arraySize != 0);
    assert(std::is_sorted(a_array, a_array + a_arraySize));

    if ((a_rotates % a_arraySize) == 0 /* Array would stay the same */
        || a_arraySize == 1 /* No need to rotate 1 item */)
    {
        return;
    }

    int prevVal = a_array[0];
    for (size_t iterations = 0, j = a_rotates % a_arraySize; iterations < a_arraySize; ++iterations, j = (j + a_rotates) % a_arraySize)
    {
        int nextVal = a_array[j];
        a_array[j] = prevVal;
        prevVal = nextVal;
    }
}

// O(2n) solution WITHOUT EXPENSIVE MODULU: reverse the array one time, and then do 2 more reverses -
// (from 0 [begin] to rotate_val && rotate_val + 1 to size - 1 [end])