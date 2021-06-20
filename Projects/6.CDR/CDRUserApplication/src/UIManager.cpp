#include "../inc/UIManager.hpp"
#include "../inc/RequestBuilderFactory.hpp"
#include "../inc/UrlBuilderParams.hpp"
#include "../inc/RoutingParams.hpp"
#include "../inc/RoutingStrategy.hpp"
#include "../../Infrastructure/JsonSerializer/json.hpp"


nm::cdr::UIManager::UIManager(const std::string& a_restApiServerIp, const unsigned int a_restApiServerPort)
: m_requestsHandler(a_restApiServerIp, a_restApiServerPort)
, m_requestBuilder(nullptr)
, m_restApiServerIp(a_restApiServerIp)
, m_restApiServerPort(a_restApiServerPort) {
}


nm::cdr::RequestBuilderParams* nm::cdr::UIManager::CreateParamsOfRequest() {
    UrlBuilderParams* params = new UrlBuilderParams();
    params->m_destServerIpAddress = this->m_restApiServerIp;
    params->m_destServerPortNumber = this->m_restApiServerPort;
    params->m_isSecuredHTTP = false;

    return params;
}


nlohmann::json nm::cdr::UIManager::LaunchRequstAndReturnResponse(RequestBuilderParams* a_params) {
    // Create Builder
    this->m_requestBuilder = RequestBuilderFactory::CreateRequestBuilder(a_params);

    // Build request
    std::string request = this->m_requestBuilder->BuildRequest();

    // Send Request
    this->m_requestsHandler.Request(request);

    // Get Response
    nlohmann::json j = this->m_requestsHandler.Response();

    // Clean
    delete static_cast<UrlBuilderParams*>(a_params)->m_routingParams; // FIXME: Change the pointers to smart pointer to support RAII (the current implementation is NOT exception safe)
    delete a_params;
    delete this->m_requestBuilder;

    return j;
}


nlohmann::json nm::cdr::UIManager::SendMsisdnRequestAndGetResponse(const std::string& a_msisdn) {
    // Create Builder's params
    UrlBuilderParams* params = static_cast<UrlBuilderParams*>(CreateParamsOfRequest());
    params->m_mainRouteOption = "msisdn";
    params->m_routingParams = new RoutingParams();
    params->m_routingParams->m_msisdn = a_msisdn;
    params->m_routingStrategy = new MsisdnRoutingStrategy();

    return this->LaunchRequstAndReturnResponse(params);
}


nlohmann::json nm::cdr::UIManager::SendOperatorRequestAndGetResponse(const std::string& a_mcc, const std::string& a_mnc) {
    // Create Builder's params
    UrlBuilderParams* params = static_cast<UrlBuilderParams*>(CreateParamsOfRequest());
    params->m_mainRouteOption = "operator";
    params->m_routingParams = new RoutingParams();
    params->m_routingParams->m_mccmnc = a_mcc + a_mnc;
    params->m_routingStrategy = new OperatorRoutingStrategy();

    return this->LaunchRequstAndReturnResponse(params);
}


nlohmann::json nm::cdr::UIManager::SendLinkRequestAndGetResponse(const std::string& a_firstParty, const std::string& a_secondParty) {
    // Create Builder's params
    UrlBuilderParams* params = static_cast<UrlBuilderParams*>(CreateParamsOfRequest());
    params->m_mainRouteOption = "link";
    params->m_routingParams = new RoutingParams();
    params->m_routingParams->m_firstParty = a_firstParty;
    params->m_routingParams->m_secondParty = a_secondParty;
    params->m_routingStrategy = new LinkRoutingStrategy();

    return this->LaunchRequstAndReturnResponse(params);
}