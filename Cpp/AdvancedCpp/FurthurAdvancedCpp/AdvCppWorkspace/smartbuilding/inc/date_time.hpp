#ifndef NM_DATE_TIME_HPP
#define NM_DATE_TIME_HPP


#include <string> // std::string, std::to_string


namespace smartbuilding
{

// A class to represent the current Time Stamp of its own creating time (Now)
// Each instance "takes a snapshot" of the current timestamp (position) [local time]
class DateTime
{
public:
    DateTime();
    DateTime(const DateTime& a_other) = default;
    DateTime& operator=(const DateTime& a_other) = default;
    ~DateTime() = default;

    int Hours() const { return m_hours; }
    int Minutes() const { return m_minutes; }
    int Seconds() const { return m_seconds; }
    int Day() const { return m_day; }
    int Month() const { return m_month; }
    int Year() const { return m_year; }
    std::string ToString() const;

private:
    static const unsigned int OFFSET_OF_YEARS_IN_SYSTEM = 1900;

private:
    int m_hours;
    int m_minutes;
    int m_seconds;
    int m_day;
    int m_month;
    int m_year;
};

} // smartbuilding


#endif // NM_DATE_TIME_HPP