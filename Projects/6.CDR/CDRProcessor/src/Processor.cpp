#include "../inc/Processor.hpp"
#include <fstream> // std::ofstream
#include <time.h> // time
#include "../inc/RAMDataBase.hpp"
#include "../../Infrastructure/Multithreaded/Thread.hpp"
#include "../../Infrastructure/Network/TCPListeningSocket.hpp"
#include "../../Infrastructure/Network/Server/TCPServer.hpp"


static void* ProviderListeningAction(void* a_context);
static void* RestApiServerAction(void* a_context);


nm::cdr::Processor::Processor() // TODO: Create DataBaseFactory class
: m_parser()
, m_globalThreadsData(new GlobalProcessorThreadsData(new RAMDataBase())) {
    m_globalThreadsData->m_database->Load("../ProcessorFiles/database/CDRProcessorData.txt");
    this->StartProviderListeningThread();
    this->StartRestApiServerThread();
}


nm::cdr::Processor::~Processor() {
    this->m_globalThreadsData->m_isStopRequiredForRunningThreads = true; // Tells the running threads that they should stop
    while(!this->m_globalThreadsData->m_ProviderListeningHasFinished || !this->m_globalThreadsData->m_restApiServerHasFinished); // A polling while loop - to be 100% sure that the threads indeed completely finished their job (after the stopping signal through the global data)
    m_globalThreadsData->m_database->Save("../ProcessorFiles/database/CDRProcessorData.txt");
}



void nm::cdr::Processor::StartProviderListeningThread() {
    Thread providerListeningThread(&ProviderListeningAction, static_cast<void*>(this->m_globalThreadsData.get()));
    providerListeningThread.Detach();
}


void nm::cdr::Processor::StartRestApiServerThread() {
    Thread providerListeningThread(&RestApiServerAction, static_cast<void*>(this->m_globalThreadsData.get()));
    providerListeningThread.Detach();
}


void nm::cdr::Processor::Process() {
 // TODO
}


static void* ProviderListeningAction(void* a_context) {
    nm::cdr::Processor::GlobalProcessorThreadsData* data = static_cast<nm::cdr::Processor::GlobalProcessorThreadsData*>(a_context);
    nm::TCPListeningSocket listeningSocket(data->m_portNumberOfProviderListening);

    listeningSocket.Configure();
    listeningSocket.Listen();

    // Main loop
    while(!data->m_isStopRequiredForRunningThreads) {
        nm::TCPSocket::BytesBufferProxy completeMessage, newBuffer;

        bool hasAConnection = listeningSocket.Accept();
        if(hasAConnection) {
            size_t bufferSize = 4096; // 4 KB
            newBuffer = listeningSocket.Receive(bufferSize);
            while(newBuffer.Size()) { // A loop to receive the whole new message
                completeMessage += newBuffer;
                newBuffer = listeningSocket.Receive(bufferSize);
            }

            if(!completeMessage.Size()) {
                continue; // Loop again - the message had not received at all
            }

            // Create new CdrFile
            std::ofstream newCdrFile(std::string("../ProcessorFiles/new/cdrfile_") + std::to_string(time(nullptr)) + ".txt");

            // Put the new bytes buffer in the Processor's "new" directory (ProcessorFiles/new/<cdr_filename>)
            newCdrFile << completeMessage.ToBytes();
        }
    }

    // If the provider listening has stopped
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