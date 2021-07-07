#ifndef NM_LOCK_GUARD_HPP
#define NM_LOCK_GUARD_HPP


#include <cstddef> // size_t
#include "mutex.hpp"


namespace advcpp
{

// A mutex's guard implementation - should be used by passing a ref to a Mutex object, and the Guard will provide unlocking when a destruction is occurred (RAII concept)
class LockGuard
{
public:
    LockGuard(Mutex& a_mutexToGuard);
    LockGuard(const LockGuard& a_other) = delete;
    LockGuard& operator=(const LockGuard& a_other) = delete;
    ~LockGuard();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

private:
    Mutex& m_mutexToGuard;
};

} // advcpp


#endif // NM_LOCK_GUARD_HPP