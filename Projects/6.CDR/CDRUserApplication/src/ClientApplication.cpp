#include "../inc/ClientApplication.hpp"
#include "../inc/UIManager.hpp"


nm::cdr::ClientApplication::ClientApplication()
: m_manager(ClientApplication::REST_API_SERVER_IP, ClientApplication::REST_API_SERVER_PORT) {
}


void nm::cdr::ClientApplication::Run() {
    // 1. Show menu
    // 2. Get input (number) [msisdn {client number}, operator {2 strings}, link {2 client numbers}]
    // 3. Get the strings (according to the prev selection)
    // 4. use the ui manager methods
    // 5. print the result to user
}