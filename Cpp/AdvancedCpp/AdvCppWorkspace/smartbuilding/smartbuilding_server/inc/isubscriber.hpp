#ifndef NM_ISUBSCRIBER_HPP
#define NM_ISUBSCRIBER_HPP


#include <memory> // std::shared_ptr
#include "tcp_socket.hpp"
#include "event.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace smartbuilding
{

class ISubscriber
{
public:
    virtual ~ISubscriber() = default;
    virtual void Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue) = 0;
};

} // smartbuilding


#endif // NM_ISUBSCRIBER_HPP
