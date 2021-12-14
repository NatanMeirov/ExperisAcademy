#ifndef NM_SAFE_LOGGER_DECORATOR_HPP
#define NM_SAFE_LOGGER_DECORATOR_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <mutex> // std::mutex, std::lock_guard
#include "ilogger.hpp"


namespace smartbuilding
{

class SafeLoggerDecorator : public ILogger
{
public:
    explicit SafeLoggerDecorator(std::shared_ptr<ILogger> a_loggerToDecorate) : m_decoratedLogger(a_loggerToDecorate) {}

    virtual void Log(const std::string& a_message, ILogger::LogLevel a_logLevel) override
    {
        // Multithreaded safe write operation (to handle the writing to a shared resource)
        std::lock_guard<std::mutex> guard(m_lock);
        m_decoratedLogger->Log(a_message, a_logLevel);
    }

private:
    std::shared_ptr<ILogger> m_decoratedLogger;
    std::mutex m_lock;
};

} // smartbuilding


#endif // NM_SAFE_LOGGER_DECORATOR_HPP