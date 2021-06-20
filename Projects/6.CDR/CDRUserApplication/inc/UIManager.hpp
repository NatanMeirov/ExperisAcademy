#ifndef __NM_CDR_UIMANAGER_HPP__
#define __NM_CDR_UIMANAGER_HPP__


#include <string>  // std::string
#include <ostream> // ostream
#include "RequestsHandler.hpp"
#include "IRequestBuilder.hpp"
#include "RequestBuilderParams.hpp"
#include "../../Infrastructure/JsonSerializer/json.hpp"


namespace nm {

namespace cdr {

class UIManager {
public:
    UIManager(const std::string& a_restApiServerIp, const unsigned int a_restApiServerPort);
    UIManager(const UIManager& a_other) = delete;
    UIManager& operator=(const UIManager& a_other) = delete;
    ~UIManager() = default;

    nlohmann::json SendMsisdnRequestAndGetResponse(const std::string& a_msisdn);
    nlohmann::json SendOperatorRequestAndGetResponse(const std::string& a_mcc, const std::string& a_mnc);
    nlohmann::json SendLinkRequestAndGetResponse(const std::string& a_firstParty, const std::string& a_secondParty);

private:
    RequestBuilderParams* CreateParamsOfRequest();
    nlohmann::json LaunchRequstAndReturnResponse(RequestBuilderParams* a_params);

    RequestsHandler m_requestsHandler;
    IRequestBuilder* m_requestBuilder; // Creates on demand and destroys periodically on finish
    std::string m_restApiServerIp; // Must have - to create a new builder every new request of the client
    unsigned int m_restApiServerPort;
};

} // cdr

} // nm


#endif // __NM_CDR_UIMANAGER_HPP__