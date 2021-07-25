#include "../inc/tcp_server.hpp" //!
#include <cstddef> // size_t
#include <list> // std::list
#include <memory> // std::shared_ptr
#include <unistd.h> // close
#include <vector> // std::vector
#include <string> // std::string, std::to_string
#include <stdexcept> // std::runtime_error
#include <sys/select.h> /* select, fd_set and its MACROS */
#include "../inc/tcp_server_socket.hpp" //!
#include "atomic_value.hpp"


namespace infra
{

template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::TCPServer(ClientMessageHandler a_onClientMessage, ErrorHandler a_onError, NewClientConnectionHandler a_onNewClientConnection, CloseClientConnectionHandler a_onCloseClientConnection, unsigned int a_listeningPort, unsigned int a_maxWaitingConnections, size_t a_receivedMessagesBufferSize)
: m_serverSocket(a_listeningPort)
, m_connectedClients()
, m_problematicClients()
, m_onClientMessage(a_onClientMessage)
, m_onError(a_onError)
, m_onNewClientConnection(a_onNewClientConnection)
, m_onCloseClientConnection(a_onCloseClientConnection)
, m_socketsSignalsIndicator()
, m_maxWaitingConnections(a_maxWaitingConnections)
, m_maxAmountOfConnectedClientsAtTheSameTime(MAX_CONNECTED_CLIENTS_AT_THE_SAME_TIME_TO_SERVER)
, m_receivedMessagesBufferSize(a_receivedMessagesBufferSize)
, m_currentConnectedClientsCount(0)
, m_isStopServerFromRunningRequired(false)
{
    if(a_listeningPort < MIN_PORT_VALUE || a_listeningPort > MAX_PORT_VALUE)
    {
        throw std::runtime_error("Error: wrong port number");
    }
    if(a_maxWaitingConnections == 0)
    {
        throw std::runtime_error("Error: maximum amount of waiting connections cannot be 0");
    }
    if(a_receivedMessagesBufferSize < MIN_BUFFER_SIZE)
    {
        throw std::runtime_error("Error: minimum messages buffer size must be more than " + std::to_string(MIN_BUFFER_SIZE));
    }

    InitializeServerSocket(a_maxWaitingConnections);
    InitializeFDSet();
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::InitializeServerSocket(size_t a_maxWaitingConnections)
{
    m_serverSocket.Configure();
    m_serverSocket.Listen(a_maxWaitingConnections);
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::InitializeFDSet()
{
    FD_ZERO(&m_socketsSignalsIndicator);
    FD_SET(m_serverSocket.InnerSocketID(), &m_socketsSignalsIndicator);
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::Run()
{
    fd_set tempSocketsSignalsIndicator;
    StatusCode status;
    advcpp::AtomicValue<int> socketsSignalsCount;

    while(true)
    {
        if(m_isStopServerFromRunningRequired)
        {
            break;
        }

        // First: saving the "master" fd_set, because the select is deleting the given fd_set's content
        tempSocketsSignalsIndicator = m_socketsSignalsIndicator;

        socketsSignalsCount = select(FILE_DESCRIPTORS_LIMIT, &tempSocketsSignalsIndicator, NULL, NULL, NULL);
        if(socketsSignalsCount.Get() < 0)
        {
            m_isStopServerFromRunningRequired = m_onError(SERVER_INTERNAL_ERROR, MapServerErrorsToMessages(SERVER_INTERNAL_ERROR));
            if(m_isStopServerFromRunningRequired)
            {
                break; /* The main server loop will stop and the Run function will finish */
            }
        }

        // Handling new connections (checking the server's listening socket):
        if(FD_ISSET(m_serverSocket.InnerSocketID(), &tempSocketsSignalsIndicator))
        {
            status = AcceptNewClients();
            if(status != SUCCESS)
            {
                m_isStopServerFromRunningRequired = m_onError(status, MapServerErrorsToMessages(status));
                if(m_isStopServerFromRunningRequired)
                {
                    break; // The main server loop will stop and the ServerRun function will finish
                }
            }

            --socketsSignalsCount; // After handling the listening socket
        }

        if(socketsSignalsCount > 0) // Check if only the listening socket had a notification -> if true: can continue to be blocked by the OS using the select (so end current loop)
        {
            HandleExistingClientsRequests(tempSocketsSignalsIndicator, socketsSignalsCount.Get());
        }
    }
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
std::string TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::MapServerErrorsToMessages(StatusCode a_statusCode) const
{
    if(a_statusCode == MEMORY_ALLOCATION_FAILED)
    {
        return "Failed while tried to allocate memory...";
    }
    else if(a_statusCode == ACCEPTING_CLIENT_FAILED)
    {
        return "Failed while tried to accept new client...";
    }
    else if(a_statusCode == SERVER_INTERNAL_ERROR)
    {
        return "An internal server error has occurred...";
    }
    else
    {
        return "Unknown error has occurred...";
    }
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
typename TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::StatusCode TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::AcceptNewClients()
{
    HandlingClientResult result;

    if(m_currentConnectedClientsCount == m_maxAmountOfConnectedClientsAtTheSameTime)
    {
        return SUCCESS; /* Cannot add more clients, for now */
    }

    try
    {
        m_serverSocket.Accept();
    }
    catch(const std::exception& ex)
    {
        return ACCEPTING_CLIENT_FAILED;
    }

    ClientInfo newClient;

    try
    {
        newClient.m_clientID = m_serverSocket.GetLastAcceptedClientSocketID();
        std::pair<std::string,unsigned int> clientIpAndPortInfo = m_serverSocket.GetLastAcceptedClientIpAndPortData();
        newClient.m_clientIPAddress = clientIpAndPortInfo.first;
        newClient.m_clientPort = clientIpAndPortInfo.second;

        m_connectedClients[newClient.m_clientID] = newClient;
    }
    catch(const std::bad_alloc& baex)
    {
        return MEMORY_ALLOCATION_FAILED;
    }
    catch(const std::exception& ex)
    {
        return SERVER_INTERNAL_ERROR;
    }

    // Set the new client to notify the sockets signals indicator
    FD_SET(newClient.m_clientID, &m_socketsSignalsIndicator);

    ++m_currentConnectedClientsCount;

    // Semi initialization of the response object
    std::shared_ptr<Response> response(new Response());
    response->m_clients.push_back(newClient.m_clientID);

    m_onNewClientConnection(newClient, response);

    result = HandleResponse(response);
    if(result == CLIENT_FINISH)
    {
        DisconnectAndRemoveClientFromServer(newClient.m_clientID);
    }

    return SUCCESS;
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
typename TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandlingClientResult TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandleResponse(std::shared_ptr<Response> a_response)
{
    if(a_response->m_status == SEND_MESSAGE)
    {
        size_t clientsCountToSendMessageFor = a_response->m_clients.size();
        for(size_t i = 0; i < clientsCountToSendMessageFor; ++i)
        {
            try
            {
                SendMessageTo(a_response->m_clients[i], a_response->m_message);
            }
            catch(...)
            {
                m_problematicClients.push_back(a_response->m_clients[i]);
            }
        }
    }
    else if(a_response->m_status == DISCONNECT_CLIENT)
    {
        return CLIENT_FINISH; // Disconnect the client
    }

    // DO_NOTHING or done with SEND_MESSAGE:
    return CLIENT_KEEP;
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
size_t TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::SendMessageTo(ClientID a_clientID, Message a_message)
{
    m_serverSocket.SetClientIDToSendMessageTo(a_clientID);
    return m_serverSocket.Send(a_message);
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::DisconnectAndRemoveClientFromServer(ClientID a_clientID)
{
    m_onCloseClientConnection(m_connectedClients.at(a_clientID));

    m_connectedClients.erase(a_clientID);
    FD_CLR(a_clientID, &m_socketsSignalsIndicator);
    close(a_clientID);
    --m_currentConnectedClientsCount;
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandleExistingClientsRequests(fd_set a_socketsSignalsIndicator, int a_clientsSocketSignalsCount)
{
    advcpp::AtomicValue<int> clientsSocketSignalsCount(a_clientsSocketSignalsCount);
    bool isServerShouldStopAfterHandlingAllClients = false;
    HandlingClientResult result;

    // TODO!
}

} // infra