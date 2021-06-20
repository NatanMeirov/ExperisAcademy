#include "../inc/Processor.hpp"
#include "../inc/RAMDataBase.hpp"
#include "../../Infrastructure/Multithreaded/Thread.hpp"
#include "../../Infrastructure/Network/TCPListeningSocket.hpp"
#include "../../Infrastructure/Network/Server/TCPServer.hpp"


static void* ProviderListeningAction(void* a_context);
static void* RestApiServerAction(void* a_context);


nm::cdr::Processor::Processor()
: m_database(new RAMDataBase()) // TODO: Create DataBaseFactory class
, m_parser()
, m_globalThreadsData(*this->m_database) {
    this->m_database->Load("../ProcessorFiles/database/CDRProcessorData.txt");
    this->StartProviderListeningThread();
    this->StartRestApiServerThread();
}


nm::cdr::Processor::~Processor() {
    this->m_globalThreadsData.m_hasStopRequiredForRunningThreads = true; // Tells the running threads that they should stop
    while(!this->m_globalThreadsData.m_ProviderListeningHasFinished || !this->m_globalThreadsData.m_restApiServerHasFinished); // A polling while loop - to be 100% sure that the threads indeed completely finished their job (after the stopping signal through the global data)
    this->m_database->Save("../ProcessorFiles/database/CDRProcessorData.txt");
}



void nm::cdr::Processor::StartProviderListeningThread() {
    Thread providerListeningThread(&ProviderListeningAction, static_cast<void*>(&this->m_globalThreadsData));
    providerListeningThread.Detach();
}


void nm::cdr::Processor::StartRestApiServerThread() {
    Thread providerListeningThread(&RestApiServerAction, static_cast<void*>(&this->m_globalThreadsData));
    providerListeningThread.Detach();
}


static void* ProviderListeningAction(void* a_context) {
    nm::cdr::Processor::GlobalProcessorThreadsData* data = static_cast<nm::cdr::Processor::GlobalProcessorThreadsData*>(a_context);
    nm::TCPListeningSocket listeningSocket(data->m_portNumberOfProviderListening);

    listeningSocket.Configure();
    listeningSocket.Listen();

    // Main loop
    while(!data->m_hasStopRequiredForRunningThreads) {
        bool hasAConnection = listeningSocket.Accept();
        if(hasAConnection) {
            size_t bufferSize = 4096; // 4 KB
            nm::TCPSocket::BytesBufferProxy completeMessage;
            nm::TCPSocket::BytesBufferProxy newBuffer = listeningSocket.Receive(bufferSize);
            while(newBuffer.Size()) { // A loop to receive the whole message
                completeMessage += newBuffer;
                newBuffer = listeningSocket.Receive(bufferSize);
            }
        }

        // 1. Parse the message to a CdrFile
        // 2. Put the new file in the Processor's "new" directory (ProcessorFiles/new) [use DIR* and struct dirent*]
    }

    data->m_ProviderListeningHasFinished = true; // "Soft landing" - thread's has safely stopped its running (and "signals" the processor through this flag)

    return nullptr;
}


static void* RestApiServerAction(void* a_context) {
    nm::cdr::Processor::GlobalProcessorThreadsData* data = static_cast<nm::cdr::Processor::GlobalProcessorThreadsData*>(a_context);
    nm::TCPServer server(nullptr, nullptr, nullptr, nullptr, nullptr, data->m_portNumberOfRestApiServer, data->m_restApiServerMaxWaitingClients, data->m_restApiServerMaxBufferSizeForSingleMessage); // TODO - create relevant handling function!
    server.Run();

    // If the server has stopped (can be by kill signal or by configuring the inner OnMessage handler function (to check the flag if stop is required))
    data->m_restApiServerHasFinished = true; // "Soft landing" - thread's has safely stopped its running (and "signals" the processor through this flag)

    return nullptr;
}