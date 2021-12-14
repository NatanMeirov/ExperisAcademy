#ifndef NM_SAFE_LOGGERS_MANAGER_HPP
#define NM_SAFE_LOGGERS_MANAGER_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map>
#include "safe_logger_decorator.hpp"


namespace smartbuilding
{

class SafeLoggersManager
{
public:
    SafeLoggersManager() = default;
    SafeLoggersManager(const SafeLoggersManager& a_other) = delete;
    SafeLoggersManager& operator=(const SafeLoggersManager& a_other) = delete;
    ~SafeLoggersManager() = default;

    std::shared_ptr<SafeLoggerDecorator> GetLogger(const std::string& a_logFileName); // Returns a default.log logger if a_logFileName is empty

private:
    std::unordered_map<std::string, std::shared_ptr<SafeLoggerDecorator>> m_loggersTable;
};

} // smartbuilding


#endif // NM_SAFE_LOGGERS_MANAGER_HPP
