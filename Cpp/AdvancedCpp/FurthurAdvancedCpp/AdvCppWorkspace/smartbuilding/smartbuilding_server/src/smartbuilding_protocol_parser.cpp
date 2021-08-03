#include "smartbuilding_protocol_parser.hpp"
#include <string> // std::string, std::getline, std::stoul, std::stoi
#include <sstream> // std::stringstream
#include <mutex> // std::mutex, std::lock_guard
#include "tcp_socket.hpp"
#include "event.hpp"


namespace smartbuilding
{

std::shared_ptr<SmartBuildingProtocolParser> SmartBuildingProtocolParser::GetProtocolParser()
{
    static bool hasInitializedAlready = false;
    static std::mutex lock;

    if(!hasInitializedAlready) // Used to avoid the expensive OS call while mutex is not needed at all (after the first initialization)
    {
        std::lock_guard<std::mutex> guard(lock);
        if(!hasInitializedAlready)
        {
            hasInitializedAlready = true;
            return std::shared_ptr<SmartBuildingProtocolParser>(new SmartBuildingProtocolParser());
        }
    }

    return nullptr;
}


infra::TCPSocket::BytesBufferProxy SmartBuildingProtocolParser::Pack(const Event& a_event) const
{
    std::string buffer;
    std::string sep("&");

    // Pack EventType:
    buffer += a_event.Type();
    buffer += sep;

    // Pack EventLocation:
    Event::EventLocation loc = a_event.Location();
    buffer += std::to_string(loc.Floor());
    buffer += sep;
    buffer += std::to_string(loc.Room());
    buffer += sep;

    // Pack EventTimestamp:
    Event::EventTimestamp timestamp = a_event.Timestamp();
    buffer += timestamp.TimeToString();
    buffer += sep;
    buffer += timestamp.DateToString();
    buffer += sep;

    // Should not copy the '\0' at the end of the string:
    return infra::TCPSocket::BytesBufferProxy(reinterpret_cast<const unsigned char*>(buffer.c_str()), buffer.size()) + a_event.Data();
}


Event SmartBuildingProtocolParser::Unpack(infra::TCPSocket::BytesBufferProxy a_buffer) const
{
    std::stringstream buffer(reinterpret_cast<const char*>(a_buffer.ToBytes()));
    std::string subStringContainer;
    char delimiter = '&';

    // EventType extraction:
    std::getline(buffer, subStringContainer, delimiter);
    Event::EventType type = subStringContainer;

    // Location extraction:
    std::getline(buffer, subStringContainer, delimiter);
    unsigned int floor = std::stoul(subStringContainer);
    std::getline(buffer, subStringContainer, delimiter);
    unsigned int room = std::stoul(subStringContainer);

    Event::EventLocation location(floor, room);

    // Timestamp extraction:
    std::getline(buffer, subStringContainer, delimiter);
    std::string time = subStringContainer;
    std::getline(buffer, subStringContainer, delimiter);
    std::string date = subStringContainer;

    Event::EventTimestamp timestamp = ParseDateAndTime(time, date);

    // Payload data extraction:
    std::getline(buffer, subStringContainer, delimiter);
    infra::TCPSocket::BytesBufferProxy data(reinterpret_cast<const unsigned char*>(subStringContainer.c_str()), subStringContainer.size()); // Should not copy the '\0' at the end of the string

    return Event(data, timestamp, location, type);
}


DateTime SmartBuildingProtocolParser::ParseDateAndTime(const std::string& a_time, const std::string& a_date) const
{
    std::string subStringContainer;
    char delimiter = ':';
    std::stringstream timeBuffer(a_time);

    // Time extraction:
    std::getline(timeBuffer, subStringContainer, delimiter);
    int hours = std::stoi(subStringContainer);
    std::getline(timeBuffer, subStringContainer, delimiter);
    int minutes = std::stoi(subStringContainer);
    std::getline(timeBuffer, subStringContainer, delimiter);
    int seconds = std::stoi(subStringContainer);

    delimiter = '.';
    std::stringstream dateBuffer(a_date);

    // Date extraction:
    std::getline(timeBuffer, subStringContainer, delimiter);
    int day = std::stoi(subStringContainer);
    std::getline(timeBuffer, subStringContainer, delimiter);
    int month = std::stoi(subStringContainer);
    std::getline(timeBuffer, subStringContainer, delimiter);
    int year = std::stoi(subStringContainer);

    return Event::EventTimestamp(hours, minutes, seconds, day, month, year);
}

} // smartbuilding
