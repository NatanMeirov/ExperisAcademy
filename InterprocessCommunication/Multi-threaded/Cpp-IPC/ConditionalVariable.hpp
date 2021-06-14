#ifndef __NM_CONDITIONALVARIABLE_HPP__
#define __NM_CONDITIONALVARIABLE_HPP__


#include <cstddef> // size_t
#include <pthread.h>
#include "Mutex.hpp"


namespace nm {

class ConditionalVariable {
public:
    ConditionalVariable();
    ConditionalVariable(const ConditionalVariable& a_other) = default;
    ConditionalVariable& operator=(const ConditionalVariable& a_other) = default;
    ~ConditionalVariable();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    void Wait(nm::Mutex& a_mutex);
    void Signal();

private:
    pthread_cond_t m_conditionalVariable;
    bool m_isAvailableCondVar;
};

} // nm


#endif // __NM_CONDITIONALVARIABLE_HPP__