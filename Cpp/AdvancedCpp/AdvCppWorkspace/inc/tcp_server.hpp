#ifndef NM_TCP_SERVER_HPP
#define NM_TCP_SERVER_HPP


#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include <string> // std::string
#include <list> // std::list
#include <vector> // std::vector
#include <unordered_map> // std::unordered_map
#include <sys/select.h> // fd_set
#include "tcp_server_socket.hpp"
#include "tcp_socket.hpp"


namespace infra
{

// Note: all the required functors MUST be copy-constructable and implement operator(), that gets diffrent parameters (see more details in the following lines),
//       and should handle: ClientMessageHandler - on new message arrived, ErrorHandler - on error occurrance, NewClientConnectionHandler - on new client connected, CloseClientConnectionHandler - on closed connection with client
// Note 2: the application context (like: [this]) - should be part of the given handling functors
// Note 3: the ClientMessageHandler and ErrorHandler functors should return a boolean value - true if the server should stop its running, or false if the server should continue its running, the others should return nothing (void)

// Concept of ClientMessageHandler: should be a functor that implements: operator()(Message&, std::pair<ClientID,std::shared_ptr<TCPSocket>>, Response&) while Message is the received buffer, and ClientID and its related TCPSocket as pair - is the client that sent that message, and a REFERENCE to a semi filled Response object to send back, while response is:
//                                 - The response status to tell the server which operation it should do: RESPONSE_DO_NOTHING, RESPONSE_SEND_MESSAGE, RESPONSE_DISCONNECT_CLIENT (with the specified ID of the response object) [if is default value or other input - the server will use DO_NOTHING]
//                                 - The clients ID to send the response message to (by default: adding the client ID of the client that the server received the message from)
//                                 - The response message to send to the selected clients
// Concept of ErrorHandler: should be a functor that implements: operator()(StatusCode, std::string) - while StatusCode is the error that occurred, and std::string is the related error message
// Concept of NewClientConnectionHandler: should be a functor that implements: operator()(std::pair<ClientID,std::shared_ptr<TCPSocket>>, Response&)- while clientID and its related TCPSocket is the information about the new connected client, and a REFERENCE to a Response object (like above)
// Concept of CloseClientConnectionHandler: should be a functor that implements: operator()(ClientID) - while ClientID is the client that the connection has closed with
template <typename ClientMessageHandler, typename ErrorHandler, typename NewClientConnectionHandler, typename CloseClientConnectionHandler>
class TCPServer
{
public:
    using Message = TCPListeningSocket::BytesBufferProxy;
    using ClientID = int;
    enum StatusCode { SUCCESS, MEMORY_ALLOCATION_FAILED, ACCEPTING_CLIENT_FAILED, SERVER_INTERNAL_ERROR };
    enum HandlingClientResult { CLIENT_FINISH, CLIENT_KEEP, CLIENT_ERROR };
    enum ResponseStatus { DO_NOTHING, SEND_MESSAGE, DISCONNECT_CLIENT };
    struct Response
    {
        ResponseStatus m_status; // The operation that the server should do, if an operation not specified (or wrong value) - the server will use DO_NOTHING command
        std::vector<ClientID> m_clients; // The clients to send the response message to (default value: the client info of the client that the server received a message from)
        Message m_message;
    };

public:

// Warning - an exception would be thrown from the c'tor in the following cases:
// If a_serverPort is not between 1025 to 64K
// If a_maxWaitingConnections is equal to 0
// The server initialization can fail because of various internal errors, if at least one of them occurs: an exception would be thrown (and will NOT trigger the OnError handle functor)
    TCPServer(ClientMessageHandler a_onClientMessage, ErrorHandler a_onError, NewClientConnectionHandler a_onNewClientConnection, CloseClientConnectionHandler a_onCloseClientConnection, unsigned int a_listeningPort, unsigned int a_maxWaitingConnections);
    TCPServer(const TCPServer& a_other) = delete;
    TCPServer& operator=(const TCPServer& a_other) = delete;
    ~TCPServer() = default; // Each TCPSocket is closing its own wrapped file descriptor (SocketID)

    void Run();

private:
    void InitializeFDSet();
    std::string MapServerErrorsToMessages(StatusCode a_statusCode) const;
    StatusCode AcceptNewClients();
    HandlingClientResult HandleResponse(Response& a_response);
    size_t SendMessageTo(ClientID a_clientID, Message a_message);
    void DisconnectAndRemoveClientFromServer(ClientID a_clientID);
    void HandleExistingClientsRequests(fd_set a_tempSocketsSignalsIndicator, int a_clientsSocketSignalsCount);
    HandlingClientResult HandleSingleClientRequest(ClientID a_clientID, Message& a_message);
    Message ReceiveMessageFrom(ClientID a_clientID);

private:
    static const size_t MESSAGES_BUFFER_SIZE = 4096;
    static const unsigned int MAX_CONNECTED_CLIENTS_AT_THE_SAME_TIME_TO_SERVER = 1020;
    static const unsigned int FILE_DESCRIPTORS_LIMIT = 1024;
    static const unsigned int MIN_BUFFER_SIZE = 1024;
    static const unsigned int MIN_PORT_VALUE = 1025;
    static const unsigned int MAX_PORT_VALUE = 64000;

private:
    TCPServerSocket m_serverSocket;
    std::unordered_map<ClientID,std::shared_ptr<TCPSocket>> m_connectedClientsTable;
    ClientMessageHandler m_onClientMessage;
    ErrorHandler m_onError;
    NewClientConnectionHandler m_onNewClientConnection;
    CloseClientConnectionHandler m_onCloseClientConnection;
    fd_set m_socketsSignalsIndicator;
    unsigned int m_maxWaitingConnections;
    unsigned int m_maxAmountOfConnectedClientsAtTheSameTime;
    unsigned int m_currentConnectedClientsCount;
    bool m_isStopServerFromRunningRequired;
};

} // infra


#include "inl/tcp_server.hxx"


#endif // NM_TCP_SERVER_HPP
