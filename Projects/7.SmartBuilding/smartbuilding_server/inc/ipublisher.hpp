#ifndef NM_IPUBLISHER_HPP
#define NM_IPUBLISHER_HPP


#include <memory> // std::shared_ptr
#include "tcp_socket.hpp"
#include "event.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace smartbuilding
{

class IPublisher
{
public:
    virtual ~IPublisher() = default;
    virtual void Publish(infra::TCPSocket::BytesBufferProxy a_bytesBuffer, std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueue) = 0;
};

} // smartbuilding


#endif // NM_IPUBLISHER_HPP