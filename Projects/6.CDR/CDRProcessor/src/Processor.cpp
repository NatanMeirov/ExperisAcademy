#include "../inc/Processor.hpp"
#include <fstream> // std::ofstream, std::ifstream
#include <iostream> // Error handling
#include <vector>
#include <unistd.h> // sleep
#include <stdlib.h> // malloc
#include <string.h> // memcpy, strcpy
#include "../inc/RAMDataBase.hpp"
#include "../../Infrastructure/Multithreaded/Thread.hpp"
#include "../../Infrastructure/Multithreaded/TThreadPool.hpp"
#include "../../Infrastructure/Multithreaded/SafeQueue.hpp"
#include "../../Infrastructure/Network/TCPListeningSocket.hpp"
#include "../../Infrastructure/Network/Server/TCPServer.hpp"
#include "../../Infrastructure/System/Directory.hpp"
#include "../../Infrastructure/JsonSerializer/json.hpp"


static void* ProviderListeningAction(void* a_context);
static void* RestApiServerAction(void* a_context);
static int ServerOnError(ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo);
static int ServerOnMessage(void* _message, int _clientID, Response* _response, void* _applicationInfo);
int HandleError(Response* a_response);
int HandleMsisdn(const std::string& a_request, Response* a_response, nm::cdr::Processor::GlobalProcessorThreadsData* a_data);
int HandleOperator(const std::string& a_request, Response* a_response, nm::cdr::Processor::GlobalProcessorThreadsData* a_data);
int HandleLinkGraph(const std::string& a_request, Response* a_response, nm::cdr::Processor::GlobalProcessorThreadsData* a_data);


nm::cdr::Processor::Processor(const unsigned int a_processingTimeAmountInMinutes) // TODO: Create DataBaseFactory class
: m_parser()
, m_globalThreadsData(new GlobalProcessorThreadsData(new RAMDataBase()))
, m_processingTimeAmountInSeconds(a_processingTimeAmountInMinutes * Processor::SECONDS_IN_ONE_MINUTE) {
    m_globalThreadsData->m_database->Load("ProcessorFiles/database");
    this->StartProviderListeningThread();
    this->StartRestApiServerThread();
}


nm::cdr::Processor::~Processor() {
    this->m_globalThreadsData->m_isStopRequiredForRunningThreads = true; // Tells the running threads that they should stop
    for(size_t i = 0; i < this->m_globalThreadsData->m_processorRelatedThreads.size(); ++i) {
        this->m_globalThreadsData->m_processorRelatedThreads.at(i)->Cancel(); // Cancel the processor's working threads
    }
    m_globalThreadsData->m_database->Save("ProcessorFiles/database");
}


void nm::cdr::Processor::Run() {
    while(true) { // Polling
        sleep(this->SleepingAmount());
        this->Process();
    }
}


void nm::cdr::Processor::StartProviderListeningThread() {
    Thread providerListeningThread(&ProviderListeningAction, static_cast<void*>(this->m_globalThreadsData.get()));
    providerListeningThread.Detach();
    this->m_globalThreadsData->m_processorRelatedThreads.push_back(&providerListeningThread);
}


void nm::cdr::Processor::StartRestApiServerThread() {
    Thread restApiServerThread(&RestApiServerAction, static_cast<void*>(this->m_globalThreadsData.get()));
    restApiServerThread.Detach();
    this->m_globalThreadsData->m_processorRelatedThreads.push_back(&restApiServerThread);
}


// Implementation that uses a thread pool in a thread pool
// void nm::cdr::Processor::Process() {
//     SafeQueue<std::vector<Cdr>> outputQueue(Processor::WORKING_TASKS_QUEUE_SIZE);

//     auto task = [this, &outputQueue](const std::string& a_cdrFile){
//         outputQueue.Enqueue(this->m_parser.ParseCdrFileToCdrs(a_cdrFile));
//     };

//     compiletime::ThreadPool<std::string, decltype(task)> threadPool(Processor::THREADS_NUMBER, Processor::WORKING_TASKS_QUEUE_SIZE, task);

