#ifndef NM_IDECODER_HPP
#define NM_IDECODER_HPP


#include "tcp_socket.hpp"
#include "event.hpp"


namespace smartbuilding
{

// An interface of a decoder class that the VENDOR of the HW device should supply to its related SoftwareAgent that would use it, to decode a bytes data buffer to a new Event object
class IDecoder
{
public:
    virtual ~IDecoder() = default;
    virtual Event Decode(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) = 0;
};

} // smartbuilding


#endif // NM_IDECODER_HPP
