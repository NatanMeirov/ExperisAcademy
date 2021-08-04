#include "safe_loggers_manager.hpp"
#include <string> // std::string
#include <memory> // std::shared_ptr, std::make_shared
#include <unordered_map>
#include "file_logger.hpp"
#include "safe_logger_decorator.hpp"


namespace smartbuilding
{

std::shared_ptr<SafeLoggerDecorator> SafeLoggersManager::GetLogger(const std::string& a_logFileName)
{
    std::string logName;
    if(a_logFileName == "")
    {
        logName = "default.log";
    }
    else
    {
        logName = a_logFileName;
    }

    if(m_loggersTable.find(logName) == m_loggersTable.end()) // Key has not found
    {
        // Create new logger (new map entry)
        m_loggersTable[logName] = std::shared_ptr<SafeLoggerDecorator>(new SafeLoggerDecorator(std::shared_ptr<FileLogger>(new FileLogger(logName))));
    }

    return m_loggersTable[logName];
}

} // smartbuilding
