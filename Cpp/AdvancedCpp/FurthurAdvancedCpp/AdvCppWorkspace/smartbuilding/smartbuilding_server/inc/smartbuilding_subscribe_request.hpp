#ifndef NM_SMARTBUILDING_SUBSCRIBE_REQUEST_HPP
#define NM_SMARTBUILDING_SUBSCRIBE_REQUEST_HPP


#include <vector> // std::vector
#include "smartbuilding_request.hpp"
#include "event.hpp"
#include "subscription_location.hpp"


namespace smartbuilding
{

class SmartBuildingSubscribeRequest : public SmartBuildingRequest
{
public:
    explicit SmartBuildingSubscribeRequest(std::string a_requestSenderID, const Event::EventType& a_interestedEventType, const SubscriptionLocation& a_subscriptionLocation)
    : SmartBuildingRequest(a_requestSenderID)
    , m_interestedEventType(a_interestedEventType)
    , m_subscriptionLocation(a_subscriptionLocation)
    {
    }

    virtual std::string RequestType() const override { return "Subscribe"; }

    Event::EventType InterestedEventType() const { return m_interestedEventType; }
    SubscriptionLocation SubscriptionLoc() const { return m_subscriptionLocation; }

private:
    Event::EventType m_interestedEventType;
    SubscriptionLocation m_subscriptionLocation;
};

} // smartbuilding


#endif // NM_SMARTBUILDING_SUBSCRIBE_REQUEST_HPP
