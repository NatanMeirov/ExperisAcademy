#ifndef NM_SMARTBUILDING_NETWORK_PROTOCOL_HPP
#define NM_SMARTBUILDING_NETWORK_PROTOCOL_HPP


#include <memory> // std::shared_ptr
#include "tcp_socket.hpp" // BytesBufferProxy
#include "smartbuilding_request.hpp"


namespace smartbuilding
{

/**
*
* Protocol's buffer design documentation:
*
* each new field should be saperated by & sign. (Spaces in this example should be ignored)
* note: this protocol should be updated when a new SmartBuildingRequest type is going to be added to the system's requests supported list!
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
* Subscribe request:
* ['S' & DeviceID & Room/s & Floor/s & EventType]
* Rooms: a COMMA separeted list of rooms to subscribe for (to indicate all rooms: put 0 without any additional commas)
* Floors: a COMMA separeted list of floors to subscribe for (to indicate all floors: put 0 without any additional commas)
* EventType: string
*
*
* Event request:
* ['E' & DeviceID & Room & Floor & AdditionalEventData]
* AdditionalEventData: BytesBufferProxy (should include ALL the data that the IPublisher device should get to create a new Event object [example: event type, location (room + floor), timestamp, and data payload])
*
**/


// A singleton SmartBuildingNetworkProtocol class
// Provides a double check lock initialization to support multithreading (multithreaded safety)
class SmartBuildingNetworkProtocol
{
public:
    // An heap allocation initialization, returns nullptr if SmartBuildingNetworkProtocol had already initialized in the system
    static std::shared_ptr<SmartBuildingNetworkProtocol> GetNetworkProtocol();

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
    std::shared_ptr<SmartBuildingRequest> ParseConnectRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    std::shared_ptr<SmartBuildingRequest> ParseSubscribeRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
    std::shared_ptr<SmartBuildingRequest> ParseEventRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const;
};

} // smartbuilding


#endif // NM_SMARTBUILDING_NETWORK_PROTOCOL_HPP
