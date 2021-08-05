#include "controller_agent.hpp"
#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "event.hpp"
#include "isubscriber.hpp"
#include "subscription_location.hpp"
#include "tcp_socket.hpp"


smartbuilding::ControllerAgent::ControllerAgent(std::shared_ptr<IEncoder> a_encoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location)
: SoftwareAgent(a_configurations, a_logger, a_remoteDeviceID, a_location)
, m_encoder(a_encoder)
{
}


void smartbuilding::ControllerAgent::Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue)
{
    infra::TCPSocket::BytesBufferProxy bytesBufferToHandle = m_encoder->Encode(a_event);
    a_handledBuffersQueue->Enqueue(std::make_pair(RemoteDeviceID(), bytesBufferToHandle));
}
