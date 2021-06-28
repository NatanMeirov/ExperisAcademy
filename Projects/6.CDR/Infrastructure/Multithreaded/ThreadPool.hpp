/* A run time ThreadPool, using virtual interface (ICommand design pattern [implements Execute()]) as Task */

#ifndef __NM_THREADPOOL_HPP__
#define __NM_THREADPOOL_HPP__


#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include "Mutex.hpp"
#include "LockGuard.hpp"
#include "Thread.hpp"
#include "SafeQueue_Inline.hpp"
#include "ICommand.hpp"


namespace nm {

namespace runtime {

class ThreadPool {
public:
    ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue);
    ~ThreadPool();

    void PushTask(ICommand* a_task);
    // void PushTask(std::shared_ptr<ICommand> a_task);
    void StopExecution(); // Stop TP from working - Would cancel all the running Threads

    struct ThreadPoolSharedData {
        explicit ThreadPoolSharedData(const size_t a_workingQueueInitialSize) : m_tasksQueue(a_workingQueueInitialSize), m_lock(), m_isRequiredStopThreadPoolFromRunning(false) {}

        bool IsRequiredStopThreadPoolFromRunning() { nm::LockGuard guard(this->m_lock); return this->m_isRequiredStopThreadPoolFromRunning; }
        void SetIsRequiredStopThreadPoolFromRunning(const bool a_state) { nm::LockGuard guard(this->m_lock); this->m_isRequiredStopThreadPoolFromRunning = a_state; }

        // SafeQueue<std::shared_ptr<ICommand>> m_tasksQueue;
        SafeQueue<ICommand*> m_tasksQueue;
        nm::Mutex m_lock;
        bool m_isRequiredStopThreadPoolFromRunning;

    };

private:
    std::vector<Thread> m_workers;
    std::shared_ptr<ThreadPoolSharedData> m_threadPoolSharedData;
};

} // runtime

} // nm


#endif // __NM_THREADPOOL_HPP__