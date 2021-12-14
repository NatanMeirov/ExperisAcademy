#ifndef NM_SMARTBUILDING_NETWORK_PROTOCOL_HPP
#define NM_SMARTBUILDING_NETWORK_PROTOCOL_HPP


#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include <string> // std::string
#include <sstream> // std::stringstream, std::getline
#include "tcp_socket.hpp" // BytesBufferProxy
#include "smartbuilding_request.hpp"


namespace smartbuilding
{

/**
*
* Protocol's buffer design documentation:
*
* Each new field should be saperated by & sign. (Spaces in this example should be ignored)
* Note: this protocol should be updated when a new SmartBuildingRequest type is going to be added to the system's requests supported list!
*
* A base buffer example:
* [ RequestType & DeviceID (if needed: & AdditionalData)]
* RequestType: char (for the valid chars table - check the supported requests details below)
* DeviceID: string
*
*
* Supported requests buffers designs (Spaces in this example should be ignored):
*
* Connect request:
* ['C' & DeviceID]
*
*
* Warning: before sending a disconnect request, the remote devices MUST make sure to send unsubscribe requests, to unsubscribe itself from ALL the pre-subscribed events!
* Disconnect request:
* ['D' & DeviceID]
*
*
* Subscribe request:
* ['S' & DeviceID & Room/s & Floor/s & EventType]
* Rooms: a COMMA separeted list of rooms to subscribe for (to indicate all rooms: put 0 without any additional commas)
* Floors: a COMMA separeted list of floors to subscribe for (to indicate all floors: put 0 without any additional commas)
* EventType: string
*
*
* Unsubscribe request:
* ['U' & DeviceID & EventType]
* EventType: string
*
*
* Event request:
* ['E' & DeviceID & EventDataBuffer]
* EventDataBuffer: BytesBufferProxy (should include ALL the data that the IPublisher device should get to create a new Event object [example: event type, location (room + floor), timestamp, and data payload])
*
**/


// A singleton SmartBuildingNetworkProtocol class
// Provides a double check lock initialization to support multithreading (multithreaded safety)
class SmartBuildingNetworkProtocol
{
public:
    // An heap allocation initialization, returns nullptr if SmartBuildingNetworkProtocol had already initialized in the system
    static std::unique_ptr<SmartBuildingNetworkProtocol> GetNetworkProtocol();

    SmartBuildingNetworkProtocol(const SmartBuildingNetworkProtocol& a_other) = delete;
    SmartBuildingNetworkProtocol& operator=(const SmartBuildingNetworkProtocol& a_other) = delete;
    ~SmartBuildingNetworkProtocol() = default;

    // Main protocol's parsing method:
    // [Returns nullptr if the buffer is empty or if the buffer is not matching the protocol]
    std::shared_ptr<SmartBuildingRequest> Parse(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;

    // TODO: Add a support to pack HW device's supplied data object into a complete Protocol's Buffer, ready for easy networking send operation

private:
    SmartBuildingNetworkProtocol() = default;

private:
    bool IsRequestType(char a_type, infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    bool HasAllIndication(const std::vector<unsigned int>& a_sequence) const;
    std::shared_ptr<SmartBuildingRequest> ParseConnectRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    std::shared_ptr<SmartBuildingRequest> ParseDisconnectRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    std::shared_ptr<SmartBuildingRequest> ParseSubscribeRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    std::shared_ptr<SmartBuildingRequest> ParseUnsubscribeRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    std::shared_ptr<SmartBuildingRequest> ParseEventRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;

    template<typename NextType>
    NextType ExtractNext(std::stringstream& a_ss, char a_delimiter = '\n') const // If NextType CANNOT be initialize be passing a std::string to its constructor - this template function should be SPECIALIZED TO FIT THE NEEDS!
    {
        std::string subStringContainer;
        std::getline(a_ss, subStringContainer, a_delimiter);

        return NextType(subStringContainer);
    }
};

} // smartbuilding


#endif // NM_SMARTBUILDING_NETWORK_PROTOCOL_HPP
