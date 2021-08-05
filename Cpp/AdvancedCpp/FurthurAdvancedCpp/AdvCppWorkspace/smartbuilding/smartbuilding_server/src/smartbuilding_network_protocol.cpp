#include "smartbuilding_network_protocol.hpp"
#include <string> // std::string, std::getline, std::stoul
#include <sstream> // std::stringstream
#include <mutex> // std::mutex, std::lock_guard
#include "tcp_socket.hpp"
#include "event.hpp"


namespace smartbuilding
{

std::shared_ptr<SmartBuildingNetworkProtocol> SmartBuildingNetworkProtocol::GetNetworkProtocol()
{
    static bool hasInitializedAlready = false;
    static std::mutex lock;

    if(!hasInitializedAlready) // Used to avoid the expensive OS call while mutex is not needed at all (after the first initialization)
    {
        std::lock_guard<std::mutex> guard(lock);
        if(!hasInitializedAlready)
        {
            hasInitializedAlready = true;
            return std::shared_ptr<SmartBuildingNetworkProtocol>(new SmartBuildingNetworkProtocol());
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
    else if(IsRequestType('S', a_bytesBuffer))
    {
        return ParseSubscribeRequest(a_bytesBuffer);
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


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseConnectRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{

}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseSubscribeRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{

}


std::shared_ptr<SmartBuildingRequest> SmartBuildingNetworkProtocol::ParseEventRequest(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{

}


bool SmartBuildingNetworkProtocol::IsRequestType(char a_type, infra::TCPSocket::BytesBufferProxy a_bytesBuffer) const
{
    return a_bytesBuffer.ToBytes()[0] == static_cast<unsigned char>(a_type);
}


// Event SmartBuildingNetworkProtocol::Unpack(infra::TCPSocket::BytesBufferProxy a_buffer) const
// {
//     std::stringstream buffer(reinterpret_cast<const char*>(a_buffer.ToBytes()));
//     std::string subStringContainer;
//     char delimiter = '&';

//     // EventType extraction:
//     std::getline(buffer, subStringContainer, delimiter);
//     Event::EventType type = subStringContainer;

//     // Location extraction:
//     std::getline(buffer, subStringContainer, delimiter);
//     unsigned int floor = std::stoul(subStringContainer);
//     std::getline(buffer, subStringContainer, delimiter);
//     unsigned int room = std::stoul(subStringContainer);

//     Event::EventLocation location(floor, room);

//     // Timestamp extraction:
//     std::getline(buffer, subStringContainer, delimiter);
//     std::string time = subStringContainer;
//     std::getline(buffer, subStringContainer, delimiter);
//     std::string date = subStringContainer;

//     Event::EventTimestamp timestamp = ParseDateAndTime(time, date);

//     // Payload data extraction:
//     std::getline(buffer, subStringContainer, delimiter);
//     infra::TCPSocket::BytesBufferProxy data(reinterpret_cast<const unsigned char*>(subStringContainer.c_str()), subStringContainer.size()); // Should not copy the '\0' at the end of the string

//     return Event(data, timestamp, location, type);
// }


// DateTime SmartBuildingNetworkProtocol::ParseDateAndTime(const std::string& a_time, const std::string& a_date) const
// {
//     std::string subStringContainer;
//     char delimiter = ':';
//     std::stringstream timeBuffer(a_time);

//     // Time extraction:
//     std::getline(timeBuffer, subStringContainer, delimiter);
//     int hours = std::stoi(subStringContainer);
//     std::getline(timeBuffer, subStringContainer, delimiter);
//     int minutes = std::stoi(subStringContainer);
//     std::getline(timeBuffer, subStringContainer, delimiter);
//     int seconds = std::stoi(subStringContainer);

//     delimiter = '.';
//     std::stringstream dateBuffer(a_date);

//     // Date extraction:
//     std::getline(timeBuffer, subStringContainer, delimiter);
//     int day = std::stoi(subStringContainer);
//     std::getline(timeBuffer, subStringContainer, delimiter);
//     int month = std::stoi(subStringContainer);
//     std::getline(timeBuffer, subStringContainer, delimiter);
//     int year = std::stoi(subStringContainer);

//     return Event::EventTimestamp(hours, minutes, seconds, day, month, year);
// }

} // smartbuilding
