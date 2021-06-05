#ifndef __OUTPUTSTREAMER_HPP__
#define __OUTPUTSTREAMER_HPP__


#include "EndLine.hpp"
#include "Uppercase.hpp"
#include "Lowercase.hpp"
#include "Normalcase.hpp"


class OutputStreamer {
public:

    enum CStringPrintingNotation{ NORMALCASE = 0, UPPERCASE, LOWERCASE };

    OutputStreamer();
    OutputStreamer(const OutputStreamer& a_other);
    OutputStreamer& operator=(const OutputStreamer& a_other);
    ~OutputStreamer();

    // Operators
    OutputStreamer& operator<<(const EndLine& a_endl); // Indicator for end line
    OutputStreamer& operator<<(const unsigned long& a_unsignedLongNumber);
    OutputStreamer& operator<<(const long& a_signedLongNumber);
    OutputStreamer& operator<<(const int& a_signedNumber);
    OutputStreamer& operator<<(const unsigned int& a_unsignedNumber);
    OutputStreamer& operator<<(const double& a_floationNumber);
    OutputStreamer& operator<<(const char& a_char);
    OutputStreamer& operator<<(const char* a_string); // Throws: std::invalid_argument if pointer is NULL
    OutputStreamer& operator<<(const void* a_pointer); // Pointer must be cast to void* before calling the << operator, Throws: std::invalid_argument if pointer is NULL, // Default cstring printing value: Normalcase
    OutputStreamer& operator<<(const Uppercase& a_uppercase); // Indicator for uppercase for c-strings
    OutputStreamer& operator<<(const Lowercase& a_lowercase); // Indicator for lowercase for c-strings
    OutputStreamer& operator<<(const Normalcase& a_normalcase); // Indicator for normalcase for c-strings

private:

    CStringPrintingNotation m_cstringPrintingNotation; // Default value: NORMALCASE
};


#endif // __OUTPUTSTREAMER_HPP__