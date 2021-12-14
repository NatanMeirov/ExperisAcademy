#include <string> // std::string, std::stoul
#include <memory> // std::unique_ptr, std::shared_ptr, std::make_shared
#include <stdexcept> // std::invalid_argument
#include "hub.hpp"
#include "smartbuilding_network_protocol.hpp"
#include "iconfig_reader.hpp"
#include "ini_reader.hpp"


#define SERVER_SYSTEM_ARGS_COUNT 3


using namespace smartbuilding;


// Args: 1) server port | 2) server's max waiting clients at the same time | 3) config file path
int main(int argc, const char** argv)
{
    std::string configFilePath;
    unsigned int port;
    unsigned int clientsCount;

    try
    {
        if(argc != SERVER_SYSTEM_ARGS_COUNT + 1) // +1 stands for the program name
        {
            throw std::invalid_argument("Wrong argc value");
        }

        port = std::stoul(std::string(argv[1]));
        clientsCount = std::stoul(std::string(argv[2]));
        configFilePath = std::string(argv[3]);
    }
    catch(...)
    {
        return 1;
    }

    std::unique_ptr<SmartBuildingNetworkProtocol> networkProtocolParser = SmartBuildingNetworkProtocol::GetNetworkProtocol();
    std::shared_ptr<IConfigReader> iniReader = std::make_shared<IniReader>();

    Hub hub(std::move(networkProtocolParser), iniReader, configFilePath, port, clientsCount);
    hub.Start();

    return 0;
}
