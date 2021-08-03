#ifndef NM_SMARTBUILDING_PROTOCOL_PARSER_HPP
#define NM_SMARTBUILDING_PROTOCOL_PARSER_HPP


#include <memory> // std::shared_ptr
#include "tcp_socket.hpp" // BytesBufferProxy
#include "event.hpp"


namespace smartbuilding
{
/**
* Protocol's buffer design (documentation):
* each new field should be saperated by & sign. (Spaces in this example should be ignored)
*
* [ Type & FloorNumber & RoomNumber & Hours:Minutes:Seconds & Day.Month.Year && AdditionalDataPayload ]
*
* Type: string
* FloorNumber: unsigned int
* RoomNumber: unsigned int
* Timestamp:
*       Hours: int
*       Minutes: int
*       Seconds: int
*       Day: int
*       Month: int
*       Year: int
* AdditionalDataPayload: BytesBufferProxy
**/


// A singleton SmartBuildingProtocolParser class
// Provides a double check lock initialization to support multithreading (multithreaded safety)
class SmartBuildingProtocolParser
{
public:
    // An heap allocation initialization, returns nullptr if SmartBuildingProtocolParser had already initialized in the system
    static std::shared_ptr<SmartBuildingProtocolParser> GetProtocolParser();

    SmartBuildingProtocolParser(const SmartBuildingProtocolParser& a_other) = delete;
    SmartBuildingProtocolParser& operator=(const SmartBuildingProtocolParser& a_other) = delete;
    ~SmartBuildingProtocolParser() = default;

    // Protocol's parsing methods
    infra::TCPSocket::BytesBufferProxy Pack(const Event& a_event) const;
    Event Unpack(infra::TCPSocket::BytesBufferProxy a_buffer) const;

private:
    SmartBuildingProtocolParser() = default;

private:
    Event::EventTimestamp ParseDateAndTime(const std::string& a_time, const std::string& a_date) const;
};

} // smartbuilding


#endif // NM_SMARTBUILDING_PROTOCOL_PARSER_HPP
