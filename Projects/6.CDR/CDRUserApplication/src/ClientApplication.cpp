// THIS IS A TESTING CLIENT APP
// TODO - Change this client application to be more interactive and more safe (with better design)

#include "../inc/ClientApplication.hpp"
#include <string>
#include <iostream>
#include "../inc/UIManager.hpp"


#define CLIENT_BILLING 1
#define OPERATOR 2
#define CLIENT_LINK_GRAPH 3


nm::cdr::ClientApplication::ClientApplication()
: m_manager(ClientApplication::REST_API_SERVER_IP, ClientApplication::REST_API_SERVER_PORT) {
}


void nm::cdr::ClientApplication::Run() {
    while(true) { // TODO - Add an exit/quit option
        this->ShowMenu();
        long userChoice = this->GetUserRequestAsNumberInRange(1, 3);
        switch(userChoice) { // TODO: Use ICommand interface to implement OOD menu, and remove this switch case... Use enum instead of Pre-processor #define...
        case CLIENT_BILLING: {
            this->HandleClientBilling();
            break;
        }

        case OPERATOR: {
            this->HandleOperator();
            break;
        }

        case CLIENT_LINK_GRAPH: {
            this->HandleClientLinkGraph();
            break;
        }
        }

        std::cout << std::endl;
    }
}


void nm::cdr::ClientApplication::ShowMenu() const {
    // TODO: Upgrade the menu to be more interactive...
    std::cout << "Please enter your choice (as number):" << std::endl;
    std::cout << "1) Client (billing information)" << std::endl;
    std::cout << "2) Operator (operator settlements information)" << std::endl;
    std::cout << "3) Client's Link Graph with a second party client" << std::endl;
    std::cout << std::endl;
}


std::string nm::cdr::ClientApplication::GetUserRequestAsString() const {
    std::string userInputString;
    std::cin >> userInputString;

    return userInputString;
}


long nm::cdr::ClientApplication::GetUserRequestAsNumberInRange(long a_min, long a_max) const {
    long userInputNumber;
    std::cin >> userInputNumber;

    while(a_min > userInputNumber || a_max < userInputNumber) {
        std::cout << std::string("Try again please (valid input range: ") + std::to_string(a_min) + " to " + std::to_string(a_max) + ")" << std::endl;        std::cin >>  userInputNumber;
        std::cin >>  userInputNumber;
    }

    return userInputNumber;
}


void nm::cdr::ClientApplication::HandleClientBilling() {
    std::cout << "Enter MSISDN: ";
    std::string inputMsisdn = this->GetUserRequestAsString();

    std::cout << this->m_manager.SendMsisdnRequestAndGetResponse(inputMsisdn) << std::endl;
}


void nm::cdr::ClientApplication::HandleOperator() {
    std::cout << "Enter MCC: ";
    std::string inputMcc = this->GetUserRequestAsString();
    std::cout << "Enter MNC: ";
    std::string inputMnc = this->GetUserRequestAsString();

    std::cout << this->m_manager.SendOperatorRequestAndGetResponse(inputMcc, inputMnc) << std::endl;
}


void nm::cdr::ClientApplication::HandleClientLinkGraph() {
    std::cout << "Enter MSISDN: ";
    std::string inputMsisdn = this->GetUserRequestAsString();
    std::cout << "Enter second party MSISDN: ";
    std::string inputSecondPartyMsisdn = this->GetUserRequestAsString();

    std::cout << this->m_manager.SendLinkRequestAndGetResponse(inputMsisdn, inputSecondPartyMsisdn) << std::endl;
}