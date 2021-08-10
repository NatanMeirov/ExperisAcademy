#ifndef NM_TCP_SERVER_HXX
#define NM_TCP_SERVER_HXX


#include <cstddef> // size_t
#include <list> // std::list
#include <memory> // std::shared_ptr
#include <utility> // std::pair, std::make_pair
#include <unistd.h> // close
#include <vector> // std::vector
#include <string> // std::string, std::to_string
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::for_each
#include <sys/select.h> /* select, fd_set and its MACROS */
#include "tcp_server_socket.hpp"


namespace infra
{

template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::TCPServer(ClientMessageHandler a_onClientMessage, ErrorHandler a_onError, NewClientConnectionHandler a_onNewClientConnection, CloseClientConnectionHandler a_onCloseClientConnection, unsigned int a_listeningPort, unsigned int a_maxWaitingConnections)
: m_serverSocket(a_listeningPort)
, m_connectedClientsTable()
, m_onClientMessage(a_onClientMessage)
, m_onError(a_onError)
, m_onNewClientConnection(a_onNewClientConnection)
, m_onCloseClientConnection(a_onCloseClientConnection)
, m_socketsSignalsIndicator()
, m_maxWaitingConnections(a_maxWaitingConnections)
, m_maxAmountOfConnectedClientsAtTheSameTime(MAX_CONNECTED_CLIENTS_AT_THE_SAME_TIME_TO_SERVER)
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

    InitializeFDSet();
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
    tcpserver_details::StatusCode status;
    int socketsSignalsCount = 0;

