#ifndef __INPUTSTREAMER_HPP__
#define __INPUTSTREAMER_HPP__


#include "EndLine.hpp"


class InputStreamer {
public:

    InputStreamer() = default;
    InputStreamer(const InputStreamer& a_os) = default;
    InputStreamer& operator=(const InputStreamer& a_os) = default;
    ~InputStreamer() = default;

    // Operators
    InputStreamer& operator>>(const EndLine& a_endl); // Indicator for end line
    InputStreamer& operator>>(unsigned long& a_unsignedLongNumber);
    InputStreamer& operator>>(long& a_signedLongNumber);
    InputStreamer& operator>>(unsigned int& a_unsignedNumber);
    InputStreamer& operator>>(int& a_signedNumber);
    InputStreamer& operator>>(double& a_floationNumber);
    InputStreamer& operator>>(char& a_char);
    // InputStreamer& operator>>(char* a_string); // Creating new char* on the heap (delete is required), Throws: std::invalid_argument if pointer is NULL - still not supported (TODO)
};


#endif // __INPUTSTREAMER_HPP__