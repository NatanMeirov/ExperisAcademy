#ifndef __NM_CDR_CLIENTAPPLICATION_HPP__
#define __NM_CDR_CLIENTAPPLICATION_HPP__

#include <string>// std::string
#include "UIManager.hpp"


namespace nm {

namespace cdr {

class ClientApplication {
public:
    ClientApplication();
    ClientApplication(const ClientApplication& a_other) = delete;
    ClientApplication& operator=(const ClientApplication& a_other) = delete;
    ~ClientApplication() = default;

    void Run();

private:
    static constexpr const char* REST_API_SERVER_IP = "127.0.0.1";
    static constexpr unsigned int REST_API_SERVER_PORT = 8080;

    void ShowMenu() const;
    std::string GetUserRequestAsString() const;
    int GetUserRequestAsNumber() const;

    UIManager m_manager;
};

} // cdr

} // nm


#endif // __NM_CDR_CLIENTAPPLICATION_HPP__