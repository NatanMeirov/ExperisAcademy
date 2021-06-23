#include "../inc/Processor.hpp"
#include <fstream> // std::ofstream, std::ifstream
#include <iostream> // Error handling
#include <vector>
#include <time.h> // time
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include "../inc/RAMDataBase.hpp"
#include "../../Infrastructure/Multithreaded/Thread.hpp"
#include "../../Infrastructure/Multithreaded/TThreadPool.hpp"
#include "../../Infrastructure/Multithreaded/SafeQueue.hpp"
#include "../../Infrastructure/Network/TCPListeningSocket.hpp"
#include "../../Infrastructure/Network/Server/TCPServer.hpp"
#include "../../Infrastructure/System/Directory.hpp"


static void* ProviderListeningAction(void* a_context);
static void* RestApiServerAction(void* a_context);
static int ServerOnError(ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo);
static int ServerOnMessage(void* _message, int _clientID, Response* _response, void* _applicationInfo);

nm::cdr::Processor::Processor() // TODO: Create DataBaseFactory class
: m_parser()
, m_globalThreadsData(new GlobalProcessorThreadsData(new RAMDataBase())) {
    m_globalThreadsData->m_database->Load("ProcessorFiles/database");
    this->StartProviderListeningThread();
    this->StartRestApiServerThread();
}


nm::cdr::Processor::~Processor() {
    this->m_globalThreadsData->m_isStopRequiredForRunningThreads = true; // Tells the running threads that they should stop
    // TODO - find a way to make a "soft landing" and cancel both threads
    m_globalThreadsData->m_database->Save("ProcessorFiles/database");
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
    SafeQueue<std::vector<Cdr>> outputQueue(Processor::WORKING_TASKS_QUEUE_SIZE);

    auto task = [this, &outputQueue](const std::string& a_cdrFile){
        outputQueue.Enqueue(this->m_parser.ParseCdrFileToCdrs(a_cdrFile));
    };

    compiletime::ThreadPool<std::string, decltype(task)> threadPool(Processor::THREADS_NUMBER, Processor::WORKING_TASKS_QUEUE_SIZE, task);

    Directory processingDirectory("ProcessorFiles/new");
    Directory::DirectoryItem fileInDir;
    while((fileInDir = processingDirectory.GetNextItem())) { // While is not nullptr
        if((fileInDir.GetName() == "." || fileInDir.GetName() == "..")) {
            continue;
        }

        // Processing
        threadPool.PushWork(std::string("ProcessorFiles/new/") + fileInDir.GetName());

        // Moving the file to "done" directory
        std::ofstream doneFile(std::string("ProcessorFiles/done/") + fileInDir.GetName()); // Creates new file with the same name in "done"
        std::ifstream currentCdrFile(std::string("ProcessorFiles/new/") + fileInDir.GetName());

        doneFile << currentCdrFile.rdbuf(); // Copy
    }

    while(!outputQueue.IsEmpty()) {
        std::vector<Cdr> newCdrs = outputQueue.Dequeue();
        for(size_t i = 0; i < newCdrs.size(); ++i) {
            this->m_globalThreadsData->m_database->Add(newCdrs.at(i).m_msisdn, newCdrs.at(i));
        }
    }

    // TODO -DONT FORGET TO DELETE THE FILES IN THE NEW DIRECTORY!
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
            std::ofstream newCdrFile(std::string("ProcessorFiles/new/cdrfile_") + std::to_string(time(nullptr)) + ".txt");

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
    nm::TCPServer server(nullptr, &ServerOnMessage, ServerOnError, nullptr, nullptr, data->m_portNumberOfRestApiServer, data->m_restApiServerMaxWaitingClients, data->m_restApiServerMaxBufferSizeForSingleMessage);
    server.Run();

    // If the server has stopped (can be by kill signal or by configuring the inner OnMessage handler function (to check the flag if stop is required))
    data->m_restApiServerHasFinished = true; // "Soft landing" - thread's has safely stopped its running (and "signals" the processor through this flag)

    return nullptr;
}


// Using C Api
static int ServerOnMessage(void* _message, int _clientID, Response* _response, void* _applicationInfo) {
    // TODO:  read message, go to database and extract the correct data and return it to the user
    unsigned char* response = (unsigned char*)malloc(19 * sizeof(unsigned char));
    memcpy(response, "{ response: error }", 19);

    _response->m_isMessageDeallocationRequired = 1;
    _response->m_responseMessageContent = response;
    _response->m_responseMessageContentSize = 19;
    _response->m_responseStatus = RESPONSE_SEND_MESSAGE;

    return 0;
}


int ServerOnError(ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo) {
    std::cerr << _errorMessage << std::endl;

    return 0;
}