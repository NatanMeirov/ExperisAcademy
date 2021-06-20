#ifndef __NM_CDR_PROCESSOR_HPP__
#define __NM_CDR_PROCESSOR_HPP__


#include <memory> // std::shared_ptr
#include "IDataBase.hpp"
#include "CdrFileParser.hpp"


namespace nm {

namespace cdr {

class Processor {
public:
    struct GlobalProcessorThreadsData {
        GlobalProcessorThreadsData(const IDataBase& a_referenceToDatabase) : m_referenceToDatabase(a_referenceToDatabase), m_hasStopRequiredForRunningThreads(false), m_ProviderListeningHasFinished(false), m_restApiServerHasFinished(false) {}

        static constexpr unsigned int m_portNumberOfProviderListening = 4040;
        static constexpr unsigned int m_portNumberOfRestApiServer = 8080;
        static constexpr unsigned int m_restApiServerMaxWaitingClients = 1000;
        static constexpr unsigned int m_restApiServerMaxBufferSizeForSingleMessage = 4096;
        const IDataBase& m_referenceToDatabase;
        bool m_hasStopRequiredForRunningThreads;
        bool m_ProviderListeningHasFinished;
        bool m_restApiServerHasFinished;
    };

    Processor();
    ~Processor();

    void Process(); // Main loop of processor - checks the directory and uses the parser to get the new Cdrs (from CdrFile) - and update the database (database should use threads pool!)

private:
    void StartProviderListeningThread();
    void StartRestApiServerThread();

    std::shared_ptr<IDataBase> m_database; // Order is important!
    CdrFileParser m_parser;
    GlobalProcessorThreadsData m_globalThreadsData; // The threads should get a reference (an address) of this global data to be used inside them (should be passed as the context to the thread)
};

} // cdr

} // nm


#endif // __NM_CDR_PROCESSOR_HPP__