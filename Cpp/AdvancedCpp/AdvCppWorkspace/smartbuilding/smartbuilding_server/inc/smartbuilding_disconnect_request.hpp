#ifndef NM_SMARTBUILDING_DISCONNECT_REQUEST_HPP
#define NM_SMARTBUILDING_DISCONNECT_REQUEST_HPP


#include "smartbuilding_request.hpp"


namespace smartbuilding
{

class SmartBuildingDisconnectRequest : public SmartBuildingRequest
{
public:
    explicit SmartBuildingDisconnectRequest(const std::string& a_requestSenderID) : SmartBuildingRequest(a_requestSenderID) {}

    virtual std::string RequestType() const override { return "Disconnect"; }
};

} // smartbuilding


#endif // NM_SMARTBUILDING_DISCONNECT_REQUEST_HPP
