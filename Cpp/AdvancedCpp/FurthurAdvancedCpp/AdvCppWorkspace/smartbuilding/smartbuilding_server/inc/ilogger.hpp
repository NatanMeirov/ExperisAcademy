#ifndef NM_ILOGGER_HPP
#define NM_ILOGGER_HPP


#include <string> // std::string


namespace smartbuilding
{

// An interface of logger class
class ILogger
{
public:
    enum LogLevel { ERROR, WARNING, INFO, LAST_INDICATOR };

    virtual ~ILogger() = default;
    virtual void Log(const std::string& a_message, LogLevel a_logLevel);
};

} // smartbuilding


#endif // NM_ILOGGER_HPP