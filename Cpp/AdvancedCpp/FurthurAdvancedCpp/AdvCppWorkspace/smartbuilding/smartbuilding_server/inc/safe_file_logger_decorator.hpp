#ifndef NM_SAFE_FILE_LOGGER_DECORATOR_HPP
#define NM_SAFE_FILE_LOGGER_DECORATOR_HPP


#include <string> // std::string
#include <mutex> // std::mutex, std::lock_guard
#include "ilogger.hpp"
#include "file_logger.hpp"


namespace smartbuilding
{

class SafeFileLoggerDecorator : public ILogger
{
public:
    explicit SafeFileLoggerDecorator(const std::string& a_logFileName) : m_decoratedFileLogger(a_logFileName) {}

    virtual void Log(const std::string& a_message, ILogger::LogLevel a_logLevel) override
    {
        // Multithreaded safe operation (to handle the WRITING to a shared file [resource])
        std::lock_guard<std::mutex> guard(m_lock);
        m_decoratedFileLogger.Log(a_message, a_logLevel);
    }

private:
    FileLogger m_decoratedFileLogger;
    std::mutex m_lock;
};

} // smartbuilding


#endif // NM_SAFE_FILE_LOGGER_DECORATOR_HPP