//     std::string newDirName("ProcessorFiles/new");
//     std::string doneDirName("ProcessorFiles/done");
//     Directory processingDirectory(newDirName);
//     Directory::DirectoryItem fileInDir;
//     while((fileInDir = processingDirectory.GetNextItem())) { // While is not nullptr - and there are more items in the directory
//         if((fileInDir.GetName() == "." || fileInDir.GetName() == "..")) {
//             continue;
//         }

//         // Processing
//         threadPool.PushWork(newDirName + "/" + fileInDir.GetName());

//         // Moving the file to "done" directory
//         std::ofstream doneFile(doneDirName + "/" + fileInDir.GetName()); // Creates new file with the same name in "done"
//         std::ifstream currentCdrFile(newDirName + "/" + fileInDir.GetName());

//         doneFile << currentCdrFile.rdbuf(); // Copy
//     }

//     while(!outputQueue.IsEmpty()) {
//         std::vector<Cdr> newCdrs = outputQueue.Dequeue();
//         for(size_t i = 0; i < newCdrs.size(); ++i) {
//             this->AddNewCdrToMsisdnToImsiTable(newCdrs.at(i));
//             this->m_globalThreadsData->m_database->Add(std::to_string(newCdrs.at(i).m_imsi), newCdrs.at(i));
//         }
//     }

//     this->RemoveAllFilesFromDirectory(newDirName);
// }


void nm::cdr::Processor::Process() {
    std::vector<Cdr> newCdrs;
    std::string newDirName("ProcessorFiles/new");
    std::string doneDirName("ProcessorFiles/done");
    Directory processingDirectory(newDirName);
    Directory::DirectoryItem fileInDir;
    while((fileInDir = processingDirectory.GetNextItem())) { // While is not nullptr - and there are more items in the directory
        if((fileInDir.GetName() == "." || fileInDir.GetName() == "..")) {
            continue;
        }

        // Processing and adding to the DataBase
        newCdrs = this->m_parser.ParseCdrFileToCdrs(newDirName + "/" + fileInDir.GetName());
        for(size_t i = 0; i < newCdrs.size(); ++i) {
            this->AddNewCdrToMsisdnToImsiTable(newCdrs.at(i));
            this->m_globalThreadsData->m_database->Add(std::to_string(newCdrs.at(i).m_imsi), newCdrs.at(i));
        }

        // Moving the file to "done" directory
        std::ofstream doneFile(doneDirName + "/" + fileInDir.GetName()); // Creates new file with the same name in "done"
        std::ifstream currentCdrFile(newDirName + "/" + fileInDir.GetName());

        doneFile << currentCdrFile.rdbuf(); // Copy

        // Remove the file from "new" directory
        std::remove((newDirName + "/" + fileInDir.GetName()).c_str());
    }
}


void nm::cdr::Processor::AddNewCdrToMsisdnToImsiTable(Cdr& a_newCdr) {
    if(this->m_globalThreadsData->m_msisdnToImsiTable.find(a_newCdr.m_msisdn) == this->m_globalThreadsData->m_msisdnToImsiTable.end()) {
        // Key does not exist in the table
        this->m_globalThreadsData->m_msisdnToImsiTable[a_newCdr.m_msisdn] = a_newCdr.m_imsi; // Add to the table
    }
}


