#include "smartbuilding_network_protocol.hpp"
#include <string> // std::string, std::stoul
#include <memory> // std::unique_ptr, std::make_shared
#include <sstream> // std::stringstream, std::getline
#include <mutex> // std::mutex, std::lock_guard
#include "subscription_location.hpp"
#include "tcp_socket.hpp"
#include "event.hpp"
#include "smartbuilding_request.hpp"
#include "smartbuilding_connect_request.hpp"
#include "smartbuilding_disconnect_request.hpp"
#include "smartbuilding_subscribe_request.hpp"
#include "smartbuilding_unsubscribe_request.hpp"
#include "smartbuilding_event_request.hpp"


namespace smartbuilding
{

std::unique_ptr<SmartBuildingNetworkProtocol> SmartBuildingNetworkProtocol::GetNetworkProtocol()
{
    static bool hasInitializedAlready = false;
    static std::mutex lock;

    if(!hasInitializedAlready) // Used to avoid the expensive OS call while mutex is not needed at all (after the first initialization)
    {
        std::lock_guard<std::mutex> guard(lock);
        if(!hasInitializedAlready)
        {
            hasInitializedAlready = true;
            return std::unique_ptr<SmartBuildingNetworkProtocol>(new SmartBuildingNetworkProtocol());
        }
    }

    return nullptr;
}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::Parse(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    if(a_bytesBuffer.Size() == 0)
    {
        return nullptr;
    }

    if(IsRequestType('C', a_bytesBuffer))
    {
        return ParseConnectRequest(a_bytesBuffer);
    }
    else if(IsRequestType('D', a_bytesBuffer))
    {
        return ParseDisconnectRequest(a_bytesBuffer);
    }
    else if(IsRequestType('S', a_bytesBuffer))
    {
        return ParseSubscribeRequest(a_bytesBuffer);
    }
    else if(IsRequestType('U', a_bytesBuffer))
    {
        return ParseUnsubscribeRequest(a_bytesBuffer);
    }
    else if(IsRequestType('E', a_bytesBuffer))
    {
        return ParseEventRequest(a_bytesBuffer);
    }
    else // Buffer's header is not following the protocol's guidelines
    {
        return nullptr;
    }
}


template <>
infra::TCPSocket::BytesBufferProxy SmartBuildingNetworkProtocol::ExtractNext<infra::TCPSocket::BytesBufferProxy>(std::stringstream& a_ss, char a_delimiter) const
{
    std::string subStringContainer;
    std::getline(a_ss, subStringContainer, a_delimiter);

    return infra::TCPSocket::BytesBufferProxy(reinterpret_cast<const unsigned char*>(subStringContainer.c_str()), subStringContainer.size()); // '\n' should be ignored
}


template <>
std::vector<unsigned int> SmartBuildingNetworkProtocol::ExtractNext<std::vector<unsigned int>>(std::stringstream& a_ss, char a_delimiter) const
{
    std::string subStringContainer;
    std::getline(a_ss, subStringContainer, a_delimiter);

    std::string singleNumAsString;
    char newDelimiter = ',';
    std::vector<unsigned int> sequence;
    while(std::getline(a_ss, singleNumAsString, newDelimiter))
    {
        sequence.push_back(std::stoul(singleNumAsString));
    }

    return sequence;
}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseConnectRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    std::stringstream buffer(reinterpret_cast<const char*>(a_bytesBuffer.ToBytes()));
    char delimiter = '&';
    ExtractNext<std::string>(buffer, delimiter); // Skip the request type indicator

    // Extract Device ID:
    std::string deviceID = ExtractNext<std::string>(buffer, delimiter);

    return std::make_shared<SmartBuildingConnectRequest>(deviceID);
}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseDisconnectRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    std::stringstream buffer(reinterpret_cast<const char*>(a_bytesBuffer.ToBytes()));
    char delimiter = '&';
    ExtractNext<std::string>(buffer, delimiter); // Skip the request type indicator

    // Extract Device ID:
    std::string deviceID = ExtractNext<std::string>(buffer, delimiter);

    return std::make_shared<SmartBuildingDisconnectRequest>(deviceID);
}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseSubscribeRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    std::stringstream buffer(reinterpret_cast<const char*>(a_bytesBuffer.ToBytes()));
    char delimiter = '&';
    ExtractNext<std::string>(buffer, delimiter); // Skip the request type indicator

    // Extract Device ID:
    std::string deviceID = ExtractNext<std::string>(buffer, delimiter);

    // Extract Rooms && Floors:
    std::vector<unsigned int> rooms = ExtractNext<std::vector<unsigned int>>(buffer, delimiter);
    std::vector<unsigned int> floors = ExtractNext<std::vector<unsigned int>>(buffer, delimiter);
    bool isAllRooms = HasAllIndication(rooms);
    bool isAllFloors = HasAllIndication(floors);

    SubscriptionLocation subscriptionLoc(isAllFloors, floors, isAllRooms, rooms);

    // Extract Event Type:
    Event::EventType eventType = ExtractNext<Event::EventType>(buffer, delimiter);

    return std::make_shared<SmartBuildingSubscribeRequest>(deviceID, eventType, subscriptionLoc);
}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseUnsubscribeRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    std::stringstream buffer(reinterpret_cast<const char*>(a_bytesBuffer.ToBytes()));
    char delimiter = '&';
    ExtractNext<std::string>(buffer, delimiter); // Skip the request type indicator

    // Extract Device ID:
    std::string deviceID = ExtractNext<std::string>(buffer, delimiter);

    // Extract Event Type:
    Event::EventType eventType = ExtractNext<Event::EventType>(buffer, delimiter);

    return std::make_shared<SmartBuildingUnsubscribeRequest>(deviceID, eventType);
}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseEventRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    std::stringstream buffer(reinterpret_cast<const char*>(a_bytesBuffer.ToBytes()));
    char delimiter = '&';
    ExtractNext<std::string>(buffer, delimiter); // Skip the request type indicator

    // Extract Device ID:
    std::string deviceID = ExtractNext<std::string>(buffer, delimiter);

    // Extract Event Data Buffer:
    infra::TCPSocket::BytesBufferProxy eventDataBuffer = ExtractNext<infra::TCPSocket::BytesBufferProxy>(buffer, delimiter);

    return std::make_shared<SmartBuildingEventRequest>(deviceID, eventDataBuffer);
}


bool SmartBuildingNetworkProtocol::IsRequestType(char a_type, infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    return a_bytesBuffer.ToBytes()[0] == static_cast<unsigned char>(a_type);
}


bool SmartBuildingNetworkProtocol::HasAllIndication(const std::vector<unsigned int>& a_sequence) const
{
    return a_sequence.size() == 1 && a_sequence[0] == 0; // 0 is the protocol's indicator if all rooms/floors are wanted
}

} // smartbuilding
