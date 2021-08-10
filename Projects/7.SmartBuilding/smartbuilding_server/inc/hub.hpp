#ifndef NM_HUB_HPP
#define NM_HUB_HPP


#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "thread.hpp"
#include "thread_destruction_policies.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"
#include "tcp_server.hpp"
#include "event.hpp"
#include "smartbuilding_network_protocol.hpp"
#include "events_router.hpp"
#include "events_subscription_organizer.hpp"
#include "software_agents_manager.hpp"
#include "safe_loggers_manager.hpp"
#include "remote_devices_sockets_manager.hpp"
#include "iconfig_reader.hpp"
#include "smartbuilding_request.hpp"
#include "smartbuilding_connect_request.hpp"
#include "smartbuilding_disconnect_request.hpp"
#include "smartbuilding_subscribe_request.hpp"
#include "smartbuilding_unsubscribe_request.hpp"
#include "smartbuilding_event_request.hpp"


namespace smartbuilding
{

// Note: the hub sends response messages for each request message from a client device in JSON format,
// but the Events to the listening devices (Controllers) are sent only after been encoded to a special format that can be read by the listening device
class Hub
{
public:
    Hub(std::unique_ptr<SmartBuildingNetworkProtocol> a_networkProtocolParser, std::shared_ptr<IConfigReader> a_configFileReader, const std::string& a_configFileName, unsigned int a_serverPort, unsigned int a_maxWaitingClientsAtSameTime);
    Hub(const Hub& a_other) = delete;
    Hub& operator=(const Hub& a_other) = delete;
    ~Hub();

    void Start();

private:
    // TODO: in version 2 - improve the handlers to act like in real time server action handlers
    class OnClientMessageHandler
    {
    public:
        OnClientMessageHandler(Hub* a_thisHub) : m_thisHub(a_thisHub) {};

        bool operator()(infra::tcpserver_details::Message& a_receivedMessage, std::pair<infra::tcpserver_details::ClientID,std::shared_ptr<infra::TCPSocket>> a_clientInfo, infra::tcpserver_details::Response& a_response);

    private:
        void HandleNewConnectRequest(std::shared_ptr<SmartBuildingConnectRequest> a_connectRequest, infra::tcpserver_details::Response& a_response, std::shared_ptr<infra::TCPSocket> a_deviceSocket);
        void HandleNewDisconnectRequest(std::shared_ptr<SmartBuildingDisconnectRequest> a_disconnectRequest, infra::tcpserver_details::Response& a_response);
        void HandleNewSubscribeRequest(std::shared_ptr<SmartBuildingSubscribeRequest> a_subscribeRequest, infra::tcpserver_details::Response& a_response);
        void HandleNewUnsubscribeRequest(std::shared_ptr<SmartBuildingUnsubscribeRequest> a_unsubscribeRequest, infra::tcpserver_details::Response& a_response);
        void HandleNewEventRequest(std::shared_ptr<SmartBuildingEventRequest> a_eventRequest, infra::tcpserver_details::Response& a_response);
        bool IsConnected(const std::string& a_deviceID);
        bool IsExistInSystem(const std::string& a_deviceID);

    private:
        Hub* m_thisHub;
    };


    class OnErrorHandler
    {
        bool operator()(infra::tcpserver_details::StatusCode a_status, const std::string& a_error)
        {
            (void)(a_status); // Not in use
            (void)(a_error); // Not in use
            // TODO: Log the error

            return true; // Stop the server from running
        }
    };


    class OnNewClientConnectionHandler
    {
        void operator()(std::pair<infra::tcpserver_details::ClientID,std::shared_ptr<infra::TCPSocket>> a_clientInfo, infra::tcpserver_details::Response& a_response)
        {
            a_response.m_status = infra::tcpserver_details::DO_NOTHING;
            (void)(a_clientInfo); // Not in use
        }
    };


    class OnCloseClientConnectionHandler
    {
        void operator()(infra::tcpserver_details::ClientID a_clientID)
        {
            (void)(a_clientID); // Not in use
        }
    };

private:
    static const unsigned int QUEUE_SIZE = 100; // TODO: in version 2, read this constant from a configuration file

private:
    std::unique_ptr<SmartBuildingNetworkProtocol> m_networkProtocolParser;
    std::shared_ptr<EventsSubscriptionOrganizer> m_subscribersOrganizer;
    std::shared_ptr<EventsRouter> m_router;
    std::shared_ptr<SoftwareAgentsManager> m_agentsManager;
    std::shared_ptr<SafeLoggersManager> m_loggersManager;
    std::shared_ptr<RemoteDevicesSocketsManager> m_socketsManager;
    std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> m_routingWorkers;
    std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> m_sendingWorkers;
    std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> m_publishedEventsQueue;
    std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> m_handledBuffersQueue;
    std::shared_ptr<advcpp::Thread<advcpp::DetachPolicy>> m_publishedEventsTransmitter;
    std::shared_ptr<advcpp::Thread<advcpp::DetachPolicy>> m_handledBuffersTransmitter;
    infra::TCPServer<OnClientMessageHandler,OnErrorHandler,OnNewClientConnectionHandler,OnCloseClientConnectionHandler> m_tcpServerDriver;
};

} // smartbuilding


#endif // NM_HUB_HPP
