#include "sync_handler.hpp"
#include <exception> // std::exception_ptr, std::rethrow_exception
#include "atomic_value.hpp"


advcpp::SyncHandler::SyncHandler()
: m_caughtException()
, m_hasDone(false)
{
}


void advcpp::SyncHandler::Signal()
{
    m_hasDone.True();
}


bool advcpp::SyncHandler::Check() const
{
    Rethrow();
    return m_hasDone.Check();
}


void advcpp::SyncHandler::SetException(std::exception_ptr a_exception)
{
    m_caughtException = a_exception;
}


void advcpp::SyncHandler::Rethrow() const
{
    if(m_caughtException)
    {
        std::rethrow_exception(m_caughtException);
    }
}
