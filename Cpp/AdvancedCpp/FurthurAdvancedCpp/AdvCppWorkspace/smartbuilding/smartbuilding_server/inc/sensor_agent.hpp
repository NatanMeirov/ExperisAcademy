#ifndef NM_SENSOR_AGENT_HPP
#define NM_SENSOR_AGENT_HPP


#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "location.hpp"
#include "idecoder.hpp"
#include "ipublisher.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace smartbuilding
{

// A concrete class of a software sensor agent, that the VENDOR of the related HW device should create and initialize
class SensorAgent : public SoftwareAgent, public IPublisher
{
public:
    SensorAgent(std::shared_ptr<IDecoder> a_decoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location);

    virtual void Publish(infra::TCPSocket::BytesBufferProxy a_bytesBuffer, std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueue) override;

private:
    std::shared_ptr<IDecoder> m_decoder;
};

} // smartbuilding


#endif // NM_SENSOR_AGENT_HPP