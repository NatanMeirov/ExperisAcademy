#ifndef NM_SMARTBUILDING_UNSUBSCRIBE_REQUEST_HPP
#define NM_SMARTBUILDING_UNSUBSCRIBE_REQUEST_HPP


#include <vector> // std::vector
#include "smartbuilding_request.hpp"
#include "event.hpp"
#include "subscription_location.hpp"


namespace smartbuilding
{

class SmartBuildingUnsubscribeRequest : public SmartBuildingRequest
{
public:
    SmartBuildingUnsubscribeRequest(const std::string& a_requestSenderID, const Event::EventType& a_subscribedEventType)
    : SmartBuildingRequest(a_requestSenderID)
    , m_subscribedEventType(a_subscribedEventType)
    {
    }

    virtual std::string RequestType() const override { return "Unsubscribe"; }

    Event::EventType SubscribedEventType() const { return m_subscribedEventType; }

private:
    Event::EventType m_subscribedEventType;
};

} // smartbuilding


#endif // NM_SMARTBUILDING_UNSUBSCRIBE_REQUEST_HPP