static void* ProviderListeningAction(void* a_context) {
    nm::cdr::Processor::GlobalProcessorThreadsData* data = static_cast<nm::cdr::Processor::GlobalProcessorThreadsData*>(a_context);
    nm::TCPListeningSocket listeningSocket(data->m_portNumberOfProviderListening);

    listeningSocket.Configure();
    listeningSocket.Listen(data->m_providerListeningNumberOfConnectionWaitingQueue);

    // Main loop
    while(!data->m_isStopRequiredForRunningThreads) {
        nm::TCPSocket::BytesBufferProxy completeMessage, newBuffer;

        bool hasAConnection = listeningSocket.Accept();
        if(hasAConnection) {
            size_t bufferSize = data->m_providerListeningMaxBufferSizeForSingleMessage;
            newBuffer = listeningSocket.Receive(bufferSize);
            while(newBuffer.Size()) { // A loop to receive the whole new message
                completeMessage += newBuffer;
                newBuffer = listeningSocket.Receive(bufferSize);
            }

            if(!completeMessage.Size()) {
                continue; // Loop again - the message had not received at all
            }

            // Create new CdrFile
            std::ofstream newCdrFile(std::string("ProcessorFiles/new/cdrfile_") + std::to_string(data->SequenceNumber()) + ".txt");

            // Put the new bytes buffer in the Processor's "new" directory (ProcessorFiles/new/<cdr_filename>)
            newCdrFile.write(reinterpret_cast<const char*>(completeMessage.ToBytes()), completeMessage.Size());
        }
    }

    // If the provider listening has stopped
    data->m_ProviderListeningHasFinished = true; // "Soft landing" - thread's has safely stopped its running (and "signals" the processor through this flag)

    return nullptr;
}


static void* RestApiServerAction(void* a_context) {
    nm::cdr::Processor::GlobalProcessorThreadsData* data = static_cast<nm::cdr::Processor::GlobalProcessorThreadsData*>(a_context);
    nm::TCPServer server(static_cast<void*>(data), &ServerOnMessage, ServerOnError, nullptr, nullptr, data->m_portNumberOfRestApiServer, data->m_restApiServerMaxWaitingClients, data->m_restApiServerMaxBufferSizeForSingleMessage);
    server.Run();

    // If the server has stopped (can be by kill signal or by configuring the inner OnMessage handler function (to check the flag if stop is required))
    data->m_restApiServerHasFinished = true; // "Soft landing" - thread's has safely stopped its running (and "signals" the processor through this flag)

    return nullptr;
}


// Using C Api
static int ServerOnMessage(void* _message, int _clientID, Response* _response, void* _applicationInfo) {
    nm::cdr::Processor::GlobalProcessorThreadsData* data = static_cast<nm::cdr::Processor::GlobalProcessorThreadsData*>(_applicationInfo);
    std::string request(static_cast<const char*>(_message));
    if(request.find("GET") == std::string::npos) {
        return HandleError(_response);
    }

    if(request.find("msisdn") != std::string::npos) {
        return HandleMsisdn(request, _response, data);
    }
    else if(request.find("operator") != std::string::npos) {
        return HandleOperator(request, _response, data);
    }
    else if(request.find("link") != std::string::npos) {
        return HandleLinkGraph(request, _response, data);
    }
    else { // Error
        return HandleError(_response);
    }
}


int HandleError(Response* a_response) {
    size_t bufferSize = 1024; // 1 KB
    char* responseJson =  static_cast<char*>(malloc(bufferSize));
    if(!responseJson) {
        return 1;
    }

    nlohmann::json j = { {"error", "an error has occurred while trying to analyze the request"} };
    std::string jsonString = j.dump();
    strcpy(responseJson, jsonString.c_str());

    a_response->m_isMessageDeallocationRequired = 1;
    a_response->m_responseMessageContent = responseJson;
    a_response->m_responseMessageContentSize = bufferSize;
    a_response->m_responseStatus = RESPONSE_SEND_MESSAGE;

    return 0;
}


int HandleMsisdn(const std::string& a_request, Response* a_response, nm::cdr::Processor::GlobalProcessorThreadsData* a_data) {
    // TODO
    return 0;
}


int HandleOperator(const std::string& a_request, Response* a_response, nm::cdr::Processor::GlobalProcessorThreadsData* a_data) {
    // TODO
    return 0;
}


int HandleLinkGraph(const std::string& a_request, Response* a_response, nm::cdr::Processor::GlobalProcessorThreadsData* a_data) {
    // TODO
    return 0;
}



int ServerOnError(ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo) {
    std::cerr << _errorMessage << std::endl;

    return 0;
}