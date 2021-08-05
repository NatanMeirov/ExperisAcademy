#include "sensor_agent.hpp"
#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "location.hpp"
#include "idecoder.hpp"
#include "ipublisher.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


smartbuilding::SensorAgent::SensorAgent(std::shared_ptr<IDecoder> a_decoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location)
: SoftwareAgent(a_configurations, a_logger, a_remoteDeviceID, a_location)
, m_decoder(a_decoder)
{
}


void smartbuilding::SensorAgent::Publish(infra::TCPSocket::BytesBufferProxy a_bytesBuffer, std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueue)
{
    Event eventToPublish = m_decoder->Decode(a_bytesBuffer);
    a_publishedEventsQueue->Enqueue(eventToPublish);
}
