#ifndef NM_THREAD_GROUP_HPP
#define NM_THREAD_GROUP_HPP


#include <cstddef> // size_t
#include <list>
#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "thread.hpp"


namespace advcpp
{

/**
 * Concept of DestructionPolicy: policy must be copy-constructable
 * The destruction policy is a FUNCTOR (implements operator() and get 1 param: Thread& obj), to be used as an instructions to know which action the Thread
 * object should call on itself when it is in a destruction stage
 * Warning: The given ICallable task will be SHARED in all the N threads - make sure that the given task is thread-safe!
 */
template <typename DestructionPolicy>
class ThreadGroup
{
public:
    ThreadGroup(std::shared_ptr<ICallable> a_commonTask, size_t a_threadsCount, DestructionPolicy a_destructionPolicy);
    ThreadGroup(const ThreadGroup& a_other) = delete;
    ThreadGroup& operator=(const ThreadGroup& a_other) = delete;
    ~ThreadGroup() = default; // Each Thread handles its destruction, specified by DestructionPolicy

    void Join();
    void Detach();
    void Cancel(bool a_ensureCompleteCancelation = false);

    void Add(size_t a_threadsToAdd);
    void Remove(size_t a_threadsToRemove);

    size_t Size() const;

private:
    size_t CleanDoneThreads();
    void AddThreads(size_t a_threadsToAdd);
    void KillThreads(size_t a_threadsToKill);

private:
    std::list<std::shared_ptr<Thread<DestructionPolicy>>> m_threadsGroup;
    std::shared_ptr<ICallable> m_commonTask;
    DestructionPolicy m_destructionPolicy;
};

} // advcpp


#include "inl/thread_group.hxx"


#endif // NM_THREAD_GROUP_HPP