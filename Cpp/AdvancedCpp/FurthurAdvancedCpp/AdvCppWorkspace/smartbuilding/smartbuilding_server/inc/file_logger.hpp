#ifndef NM_FILE_LOGGER_HPP
#define NM_FILE_LOGGER_HPP


#include <string> // std::string
#include <vector> // std::vector
#include "ilogger.hpp"


namespace smartbuilding
{

class FileLogger : public ILogger
{
public:
    explicit FileLogger(const std::string& a_logFileName);
    FileLogger(const FileLogger& a_other) = default;
    FileLogger& operator=(const FileLogger& a_other) = default;
    ~FileLogger() = default;

    virtual void Log(const std::string& a_message, LogLevel a_logLevel) override; // Throws if LogLevel is not valid

private:
    std::string MapLogLevelToString(LogLevel a_logLevel) const;

private:
    std::vector<std::string> m_logLevelToStringMap;
    std::string m_logFileName;
};

} // smartbuilding


#endif // NM_FILE_LOGGER_HPP