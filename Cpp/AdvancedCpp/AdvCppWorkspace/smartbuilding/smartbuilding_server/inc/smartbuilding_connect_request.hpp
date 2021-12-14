#ifndef NM_SMARTBUILDING_CONNECT_REQUEST_HPP
#define NM_SMARTBUILDING_CONNECT_REQUEST_HPP


#include "smartbuilding_request.hpp"


namespace smartbuilding
{

class SmartBuildingConnectRequest : public SmartBuildingRequest
{
public:
    explicit SmartBuildingConnectRequest(const std::string& a_requestSenderID) : SmartBuildingRequest(a_requestSenderID) {}

    virtual std::string RequestType() const override { return "Connect"; }
};

} // smartbuilding


#endif // NM_SMARTBUILDING_CONNECT_REQUEST_HPP
