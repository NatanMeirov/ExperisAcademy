#ifndef NM_IENCODER_HPP
#define NM_IENCODER_HPP


#include "tcp_socket.hpp"
#include "event.hpp"


namespace smartbuilding
{

class IEncoder
{
public:
    virtual ~IEncoder() = default;
    virtual infra::TCPSocket::BytesBufferProxy Encode(Event a_event) = 0;
};

} // smartbuilding


#endif // NM_IENCODER_HPP
