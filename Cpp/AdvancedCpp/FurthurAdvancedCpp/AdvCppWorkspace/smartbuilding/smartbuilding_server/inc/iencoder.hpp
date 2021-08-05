#ifndef NM_IENCODER_HPP
#define NM_IENCODER_HPP


#include "tcp_socket.hpp"
#include "event.hpp"


namespace smartbuilding
{

// An interface of a encoder class that the VENDOR of the HW device should supply to its related SoftwareAgent that would use it, to encode an Event object to a bytes data buffer
class IEncoder
{
public:
    virtual ~IEncoder() = default;
    virtual infra::TCPSocket::BytesBufferProxy Encode(Event a_event) = 0;
};

} // smartbuilding


#endif // NM_IENCODER_HPP
