#ifndef NM_CONTROLLER_AGENT_HPP
#define NM_CONTROLLER_AGENT_HPP


#include <vector> // std::vector
#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "event.hpp"
#include "iencoder.hpp"
#include "isubscriber.hpp"
#include "subscription_location.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace smartbuilding
{

class ControllerAgent : public SoftwareAgent, public ISubscriber
{
public:
    ControllerAgent(const std::vector<std::string> a_intrestedTypesToHandle, const SubscriptionLocation& a_subscriptionLocation, std::shared_ptr<IEncoder> a_encoder, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_logFileName, const std::string& a_remoteDeviceID);

    SubscriptionLocation SubscriptionLoc() const { return m_subscriptionLocation; }
    const std::vector<std::string>& IntrestedTypesToHandle() const { return m_intrestedTypesToHandle; }

    virtual void Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<infra::TCPSocket::BytesBufferProxy, advcpp::NoOperationPolicy<infra::TCPSocket::BytesBufferProxy>>> a_handledBuffersQueue) override;

private:
    std::vector<std::string> m_intrestedTypesToHandle;
    SubscriptionLocation m_subscriptionLocation;
    std::shared_ptr<IEncoder> m_encoder;
};

} // smartbuilding


#endif // NM_CONTROLLER_AGENT_HPP
