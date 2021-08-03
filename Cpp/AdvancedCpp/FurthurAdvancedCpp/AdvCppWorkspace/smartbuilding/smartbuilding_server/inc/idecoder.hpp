#ifndef NM_IDECODER_HPP
#define NM_IDECODER_HPP


#include "tcp_socket.hpp"
#include "event.hpp"


namespace smartbuilding
{

class IDecoder
{
public:
    virtual ~IDecoder() = default;
    virtual Event Decode(infra::TCPSocket::BytesBufferProxy a_bytesBuffer) = 0;
};

} // smartbuilding


#endif // NM_IDECODER_HPP
