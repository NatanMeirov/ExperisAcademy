#ifndef NM_SMARTBUILDING_EVENT_REQUEST_HPP
#define NM_SMARTBUILDING_EVENT_REQUEST_HPP


#include "smartbuilding_request.hpp"
#include "tcp_socket.hpp"


namespace smartbuilding
{

class SmartBuildingEventRequest : public SmartBuildingRequest
{
public:
    explicit SmartBuildingEventRequest(std::string a_requestSenderID, infra::TCPSocket::BytesBufferProxy a_eventDataBuffer) : SmartBuildingRequest(a_requestSenderID), m_eventDataBuffer(a_eventDataBuffer) {}

    virtual std::string RequestType() const override { return "Event"; }

    infra::TCPSocket::BytesBufferProxy EventDataBuffer() const { return m_eventDataBuffer; }

private:
    infra::TCPSocket::BytesBufferProxy m_eventDataBuffer;
};

} // smartbuilding


#endif // NM_SMARTBUILDING_EVENT_REQUEST_HPP
