#include "Thread.hpp"
#include <pthread.h> // pthread_create, pthread_join, pthread_exit
#include <stdexcept> // std::runtime_error


nm::Thread::Thread(TaskFunction a_taskToExecute, void* a_context)
: m_threadID()
, m_isAvailableThread(new bool(true)) {
    int statusCode = pthread_create(&this->m_threadID, NULL, a_taskToExecute, a_context);
    if(statusCode != 0) {
        *this->m_isAvailableThread = false;
		delete this->m_isAvailableThread;
        throw std::runtime_error("Failed while trying to create a Thread");
    }
}


nm::Thread::Thread(TaskFunction a_taskToExecute)
: Thread(a_taskToExecute, nullptr) {
}


nm::Thread::Thread(Thread&& a_rvalue) noexcept
: m_threadID(a_rvalue.m_threadID)
, m_isAvailableThread(a_rvalue.m_isAvailableThread) {
    a_rvalue.m_isAvailableThread = nullptr; // Move
}


nm::Thread& nm::Thread::Thread::operator=(Thread&& a_rvalue) noexcept {
    try {
        this->Cancel();
    }
    catch(...) { // No need to handle (error would be thrown ONLY if the thread has finished already)
    }
    delete this->m_isAvailableThread;

    this->m_threadID = a_rvalue.m_threadID;
    this->m_isAvailableThread = a_rvalue.m_isAvailableThread;
    a_rvalue.m_isAvailableThread = nullptr; // Move

    return *this;
}


nm::Thread::~Thread() {
    delete this->m_isAvailableThread;
}


void* nm::Thread::Join() {
    if(*this->m_isAvailableThread) {
        void* taskReturnedValue = nullptr;
        int statusCode = pthread_join(this->m_threadID, &taskReturnedValue);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to join");
        }

        *this->m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing

        return taskReturnedValue;
    }

    return nullptr;
}


void nm::Thread::Detach() {
    if(*this->m_isAvailableThread) {
        int statusCode = pthread_detach(this->m_threadID);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to detach");
        }

        *this->m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing
    }
}


void nm::Thread::Cancel() {
    int statusCode = pthread_cancel(this->m_threadID);
    if(statusCode != 0) {
        throw std::runtime_error("Failed while trying to cancel");
    }

    *this->m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing
}
