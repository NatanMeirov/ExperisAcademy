#include "lock_guard.hpp"
#include "mutex.hpp"


advcpp::LockGuard::LockGuard(Mutex& a_mutexToGuard)
: m_mutexToGuard(a_mutexToGuard)
{
    m_mutexToGuard.Lock();
}


advcpp::LockGuard::~LockGuard()
{
    m_mutexToGuard.Unlock();
}