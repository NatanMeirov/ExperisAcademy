#include "date_time.hpp"
#include <ctime> // struct tm
#include <string> // std::string, std::to_string


smartbuilding::DateTime::DateTime()
{
    struct tm* datetime;
    time_t timenow = time(NULL);
    datetime = localtime(&timenow);

    m_hours = datetime->tm_hour;
    m_minutes = datetime->tm_min;
    m_seconds = datetime->tm_sec;
    m_day = datetime->tm_mday;
    m_month = datetime->tm_mon;
    m_year = datetime->tm_year + OFFSET_OF_YEARS_IN_SYSTEM;
}


std::string smartbuilding::DateTime::ToString() const
{
    return std::to_string(m_hours) + ":" + std::to_string(m_minutes) + ":" + std::to_string(m_seconds)
            + "|" + std::to_string(m_day) + "." + std::to_string(m_month) + "." + std::to_string(m_year);
}
