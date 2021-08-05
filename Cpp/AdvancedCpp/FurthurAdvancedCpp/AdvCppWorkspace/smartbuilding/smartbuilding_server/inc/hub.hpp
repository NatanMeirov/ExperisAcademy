#ifndef NM_HUB_HPP
#define NM_HUB_HPP


#include <memory> // std::shared_ptr
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"
#include "tcp_server.hpp"
#include "smartbuilding_network_protocol.hpp"


namespace smartbuilding
{

class Hub
{
public:




private:
    std::shared_ptr<SmartBuildingNetworkProtocol> m_networkProtocolParser;
    advcpp::ThreadPool<advcpp::ShutdownPolicy<>> m_workers;
    // advcpp::ThreadPool<advcpp::ShutdownPolicy<>> m_emergencyWorkers; // TODO: add in version 2
};

} // smartbuilding


#endif // NM_HUB_HPP
