/* A compile time (template) ThreadPool, using Functos as Tasks */

#ifndef __NM_TTHREADPOOL_HPP__
#define __NM_TTHREADPOOL_HPP__


#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include "Thread.hpp"
#include "SafeQueue_Inline.hpp"


namespace nm {

namespace compiletime {

// The TaskFunctor is NOT generic - the task would be always the same, and only the work param (T) is changing al the time
// The TaskFunctor should contain the WHOLE DATA required to its execution - to be passed into the working threads (as their context), and the TaskFunctor should get T paramether for its execution!
// T Concept: MUST be copy-constructable
template <typename T, typename TaskFunctor>
class ThreadPool {
public:
    ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue, TaskFunctor a_task);
    ~ThreadPool();

    void PushWork(const T& a_work);
    void StopWork(); // Stop TP from working - Would cancel all the running Threads

    struct ThreadPoolSharedData {
        ThreadPoolSharedData(const size_t a_workingQueueInitialSize, TaskFunctor a_task) : m_worksQueue(a_workingQueueInitialSize), m_task(a_task), m_isRequiredStopThreadPoolFromRunning(false) {}

        TaskFunctor m_task;
        SafeQueue<T> m_worksQueue;
        bool m_isRequiredStopThreadPoolFromRunning;
    };

private:
    std::vector<Thread> m_workers;
    std::shared_ptr<ThreadPoolSharedData> m_threadPoolSharedData;
};


// ThreadPool Inline:

template <typename T, typename TaskFunctor>
inline static void* ThreadPoolWorkingProcess(void* a_context) {
    typename ThreadPool<T,TaskFunctor>::ThreadPoolSharedData* globalThreadPoolData = static_cast<typename ThreadPool<T,TaskFunctor>::ThreadPoolSharedData*>(a_context);

    while(!globalThreadPoolData->m_isRequiredStopThreadPoolFromRunning || !globalThreadPoolData->m_worksQueue.IsEmpty()) {
        T work = globalThreadPoolData->m_worksQueue.Dequeue();
        globalThreadPoolData->m_task(work); // Task Execution on work
    }

    return nullptr;
}


template <typename T, typename TaskFunctor>
ThreadPool<T, TaskFunctor>::ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue, TaskFunctor a_task)
: m_workers()
, m_threadPoolSharedData(new ThreadPoolSharedData(a_sizeOfWorkingQueue, a_task)) {
    for(size_t i = 0; i < a_workingThreadsNumber; ++i) {
        this->m_workers.push_back(std::move(nm::Thread(&ThreadPoolWorkingProcess<T,TaskFunctor>, static_cast<void*>(this->m_threadPoolSharedData.get()))));
    }
}


template <typename T, typename TaskFunctor>
ThreadPool<T,TaskFunctor>::~ThreadPool() {
    if(!this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning) {
        this->StopWork();
    }
    // Else - already stopped
}



template <typename T, typename TaskFunctor>
void ThreadPool<T,TaskFunctor>::PushWork(const T& a_work) {
    if(!this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning) {
        this->m_threadPoolSharedData->m_worksQueue.Enqueue(a_work);
    }
}


template <typename T, typename TaskFunctor>
void ThreadPool<T,TaskFunctor>::StopWork() {
    this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning = true;
    while(!this->m_threadPoolSharedData->m_worksQueue.IsEmpty()); // A polling loop to check if the safe queue is empty

    for(size_t i = 0; i < this->m_workers.size(); ++i) { // A "cleaning" loop (to ensure that all the threads has finished and did not block)
        try {
            this->m_workers.at(i).Cancel();
        }
        catch(const std::exception& ex) {
            // No-Op - it is a valid situation, because the thread may already has finished its running
        }
    }
}

} // compiletime

} // nm


#endif // __NM_TTHREADPOOL_HPP__