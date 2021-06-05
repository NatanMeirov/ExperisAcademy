#ifndef __SCREENUIMANAGER_HPP__
#define __SCREENUIMANAGER_HPP__


#include <cstddef> // size_t


class ScreenUIManager {
public:

    // Methods
    void ShowMenuMessage(const char* a_menuMessage, const bool a_useEndLine) const; // Menu format message - a_menuMessage should include the inner menu message
    void ShowMessage(const char* a_message, const bool a_useEndLine) const; // Regular message
    void ShowErrorMessage(const char* a_errorMessage, const bool a_useEndLine) const; // Red error message
    void ShowWarningMessage(const char* a_warningMessage, const bool a_useEndLine) const; // Yellow warning message
    void ShowInfoMessage(const char* a_infoMessage, const bool a_useEndLine) const; // Blue info message
    void GetInputAsString(char* a_bufferToFill, const size_t a_bufferLimit) const;
    void GetInputAsNumber(long& a_number) const; // Validate that the input is a number (throws std::invalid_argument exception)
    void GetInputAsFloatingNumber(double& a_floatingNumber) const; // Validate that the input is a number (throws std::invalid_argument exception)

protected:

    // Defines
    static const size_t MAX_BUFFER_LENGTH = 4096;
};


#endif // __SCREENUIMANAGER_HPP__