    while(true)
    {
        if(m_isStopServerFromRunningRequired)
        {
            break;
        }

        // First: saving the "master" fd_set, because the select is deleting the given fd_set's content
        tempSocketsSignalsIndicator = m_socketsSignalsIndicator;

        socketsSignalsCount = select(FILE_DESCRIPTORS_LIMIT, &tempSocketsSignalsIndicator, NULL, NULL, NULL);
        if(socketsSignalsCount < 0)
        {
            m_isStopServerFromRunningRequired = m_onError(tcpserver_details::SERVER_INTERNAL_ERROR, MapServerErrorsToMessages(tcpserver_details::SERVER_INTERNAL_ERROR));
            if(m_isStopServerFromRunningRequired)
            {
                break; /* The main server loop will stop and the Run function will finish */
            }
        }

        // Handling new connections (checking the server's listening socket):
        if(FD_ISSET(m_serverSocket.InnerSocketID(), &tempSocketsSignalsIndicator))
        {
            status = AcceptNewClients();
            if(status != tcpserver_details::SUCCESS)
            {
                m_isStopServerFromRunningRequired = m_onError(status, MapServerErrorsToMessages(status));
                if(m_isStopServerFromRunningRequired)
                {
                    break; // The main server loop will stop and the Run function will finish
                }
            }

            --socketsSignalsCount; // After handling the listening socket
        }

        if(socketsSignalsCount > 0) // Check if only the listening socket had a notification -> if true: can continue to be blocked by the OS using the select (so end current loop)
        {
            try
            {
                HandleExistingClientsRequests(tempSocketsSignalsIndicator, socketsSignalsCount);
            }
            catch(const std::bad_alloc& baex)
            {
                m_isStopServerFromRunningRequired = m_onError(tcpserver_details::MEMORY_ALLOCATION_FAILED, MapServerErrorsToMessages(tcpserver_details::MEMORY_ALLOCATION_FAILED));
                if(m_isStopServerFromRunningRequired)
                {
                    break; // The main server loop will stop and the Run function will finish
                }
            }
            catch(const std::exception& ex)
            {
                m_isStopServerFromRunningRequired = m_onError(tcpserver_details::SERVER_INTERNAL_ERROR, MapServerErrorsToMessages(tcpserver_details::SERVER_INTERNAL_ERROR));
                if(m_isStopServerFromRunningRequired)
                {
                    break; // The main server loop will stop and the Run function will finish
                }
            }
        }
    }
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
std::string TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::MapServerErrorsToMessages(tcpserver_details::StatusCode a_statusCode) const
{
    if(a_statusCode == tcpserver_details::MEMORY_ALLOCATION_FAILED)
    {
        return "Failed while tried to allocate memory...";
    }
    else if(a_statusCode == tcpserver_details::ACCEPTING_CLIENT_FAILED)
    {
        return "Failed while tried to accept new client...";
    }
    else if(a_statusCode == tcpserver_details::SERVER_INTERNAL_ERROR)
    {
        return "An internal server error has occurred...";
    }
    else
    {
        return "Unknown error has occurred...";
    }
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
typename tcpserver_details::StatusCode TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::AcceptNewClients()
{
    HandlingClientResult result;

    if(m_currentConnectedClientsCount == m_maxAmountOfConnectedClientsAtTheSameTime)
    {
        return tcpserver_details::SUCCESS; /* Cannot add more clients for now */
    }

    try
    {
        m_serverSocket.Accept();
    }
    catch(const std::exception& ex)
    {
        return tcpserver_details::ACCEPTING_CLIENT_FAILED;
    }


    tcpserver_details::ClientID newClientID = m_serverSocket.GetLastAcceptedClientSocketID();
    try
    {
        std::shared_ptr<TCPSocket> m_newClientSocket = m_serverSocket.GetLastAcceptedClientSocket();
        m_connectedClientsTable.insert({newClientID, m_newClientSocket});
    }
    catch(const std::bad_alloc& baex)
    {
        // To keep class' invariants (no throw and does nothing if 0 elements have removed)
        m_connectedClientsTable.erase(newClientID);
        return tcpserver_details::MEMORY_ALLOCATION_FAILED;
    }
    catch(const std::exception& ex)
    {
        // To keep class' invariants (no throw and does nothing if 0 elements have removed)
        m_connectedClientsTable.erase(newClientID);
        return tcpserver_details::SERVER_INTERNAL_ERROR;
    }

    // Set the new client to notify the sockets signals indicator
    FD_SET(newClientID, &m_socketsSignalsIndicator);

    ++m_currentConnectedClientsCount;

    // Semi initialization of the response object
    tcpserver_details::Response response;
    response.m_clients.push_back(newClientID);

    m_onNewClientConnection(std::make_pair(newClientID, m_connectedClientsTable[newClientID]), response);

    result = HandleResponse(response);
    if(result == CLIENT_FINISH)
    {
        DisconnectAndRemoveClientFromServer(newClientID);
    }

    return tcpserver_details::SUCCESS;
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
typename TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandlingClientResult TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandleResponse(tcpserver_details::Response& a_response)
{
    if(a_response.m_status == tcpserver_details::SEND_MESSAGE)
    {
        size_t clientsCountToSendMessagesFor = a_response.m_clients.size();
        for(size_t i = 0; i < clientsCountToSendMessagesFor; ++i)
        {
            try
            {
                SendMessageTo(a_response.m_clients[i], a_response.m_message);
            }
            catch(...)
            {
                // Handling problematic client
                DisconnectAndRemoveClientFromServer(a_response.m_clients[i]);
            }
        }
    }
    else if(a_response.m_status == tcpserver_details::DISCONNECT_CLIENT)
    {
        return CLIENT_FINISH; // Disconnect the client
    }

    // DO_NOTHING or done with SEND_MESSAGE:
    return CLIENT_KEEP;
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
size_t TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::SendMessageTo(tcpserver_details::ClientID a_clientID, tcpserver_details::Message a_message)
{
    // Sync SendMessageTo version:
    // m_serverSocket.SetClientIDToSendMessageTo(a_clientID);
    // return m_serverSocket.Send(a_message);

    // Async SendMessageTo version:
    return m_connectedClientsTable[a_clientID]->Send(a_message);
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::DisconnectAndRemoveClientFromServer(tcpserver_details::ClientID a_clientID)
{
    m_onCloseClientConnection(a_clientID);

    m_connectedClientsTable.erase(a_clientID); // The std::shared_ptr destruction would close the FD of the TCPSocket, if there are no additional references to that TCPSocket
    FD_CLR(a_clientID, &m_socketsSignalsIndicator);
    close(a_clientID);
    --m_currentConnectedClientsCount;
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
void TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandleExistingClientsRequests(fd_set a_tempSocketsSignalsIndicator, int a_clientsSocketSignalsCount)
{
    // Using 'break' statement - cannot use std::for_each here...
    auto itr = m_connectedClientsTable.begin();
    auto endItr = m_connectedClientsTable.end();

    for(; itr != endItr; ++itr)
    {
        tcpserver_details::ClientID clientID = (*itr).first;
        bool isServerShouldStopAfterHandlingAllClients = false;

        if(FD_ISSET(clientID, &a_tempSocketsSignalsIndicator))
        {
            // Handle the client's request
            tcpserver_details::Message newMessage;
            HandlingClientResult result = HandleSingleClientRequest(clientID, newMessage);
            if(result == CLIENT_FINISH || result == CLIENT_ERROR)
            {
                DisconnectAndRemoveClientFromServer(clientID);
            }
            else // CLIENT_KEEP - message has successfully received for the client
            {
                tcpserver_details::Response response;
                response.m_clients.push_back(clientID); // Current client id is the default value of the response

                isServerShouldStopAfterHandlingAllClients = m_onClientMessage(newMessage, std::make_pair((*itr).first, (*itr).second), response);
                if(isServerShouldStopAfterHandlingAllClients)
                {
                    m_isStopServerFromRunningRequired = true;
                }

                // Handle application's response
                result = HandleResponse(response);
                if(result == CLIENT_FINISH)
                {
                    DisconnectAndRemoveClientFromServer(clientID);
                }

                // Finished to handle the response from the application
            }

            --a_clientsSocketSignalsCount;
            if(a_clientsSocketSignalsCount == 0) // Finished to handle all pending clients
            {
                break;
            }
        }
    }
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
typename TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandlingClientResult TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::HandleSingleClientRequest(tcpserver_details::ClientID a_clientID, tcpserver_details::Message& a_message)
{
    try
    {
        a_message = ReceiveMessageFrom(a_clientID);
    }
    catch(...)
    {
        // Handling problematic client
        return CLIENT_ERROR;
    }

    if(a_message.Size() == 0)
    {
        return CLIENT_FINISH; // The connection has finished by the client (an empty message had received)
    }

    return CLIENT_KEEP; // The message had received successfully
}


template<typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
typename tcpserver_details::Message TCPServer<ClientMessageHandler,ErrorHandler,NewClientConnectionHandler,CloseClientConnectionHandler>::ReceiveMessageFrom(tcpserver_details::ClientID a_clientID)
{
    m_serverSocket.SetClientIDToReceiveMessageFrom(a_clientID);

    tcpserver_details::Message completeMessage, newBuffer;
    newBuffer = m_serverSocket.Receive(MESSAGES_BUFFER_SIZE);
    while(newBuffer.Size() != 0) // A loop to receive the whole new message (the loop would stop when the newBuffer size is equal to 0, so the completeMessage is completed already in the previous iteration)
    {
        completeMessage += newBuffer;
        newBuffer = m_serverSocket.Receive(MESSAGES_BUFFER_SIZE);
    }

    return completeMessage; // Could be empty message if the first receive attempt returned an empty message
}


} // infra


#endif // NM_TCP_SERVER_HXX
