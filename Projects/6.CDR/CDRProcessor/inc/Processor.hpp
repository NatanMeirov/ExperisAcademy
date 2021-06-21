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
        GlobalProcessorThreadsData(IDataBase* a_database) : m_database(a_database), m_isStopRequiredForRunningThreads(false), m_ProviderListeningHasFinished(false), m_restApiServerHasFinished(false) {}

        static constexpr unsigned int m_portNumberOfProviderListening = 4040;
        static constexpr unsigned int m_portNumberOfRestApiServer = 8080;
        static constexpr unsigned int m_restApiServerMaxWaitingClients = 1000;
        static constexpr unsigned int m_restApiServerMaxBufferSizeForSingleMessage = 4096;
        std::shared_ptr<IDataBase> m_database;
        bool m_isStopRequiredForRunningThreads;
        bool m_ProviderListeningHasFinished;
        bool m_restApiServerHasFinished;
    };

    Processor();
    ~Processor();

    void Process(); // Main loop of processor - checks the directory and uses the parser to get the new Cdrs (from CdrFile) - and update the database (database should use threads pool!)

private:
    void StartProviderListeningThread();
    void StartRestApiServerThread();

    CdrFileParser m_parser;
    std::shared_ptr<GlobalProcessorThreadsData> m_globalThreadsData; // The threads should get a reference (an address) of this global data to be used inside them (should be passed as the context to the thread)
};

} // cdr

} // nm


#endif // __NM_CDR_PROCESSOR_HPP__