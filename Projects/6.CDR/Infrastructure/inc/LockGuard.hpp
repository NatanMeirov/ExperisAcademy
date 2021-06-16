#ifndef __NM_LOCKGUARD_HPP__
#define __NM_LOCKGUARD_HPP__


#include <cstddef> // size_t
#include "Mutex.hpp"


namespace nm {

// A mutex's guard implementation - should be used by passing a ref to a Mutex object, and the Guard will provide unlocking when a destruction is occurred (RAII concept)
class LockGuard {
public:
    LockGuard(nm::Mutex& a_mutexToGuard);
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
    nm::Mutex& m_mutexToGuard;
};

} // nm


#endif // __NM_LOCKGUARD_HPP__