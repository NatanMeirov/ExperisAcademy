#include "file_logger.hpp"
#include <string> // std::string
#include <fstream> // ofstream
#include "date_time.hpp"


smartbuilding::FileLogger::FileLogger(const std::string& a_logFileName)
: m_logLevelToStringMap(LogLevel::LAST_INDICATOR)
, m_logFileName(a_logFileName)
{
    m_logLevelToStringMap[LogLevel::ERROR] = "[Error] ";
    m_logLevelToStringMap[LogLevel::WARNING] = "[Warning] ";
    m_logLevelToStringMap[LogLevel::INFO] = "[Info] ";
}


void smartbuilding::FileLogger::Log(const std::string &a_message, LogLevel a_logLevel)
{
    // Open the file resource only on demand (and not keeping it open when no using it [the file])
    std::ofstream log(m_logFileName);
    DateTime timeNowSnapshot;

    log << timeNowSnapshot.ToString() + MapLogLevelToString(a_logLevel) + a_message;
}


std::string smartbuilding::FileLogger::MapLogLevelToString(LogLevel a_logLevel) const
{
    return m_logLevelToStringMap.at(a_logLevel); // Would throw if LogLevel is not valid
}
