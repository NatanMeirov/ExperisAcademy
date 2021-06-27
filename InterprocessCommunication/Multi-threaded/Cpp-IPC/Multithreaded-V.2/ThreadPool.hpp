/* A run time ThreadPool, using virtual interface (ICommand design pattern [implements Execute()]) as Task */

#ifndef __NM_THREADPOOL_HPP__
#define __NM_THREADPOOL_HPP__


#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include "Thread.hpp"
#include "SafeQueue_Inline.hpp"
#include "ICommand.hpp"


namespace nm {

namespace runtime {

// Note: every ICommand task is stored as a std::shared_ptr, so after its execution - if there where no additional references - the memory would be free automatically (RAII)
class ThreadPool {
public:
    ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue);
    ~ThreadPool();

    void PushTask(ICommand* a_task); // The pointer will be wrapped in a std::shared_ptr, so no delete is required (iif this ICommand* should still be "alive" - make sure to create std::Shared_ptr first and then call the PushTask function)
    void PushTask(std::shared_ptr<ICommand> a_task);
    void StopExecution(); // Stop TP from working - Would cancel all the running Threads

    struct ThreadPoolSharedData {
        explicit ThreadPoolSharedData(const size_t a_workingQueueInitialSize) : m_tasksQueue(a_workingQueueInitialSize), m_isRequiredStopThreadPoolFromRunning(false) {}

        SafeQueue<std::shared_ptr<ICommand>> m_tasksQueue;
        bool m_isRequiredStopThreadPoolFromRunning;
    };

private:
    std::vector<Thread> m_workers;
    std::shared_ptr<ThreadPoolSharedData> m_threadPoolSharedData;
};

} // runtime

} // nm


#endif // __NM_THREADPOOL_HPP__