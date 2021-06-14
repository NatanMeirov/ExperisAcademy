#include "LockGuard.hpp"
#include "Mutex.hpp"


nm::LockGuard::LockGuard(nm::Mutex& a_mutexToGuard)
: m_mutexToGuard(a_mutexToGuard){
    this->m_mutexToGuard.Lock();
}


nm::LockGuard::~LockGuard(){
    this->m_mutexToGuard.Unlock();
}