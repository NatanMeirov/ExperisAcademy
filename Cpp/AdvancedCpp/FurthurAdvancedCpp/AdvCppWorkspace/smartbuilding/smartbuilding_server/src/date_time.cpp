#include "date_time.hpp"
#include <ctime> // struct tm
#include <string> // std::string, std::to_string


smartbuilding::DateTime::DateTime(int a_hours, int a_minutes, int a_seconds, int a_day, int a_month, int a_year)
: m_hours(a_hours)
, m_minutes(a_minutes)
, m_seconds(a_seconds)
, m_day(a_day)
, m_month(a_month)
, m_year(a_year)
{
}


smartbuilding::DateTime smartbuilding::DateTime::Now()
{
    struct tm* datetime;
    time_t timenow = time(NULL);
    datetime = localtime(&timenow);

    return DateTime(datetime->tm_hour, datetime->tm_min, datetime->tm_sec, datetime->tm_mday, datetime->tm_mon, datetime->tm_year + OFFSET_OF_YEARS_IN_SYSTEM);
}


std::string smartbuilding::DateTime::ToString() const
{
    return TimeToString() + "|" + DateToString();
}


std::string smartbuilding::DateTime::TimeToString() const
{
    return std::to_string(m_hours) + ":" + std::to_string(m_minutes) + ":" + std::to_string(m_seconds);
}


std::string smartbuilding::DateTime::DateToString() const
{
    return std::to_string(m_day) + "." + std::to_string(m_month) + "." + std::to_string(m_year);
}
