#include <vector> // std::vector
#include <algorithm> // std::is_sorted
#include <cassert> // assert

namespace advcpp
{

// Naive (O(n*log(n)))
// The input vector must be sorted
void MapNumbersToPowerOfTwoInplace(std::vector<int>& a_numbers)
{
    assert(std::is_sorted(a_numbers.begin(), a_numbers.end()));

    std::transform(a_numbers.begin(), a_numbers.end(), a_numbers.begin(), [](int n)
    {
        return n * n;
    });
    std::sort(a_numbers.begin(), a_numbers.end());
}

} // advcpp