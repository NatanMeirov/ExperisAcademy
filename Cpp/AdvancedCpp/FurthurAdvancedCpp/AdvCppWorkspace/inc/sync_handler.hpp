#ifndef NM_SYNC_HANDLER_HPP
#define NM_SYNC_HANDLER_HPP


#include <cstddef> // size_t
#include <exception> // std::exception_ptr
#include "atomic_value.hpp"


namespace advcpp
{

// Use Signal - to notify that the task has done (updates the flag)
// Use Check - to check if the task has done (checks the flag)
// Note: after a successful Check attempt - the flag will always be true (sync has done successfully)
// Warning: if an exception has been caught, Check would throw
class SyncHandler
{
public:
    SyncHandler();
    SyncHandler(const SyncHandler& a_other) = delete;
    SyncHandler& operator=(const SyncHandler& a_other) = delete;
    ~SyncHandler() = default;

    // Shared-Flag handling:
    void Signal();
    bool Check();

    // Exception handling:
    void SetException(std::exception_ptr a_exception);

private:
    void Rethrow();

private:
    std::exception_ptr m_caughtException;
    AtomicFlag m_hasDone;
};

} // advcpp


#endif // NM_SYNC_HANDLER_HPP