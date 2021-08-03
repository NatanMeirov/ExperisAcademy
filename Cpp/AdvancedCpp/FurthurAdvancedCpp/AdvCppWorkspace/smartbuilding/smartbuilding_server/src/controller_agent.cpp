#include "controller_agent.hpp"
#include <vector> // std::vector
#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "event.hpp"
#include "isubscriber.hpp"
#include "subscription_location.hpp"
#include "smartbuilding_protocol_parser.hpp"
#include "tcp_socket.hpp"


smartbuilding::ControllerAgent::ControllerAgent(const std::vector<std::string> a_intrestedTypesToHandle, const SubscriptionLocation& a_subscriptionLocation, std::shared_ptr<IEncoder> a_encoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_logFileName, const std::string& a_remoteDeviceID)
: SoftwareAgent(a_configurations, a_logger, a_logFileName, a_remoteDeviceID)
, m_intrestedTypesToHandle(a_intrestedTypesToHandle)
, m_subscriptionLocation(a_subscriptionLocation)
, m_encoder(a_encoder)
{
}


void smartbuilding::ControllerAgent::Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<infra::TCPSocket::BytesBufferProxy, advcpp::NoOperationPolicy<infra::TCPSocket::BytesBufferProxy>>> a_handledBuffersQueue)
{
    infra::TCPSocket::BytesBufferProxy bytesBufferToHandle = m_encoder->Encode(a_event);
    a_handledBuffersQueue->Enqueue(bytesBufferToHandle);
}
