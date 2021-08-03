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
    virtual void Notify(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<infra::TCPSocket::BytesBufferProxy, advcpp::NoOperationPolicy<infra::TCPSocket::BytesBufferProxy>>> a_handledBuffersQueue) = 0; // TODO: check if should enqueue pair<> of buffer + id indicator to know to which HW controller send the buffer
};

} // smartbuilding


#endif // NM_ISUBSCRIBER_HPP
