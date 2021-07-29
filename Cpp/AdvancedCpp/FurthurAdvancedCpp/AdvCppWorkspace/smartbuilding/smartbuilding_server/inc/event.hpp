#ifndef NM_EVENT_HPP
#define NM_EVENT_HPP


#include <string> // std::string
#include "date_time.hpp"
#include "event_location.hpp"
#include "tcp_socket.hpp"


namespace smartbuilding
{

// An extensible Event class - to represent each incoming event to the mediation server, from the sensors' agents
class Event
{
public:
    using EventType = std::string;
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
    ~Event() = default;

    DataPayload Data() const { return m_data; }
    DateTime Timestamp() const { return m_timestamp; }
    EventLocation Location() const { return m_location; }
    EventType Type() const { return m_type; }

private:
    DataPayload m_data;
    DateTime m_timestamp;
    EventLocation m_location;
    EventType m_type;
};

}


#endif // NM_EVENT_HPP