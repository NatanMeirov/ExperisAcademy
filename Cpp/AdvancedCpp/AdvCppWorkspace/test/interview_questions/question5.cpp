#include <vector> // std::vector
#include <cassert> // assert


void ReplaceElmByClosestGreaterFromRightInplace(std::vector<int>& a_numbers)
{
    if(a_numbers.size() == 0)
    {
        return;
    }

    std::vector<int> closestGreaterNumbersStack;
    size_t stackHead = 0;

    size_t remainingInterval = a_numbers.size();
    size_t currentIndex = remainingInterval - 1;

    closestGreaterNumbersStack.push_back(a_numbers[currentIndex]);
    ++stackHead;

    a_numbers[currentIndex] = -1;

    // TODO - go from right to left, push to stack dynamically
    while(--remainingInterval > 0)
    {
        --currentIndex;

        if(a_numbers[currentIndex] > closestGreaterNumbersStack[stackHead])
        {
            
        }
    }
}