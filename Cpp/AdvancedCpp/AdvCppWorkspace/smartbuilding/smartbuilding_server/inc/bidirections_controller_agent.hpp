#ifndef NM_BIDIRECTIONS_CONTROLLER_AGENT_HPP
#define NM_BIDIRECTIONS_CONTROLLER_AGENT_HPP


#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "location.hpp"
#include "idecoder.hpp"
#include "iencoder.hpp"
#include "ipublisher.hpp"
#include "isubscriber.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace smartbuilding
{

// A concrete class of a software bidirections controller agent, that the VENDOR of the related HW device should create and initialize
class BidirectionsControllerAgent : public SoftwareAgent, public ISubscriber, public IPublisher
{
    BidirectionsControllerAgent(std::shared_ptr<IEncoder> a_encoder, std::shared_ptr<IDecoder> a_decoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location);

    virtual void Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue) override;
    virtual void Publish(infra::TCPSocket::BytesBufferProxy a_bytesBuffer, std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueue) override;

private:
    // Note: cannot aggregate or composit a controller agent and a sensor agent, because of the use of the inner ID of the controller agent's base class to reach to the related socket in the table
    std::shared_ptr<IEncoder> m_encoder;
    std::shared_ptr<IDecoder> m_decoder;
};

} // smartbuilding


#endif // NM_BIDIRECTIONS_CONTROLLER_AGENT_HPP
