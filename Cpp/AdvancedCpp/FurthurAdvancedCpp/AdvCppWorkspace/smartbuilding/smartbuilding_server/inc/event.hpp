#ifndef NM_EVENT_HPP
#define NM_EVENT_HPP


#include <string> // std::string
#include "date_time.hpp"
#include "location.hpp"
#include "tcp_socket.hpp"


namespace smartbuilding
{

// An extensible Event class - to represent each incoming event to the mediation server, from the HW sensors (after parsing the network data)
class Event
{
public:
    using EventType = std::string;
    using EventTimestamp = DateTime;
    using EventLocation = Location;
    using DataPayload = infra::TCPSocket::BytesBufferProxy;

    Event(const DataPayload& a_data, const DateTime& a_timestamp, const EventLocation& a_location, const EventType& a_type)
    : m_data(a_data)
    , m_timestamp(a_timestamp)
    , m_location(a_location)
    , m_type(a_type)
    {
    }
    Event(const Event& a_other) = default;
    Event& operator=(const Event& a_other) = default;
    Event(Event&& a_other) noexcept // Move semantics (performance)
    : m_data(std::move(a_other.m_data))
    , m_timestamp(a_other.m_timestamp)
    , m_location(a_other.m_location)
    , m_type(std::move(a_other.m_type))
    {
    }
    Event& operator=(Event&& a_other) noexcept // Move semantics (performance)
    {
        if(this != &a_other)
        {
            m_data = std::move(a_other.m_data);
            m_timestamp = a_other.m_timestamp;
            m_location = a_other.m_location;
            m_type = std::move(a_other.m_type);
        }

        return *this;
    }
    ~Event() = default;

    DataPayload Data() const { return m_data; }
    EventTimestamp Timestamp() const { return m_timestamp; }
    EventLocation Location() const { return m_location; }
    EventType Type() const { return m_type; }

private:
    DataPayload m_data;
    EventTimestamp m_timestamp;
    EventLocation m_location;
    EventType m_type;
};

}


#endif // NM_EVENT_HPP