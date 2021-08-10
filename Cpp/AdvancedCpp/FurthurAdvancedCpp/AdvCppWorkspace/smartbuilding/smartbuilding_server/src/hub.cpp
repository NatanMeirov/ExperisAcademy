#include "hub.hpp"
#include <memory> // std::shared_ptr, std::make_shared
#include <utility> // std::pair
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "ipublisher.hpp"
#include "thread.hpp"
#include "thread_destruction_policies.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"
#include "tcp_server.hpp"
#include "event.hpp"
#include "smartbuilding_network_protocol.hpp"
#include "smartbuilding_request.hpp"
#include "events_router.hpp"
#include "events_subscription_organizer.hpp"
#include "software_agents_manager.hpp"
#include "software_agents_factory.hpp"
#include "safe_loggers_manager.hpp"
#include "remote_devices_sockets_manager.hpp"
#include "iconfig_reader.hpp"
#include "smartbuilding_request.hpp"
#include "smartbuilding_connect_request.hpp"
#include "smartbuilding_disconnect_request.hpp"
#include "smartbuilding_subscribe_request.hpp"
#include "smartbuilding_unsubscribe_request.hpp"
#include "smartbuilding_event_request.hpp"
#include "published_events_transmit_work.hpp"
#include "handled_buffers_transmit_work.hpp"


