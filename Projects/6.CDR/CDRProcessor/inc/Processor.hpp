#ifndef __NM_CDR_PROCESSOR_HPP__
#define __NM_CDR_PROCESSOR_HPP__


#include <memory> // std::shared_ptr
#include <unordered_map>
#include <vector>
#include "IDataBase.hpp"
#include "CdrFileParser.hpp"
#include "../../Infrastructure/Multithreaded/Mutex.hpp"
#include "../../Infrastructure/Multithreaded/LockGuard.hpp"
#include "../../Infrastructure/Multithreaded/Thread.hpp"


namespace nm {

namespace cdr {

class Processor {
public:
    struct GlobalProcessorThreadsData {
        GlobalProcessorThreadsData(IDataBase* a_database) : m_database(a_database), m_msisdnToImsiTable(), m_processorRelatedThreads(), m_isStopRequiredForRunningThreads(false), m_ProviderListeningHasFinished(false), m_restApiServerHasFinished(false), m_cdrFilesSequencialNumber(0), m_lock() {}

        static constexpr unsigned int m_portNumberOfProviderListening = 4040;
        static constexpr unsigned int m_portNumberOfRestApiServer = 8080;
        static constexpr unsigned int m_providerListeningNumberOfConnectionWaitingQueue = 10;
        static constexpr unsigned int m_restApiServerMaxWaitingClients = 100;
        static constexpr unsigned int m_restApiServerMaxBufferSizeForSingleMessage = 4096; // 4 KB
        static constexpr unsigned int m_providerListeningMaxBufferSizeForSingleMessage = 4096; // 4 KB
        std::shared_ptr<IDataBase> m_database;
        std::unordered_map<std::string, uint64_t> m_msisdnToImsiTable; // A map to be used to "map" a given MSISDN number (string) to IMSI number (uint64)
        std::vector<nm::Thread*> m_processorRelatedThreads;
        bool m_isStopRequiredForRunningThreads;
        bool m_ProviderListeningHasFinished;
        bool m_restApiServerHasFinished;

        size_t SequenceNumber() { nm::LockGuard guard(this->m_lock); return this->m_cdrFilesSequencialNumber++; } // THe guard would help if in the future the system supports more then 1 listening to provider's socket thread

    private:
        size_t m_cdrFilesSequencialNumber;
        nm::Mutex m_lock;
    };

    Processor();
    ~Processor();

    void Process(); // Main loop of processor - checks the directory and uses the parser to get the new Cdrs (from CdrFile) - and update the database (database should use threads pool!)

private:
    static const unsigned int THREADS_NUMBER = 4; // TODO: use configuration file
    static const unsigned int WORKING_TASKS_QUEUE_SIZE = 39; // TODO: use configuration file

    void AddNewCdrToMsisdnToImsiTable(Cdr& a_newCdr);
    void RemoveAllFilesFromDirectory(const std::string& a_dirNameToRemoveAllItemsFrom);

    void StartProviderListeningThread();
    void StartRestApiServerThread();

    CdrFileParser m_parser;
    std::shared_ptr<GlobalProcessorThreadsData> m_globalThreadsData; // The threads should get a reference (an address) of this global data to be used inside them (should be passed as the context to the thread)
};

} // cdr

} // nm


#endif // __NM_CDR_PROCESSOR_HPP__