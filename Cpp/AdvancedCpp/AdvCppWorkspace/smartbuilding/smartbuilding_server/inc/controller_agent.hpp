#ifndef NM_CONTROLLER_AGENT_HPP
#define NM_CONTROLLER_AGENT_HPP


#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "location.hpp"
#include "event.hpp"
#include "iencoder.hpp"
#include "isubscriber.hpp"
#include "subscription_location.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace smartbuilding
{

// A concrete or base class of a software controller agent, that the VENDOR of the related HW device should create and initialize
class ControllerAgent : public SoftwareAgent, public ISubscriber
{
public:
    ControllerAgent(std::shared_ptr<IEncoder> a_encoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location);

    virtual void Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue) override;

private:
    std::shared_ptr<IEncoder> m_encoder;
};

} // smartbuilding


#endif // NM_CONTROLLER_AGENT_HPP