namespace smartbuilding
{

Hub::Hub(std::unique_ptr<SmartBuildingNetworkProtocol> a_networkProtocolParser, std::shared_ptr<IConfigReader> a_configFileReader, const std::string& a_configFileName, unsigned int a_serverPort, unsigned int a_maxWaitingClientsAtSameTime)
: m_networkProtocolParser(std::move(a_networkProtocolParser))
, m_subscribersOrganizer(std::make_shared<EventsSubscriptionOrganizer>())
, m_router(std::make_shared<EventsRouter>(m_subscribersOrganizer))
, m_agentsManager(std::make_shared<SoftwareAgentsManager>())
, m_loggersManager(std::make_shared<SafeLoggersManager>())
, m_socketsManager(std::make_shared<RemoteDevicesSocketsManager>())
, m_routingWorkers(std::make_shared<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>>(advcpp::ShutdownPolicy<>(), QUEUE_SIZE))
, m_sendingWorkers(std::make_shared<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>>(advcpp::ShutdownPolicy<>(), QUEUE_SIZE))
, m_publishedEventsQueue(std::make_shared<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>>(QUEUE_SIZE))
, m_handledBuffersQueue(std::make_shared<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>>(QUEUE_SIZE))
, m_publishedEventsTransmitter(std::make_shared<advcpp::Thread<advcpp::DetachPolicy>>(std::make_shared<PublishedEventsTransmitWork>(m_publishedEventsQueue, m_handledBuffersQueue, m_routingWorkers, m_router), advcpp::DetachPolicy()))
, m_handledBuffersTransmitter(std::make_shared<advcpp::Thread<advcpp::DetachPolicy>>(std::make_shared<HandledBuffersTransmitWork>(m_handledBuffersQueue, m_sendingWorkers, m_socketsManager), advcpp::DetachPolicy()))
, m_tcpServerDriver(OnClientMessageHandler(this), OnErrorHandler(), OnNewClientConnectionHandler(), OnCloseClientConnectionHandler(), a_serverPort, a_maxWaitingClientsAtSameTime)
{
    SoftwareAgentsFactory agentsFactory(m_agentsManager, m_loggersManager, a_configFileReader);
    agentsFactory.CreateAgents(a_configFileName);

    m_publishedEventsTransmitter->Detach();
    m_handledBuffersTransmitter->Detach();
}


Hub::~Hub()
{
    m_routingWorkers->Shutdown();
    m_sendingWorkers->Shutdown();
    m_publishedEventsTransmitter->Cancel();
    m_handledBuffersTransmitter->Cancel();
}


void Hub::Start()
{
    m_tcpServerDriver.Run();
}


bool Hub::OnClientMessageHandler::operator()(infra::tcpserver_details::Message& a_receivedMessage, std::pair<infra::tcpserver_details::ClientID, std::shared_ptr<infra::TCPSocket>> a_clientInfo, infra::tcpserver_details::Response& a_response)
{
    std::shared_ptr<SmartBuildingRequest> newRequestToHandle = m_thisHub->m_networkProtocolParser->Parse(a_receivedMessage);
    if(!newRequestToHandle)
    {
        a_response.m_status = infra::tcpserver_details::DO_NOTHING; // By default - do not operate in the server - almost complete handling would occur here
        return false; // Wrong buffer content (server should always continue its running)
    }

    if(newRequestToHandle->RequestType() == "Connect")
    {
        HandleNewConnectRequest(std::static_pointer_cast<SmartBuildingConnectRequest>(newRequestToHandle), a_response, a_clientInfo.second);
    }
    else if(newRequestToHandle->RequestType() == "Disconnect")
    {
        HandleNewDisconnectRequest(std::static_pointer_cast<SmartBuildingDisconnectRequest>(newRequestToHandle), a_response);
    }
    else if(newRequestToHandle->RequestType() == "Subscribe")
    {
        HandleNewSubscribeRequest(std::static_pointer_cast<SmartBuildingSubscribeRequest>(newRequestToHandle), a_response);
    }
    else if(newRequestToHandle->RequestType() == "Unsubscribe")
    {
        HandleNewUnsubscribeRequest(std::static_pointer_cast<SmartBuildingUnsubscribeRequest>(newRequestToHandle), a_response);
    }
    else if(newRequestToHandle->RequestType() == "Event")
    {
        HandleNewEventRequest(std::static_pointer_cast<SmartBuildingEventRequest>(newRequestToHandle), a_response);
    }

    return false; // Server should always continue its running
}


void Hub::OnClientMessageHandler::HandleNewConnectRequest(std::shared_ptr<SmartBuildingConnectRequest> a_connectRequest, infra::tcpserver_details::Response& a_response, std::shared_ptr<infra::TCPSocket> a_deviceSocket)
{
    std::string deviceID = a_connectRequest->RequestSenderID();
    std::string responseMessage;

    if(!IsExistInSystem(deviceID))
    {
        responseMessage = "{ response: unknown device error }";
    }
    else
    {
        m_thisHub->m_socketsManager->Insert(deviceID, a_deviceSocket);
        responseMessage = "{ response: connected successfully }";
    }

    // Response:
    a_response.m_status = infra::tcpserver_details::SEND_MESSAGE;
    a_response.m_message = infra::tcpserver_details::Message(reinterpret_cast<const unsigned char*>(responseMessage.c_str()), responseMessage.size());
}


void Hub::OnClientMessageHandler::HandleNewDisconnectRequest(std::shared_ptr<SmartBuildingDisconnectRequest> a_disconnectRequest, infra::tcpserver_details::Response& a_response)
{
    std::string deviceID = a_disconnectRequest->RequestSenderID();
    std::string responseMessage;

    if(!IsExistInSystem(deviceID))
    {
        responseMessage = "{ response: unknown device error }";
    }
    else
    {
        if(!IsConnected(deviceID))
        {
            responseMessage = "{ response: device is not connected error }";
        }
        else
        {
            m_thisHub->m_socketsManager->Remove(deviceID);
            responseMessage = "{ response: disconnected successfully }";
        }
    }

    // Response:
    a_response.m_status = infra::tcpserver_details::SEND_MESSAGE;
    a_response.m_message = infra::tcpserver_details::Message(reinterpret_cast<const unsigned char*>(responseMessage.c_str()), responseMessage.size());
}


// TODO: DRY - extract most of the code of subscribe and unsubscribe to a separated method, and execute the needed operation after choosing between subscribe/unsubscribe
void Hub::OnClientMessageHandler::HandleNewSubscribeRequest(std::shared_ptr<SmartBuildingSubscribeRequest> a_subscribeRequest, infra::tcpserver_details::Response& a_response)
{
    std::string deviceID = a_subscribeRequest->RequestSenderID();
    std::string responseMessage;

    if(!IsExistInSystem(deviceID))
    {
        responseMessage = "{ response: unknown device error }";
    }
    else
    {
        if(!IsConnected(deviceID))
        {
            responseMessage = "{ response: device is not connected error }";
        }
        else
        {
            std::shared_ptr<SoftwareAgent> deviceAgent = m_thisHub->m_agentsManager->FindByID(deviceID); // Won't be nullptr (checked before)
            std::shared_ptr<ISubscriber> deviceAsSubscriber = std::dynamic_pointer_cast<ISubscriber>(deviceAgent);
            if(!deviceAsSubscriber)
            {
                responseMessage = "{ response: device is not a subscriber error }";
            }
            else // If is indeed a subscriber
            {
                m_thisHub->m_subscribersOrganizer->Subscribe(deviceAsSubscriber, a_subscribeRequest->InterestedEventType(), a_subscribeRequest->SubscriptionLoc());
                responseMessage = "{ response: subscribed successfully }";
            }
        }
    }

    // Response:
    a_response.m_status = infra::tcpserver_details::SEND_MESSAGE;
    a_response.m_message = infra::tcpserver_details::Message(reinterpret_cast<const unsigned char*>(responseMessage.c_str()), responseMessage.size());
}


void Hub::OnClientMessageHandler::HandleNewUnsubscribeRequest(std::shared_ptr<SmartBuildingUnsubscribeRequest> a_unsubscribeRequest, infra::tcpserver_details::Response& a_response)
{
    std::string deviceID = a_unsubscribeRequest->RequestSenderID();
    std::string responseMessage;

    if(!IsExistInSystem(deviceID))
    {
        responseMessage = "{ response: unknown device error }";
    }
    else
    {
        if(!IsConnected(deviceID))
        {
            responseMessage = "{ response: device is not connected error }";
        }
        else
        {
            std::shared_ptr<SoftwareAgent> deviceAgent = m_thisHub->m_agentsManager->FindByID(deviceID); // Won't be nullptr (checked before)
            std::shared_ptr<ISubscriber> deviceAsSubscriber = std::dynamic_pointer_cast<ISubscriber>(deviceAgent);
            if(!deviceAsSubscriber)
            {
                responseMessage = "{ response: device is not a subscriber error }";
            }
            else // If is indeed a subscriber
            {
                m_thisHub->m_subscribersOrganizer->Unsubscribe(deviceAsSubscriber, a_unsubscribeRequest->SubscribedEventType());
                responseMessage = "{ response: unsubscribed successfully }";
            }
        }
    }

    // Response:
    a_response.m_status = infra::tcpserver_details::SEND_MESSAGE;
    a_response.m_message = infra::tcpserver_details::Message(reinterpret_cast<const unsigned char*>(responseMessage.c_str()), responseMessage.size());
}


void Hub::OnClientMessageHandler::HandleNewEventRequest(std::shared_ptr<SmartBuildingEventRequest> a_eventRequest, infra::tcpserver_details::Response& a_response)
{
    std::string deviceID = a_eventRequest->RequestSenderID();
    std::string responseMessage;

    if(!IsExistInSystem(deviceID))
    {
        responseMessage = "{ response: unknown device error }";
    }
    else
    {
        if(!IsConnected(deviceID))
        {
            responseMessage = "{ response: device is not connected error }";
        }
        else
        {
            std::shared_ptr<SoftwareAgent> deviceAgent = m_thisHub->m_agentsManager->FindByID(deviceID); // Won't be nullptr (checked before)
            std::shared_ptr<IPublisher> deviceAsPublisher = std::dynamic_pointer_cast<IPublisher>(deviceAgent);
            if(!deviceAsPublisher)
            {
                responseMessage = "{ response: device is not a publisher error }";
            }
            else // If is indeed a publisher
            {
                deviceAsPublisher->Publish(a_eventRequest->EventDataBuffer(), m_thisHub->m_publishedEventsQueue);
                responseMessage = "{ response: published event successfully }";
            }
        }
    }

    // Response:
    a_response.m_status = infra::tcpserver_details::SEND_MESSAGE;
    a_response.m_message = infra::tcpserver_details::Message(reinterpret_cast<const unsigned char*>(responseMessage.c_str()), responseMessage.size());
}


bool Hub::OnClientMessageHandler::IsExistInSystem(const std::string& a_deviceID)
{
    return m_thisHub->m_agentsManager->FindByID(a_deviceID) != nullptr;
}


bool Hub::OnClientMessageHandler::IsConnected(const std::string& a_deviceID)
{
    return m_thisHub->m_socketsManager->Find(a_deviceID) != nullptr;
}

} // smartbuilding
