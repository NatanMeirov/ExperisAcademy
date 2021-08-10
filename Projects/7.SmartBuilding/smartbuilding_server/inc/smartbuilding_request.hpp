#ifndef NM_SMARTBUILDING_REQUEST_HPP
#define NM_SMARTBUILDING_REQUEST_HPP


#include <string> // std::string


namespace smartbuilding
{

// An abstract base class for all the Smart Building System's network requests
class SmartBuildingRequest
{
public:
    virtual ~SmartBuildingRequest() = default;
    virtual std::string RequestType() const = 0;

    std::string RequestSenderID() const { return m_requestSenderID; }

protected:
    SmartBuildingRequest(const std::string& a_requestSenderID) : m_requestSenderID(a_requestSenderID) {}

private:
    std::string m_requestSenderID;
};

} // smartbuilding


#endif // NM_SMARTBUILDING_REQUEST_HPP
