#ifndef NM__MINIMUM_STACK_HPP
#define NM__MINIMUM_STACK_HPP


#include <cstddef> // size_t
#include <stack>
#include <deque>


namespace nm
{

namespace advcpp
{

// Concept of T: T must be copy-constructable, and be able to be used with std::less (to implement operator<())
template <typename T>
class MinimumStack
{
public:
    MinimumStack();
    MinimumStack(const MinimumStack& a_other);
    MinimumStack& operator=(const MinimumStack& a_other);
    ~MinimumStack() = default;

    void Push(const T& a_item);
    T Pop();
    const T& Top() const;
    const T& Minimum() const;
    size_t Size() const;
    bool IsEmpty() const;

private:
    std::deque<T> m_mainStack; // deque is 100% stable and it does not reallocate the items inside it (so the pointers from the m_minimumStack will always be valid!)
    std::stack<const T*> m_minimumStack;
};

} // advcpp

} // nm


#include "inl/minimum_stack.hxx"


#endif // NM__MINIMUM_STACK_HPP