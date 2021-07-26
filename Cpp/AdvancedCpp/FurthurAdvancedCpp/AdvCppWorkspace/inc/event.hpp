#ifndef NM_EVENT_HPP
#define NM_EVENT_HPP


#include "date_time.hpp"
#include "event_location.hpp"
#include "tcp_socket.hpp"


namespace smartbuilding
{

// An extensible Event class - to represent each incoming event to the mediation server, from the sensors' agents
// Note: add more event types to the EventType enum, to extend the Event class
class Event
{
public:
    enum EventType { SMOKE, FIRE, EMPTY_ROOM };
    using DataPayload = infra::TCPSocket::BytesBufferProxy;

    Event();
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