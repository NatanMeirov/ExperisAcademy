#ifndef NM_MINIMUM_STACK_HXX
#define NM_MINIMUM_STACK_HXX


#include <cstddef> // size_t
#include <stdexcept> // std::underflow_error
#include <functional> // std::less

#include "../minimum_stack.hpp"
namespace nm
{

namespace advcpp
{

template <typename T>
MinimumStack<T>::MinimumStack()
: m_mainStack()
, m_minimumStack()
{
}


template <typename T>
MinimumStack<T>::MinimumStack(const MinimumStack<T>& a_other)
: m_mainStack(a_other.m_mainStack) // std::deque and std::stack are copy-constructable
, m_minimumStack(a_other.m_minimumStack)
{
}


template <typename T>
MinimumStack<T>& MinimumStack<T>::operator=(const MinimumStack<T>& a_other)
{
    if(this != &a_other) // Saving from self-assignment
    {
        this->m_mainStack = a_other.m_mainStack;
        this->m_minimumStack = a_other.m_minimumStack;
    }

    return *this;
}


template <typename T>
void MinimumStack<T>::Push(const T& a_item)
{
    this->m_mainStack.push_back(a_item);
    T& newItemRef = this->m_mainStack.back();

    if(this->m_minimumStack.empty())
    {
        this->m_minimumStack.push(&newItemRef);
    }
    else // minimum stack is not empty
    {
        T* currentMinimumPtr = this->m_minimumStack.top();

        if(std::less<T>()(newItemRef, *currentMinimumPtr))
        // || (!std::less<T>()(a_item, currentMinimum) && !std::less<T>()(currentMinimum, a_item))) // Good for copies
        {
            this->m_minimumStack.push(&newItemRef);
        }
    }
}


template <typename T>
T MinimumStack<T>::Pop()
{
    if(this->m_mainStack.empty()) {
        throw std::underflow_error("MinimumStack is empty");
    }

    T& value = this->m_mainStack.back();
    this->m_mainStack.pop_back();

    T* currentMinimumAddress = this->m_minimumStack.top();

    // if(!std::less<T>()(returnValue, currentMinimum) && !std::less<T>()(currentMinimum, returnValue)) // Good for copies
    if(&value == currentMinimumAddress)
    {
        this->m_minimumStack.pop();
    }

    return value;
}


template <typename T>
const T& MinimumStack<T>::Top() const
{
    if(this->m_mainStack.empty()) {
        throw std::underflow_error("MinimumStack is empty");
    }

    return this->m_mainStack.back();
}


template <typename T>
const T& MinimumStack<T>::Minimum() const
{
    if(this->m_mainStack.empty()) {
        throw std::underflow_error("MinimumStack is empty");
    }

    return *this->m_minimumStack.top();
}


template <typename T>
size_t MinimumStack<T>::Size() const
{
    return this->m_mainStack.size();
}


template <typename T>
bool MinimumStack<T>::IsEmpty() const
{
    return this->m_mainStack.empty();
}

} // advcpp

} // nm


#endif // NM_MINIMUM_STACK_HXX
