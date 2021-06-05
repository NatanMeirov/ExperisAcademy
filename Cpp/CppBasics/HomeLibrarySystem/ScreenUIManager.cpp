#include "ScreenUIManager.hpp"
#include <cstddef> // size_t
#include <iostream> // std::cout, std::endl
#include <cstdlib> // strtol, strtod
#include <cstring> // strlen
#include <stdexcept> // std::invalid_argument
#include <cstdio> // fgets, stdin


void ScreenUIManager::ShowMenuMessage(const char *a_menuMessage, const bool a_useEndLine) const {
    std::cout << "*************************************************************" << std::endl;
    std::cout << "*                          Menu:                            *" << std::endl;
    std::cout << "*************************************************************" << std::endl;
    std::cout << a_menuMessage;
    if(a_useEndLine) {
        std::cout << std::endl;
    }
    std::cout << "*************************************************************" << std::endl;
}


void ScreenUIManager::ShowMessage(const char *a_message, const bool a_useEndLine) const {
    std::cout << a_message;

    if(a_useEndLine) {
        std::cout << std::endl;
    }
}


void ScreenUIManager::ShowErrorMessage(const char *a_errorMessage, const bool a_useEndLine) const {
    // TODO
}


void ScreenUIManager::ShowWarningMessage(const char *a_errorMessage, const bool a_useEndLine) const {
    // TODO
}


void ScreenUIManager::ShowInfoMessage(const char *a_errorMessage, const bool a_useEndLine) const {
    // TODO
}


void ScreenUIManager::GetInputAsString(char* a_bufferToFill, const size_t a_bufferLimit) const {
    fflush(stdin);
    fgets(a_bufferToFill, a_bufferLimit, stdin);
    fflush(stdin);

    size_t lastCharLocation = strlen(a_bufferToFill) - 1;
    if(a_bufferToFill[lastCharLocation] == '\n') {
        a_bufferToFill[lastCharLocation] = '\0';
    }
}


void ScreenUIManager::GetInputAsNumber(long& a_number) const {
    char buffer[ScreenUIManager::MAX_BUFFER_LENGTH];
    fflush(stdin);
    fgets(buffer, ScreenUIManager::MAX_BUFFER_LENGTH, stdin);
    fflush(stdin);

    a_number = strtol(buffer, NULL, 10); // 10 stands for base
    if(a_number == 0 && buffer[0] != '0') { // Because if buffer[0] == '0' -> it is a valid input
        throw std::invalid_argument("Wrong Input Error");
    }
}


void ScreenUIManager::GetInputAsFloatingNumber(double& a_floatingNumber) const {
    char buffer[ScreenUIManager::MAX_BUFFER_LENGTH];
    fflush(stdin);
    fgets(buffer, ScreenUIManager::MAX_BUFFER_LENGTH, stdin);
    fflush(stdin);

    a_floatingNumber = strtod(buffer, NULL);
    if(a_floatingNumber == 0 && buffer[0] != '0') { // Because if buffer[0] == '0' -> it is a valid input
        throw std::invalid_argument("Wrong Input Error");
    }
}