#include "bidirections_controller_agent.hpp"
#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "location.hpp"
#include "idecoder.hpp"
#include "iencoder.hpp"
#include "ipublisher.hpp"
#include "isubscriber.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


smartbuilding::BidirectionsControllerAgent::BidirectionsControllerAgent(std::shared_ptr<IEncoder> a_encoder, std::shared_ptr<IDecoder> a_decoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location)
: SoftwareAgent(a_configurations, a_logger, a_remoteDeviceID, a_location)
, m_encoder(a_encoder)
, m_decoder(a_decoder)
{
}


void smartbuilding::BidirectionsControllerAgent::Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue)
{
    infra::TCPSocket::BytesBufferProxy bytesBufferToHandle = m_encoder->Encode(a_event);
    a_handledBuffersQueue->Enqueue(std::make_pair(RemoteDeviceID(), bytesBufferToHandle));
    // Use the logger
}


void smartbuilding::BidirectionsControllerAgent::Publish(infra::TCPSocket::BytesBufferProxy a_bytesBuffer, std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueue)
{
    Event eventToPublish = m_decoder->Decode(a_bytesBuffer);
    a_publishedEventsQueue->Enqueue(eventToPublish);
    // Use the logger
}
