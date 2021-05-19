#include "OutputStreamer.hpp"
#include <cstdio> // printf, scanf
#include <stdexcept> // std::invalid_argument
#include <cctype> // toupper, tolower
#include <cstring> // strlen
#include "EndLine.hpp"
#include "Uppercase.hpp"
#include "Lowercase.hpp"
#include "Normalcase.hpp"


#define UNUSED(x) ((void)(x))


OutputStreamer::OutputStreamer()
: m_cstringPrintingNotation(NORMALCASE){
}


OutputStreamer::OutputStreamer(const OutputStreamer& a_other)
: m_cstringPrintingNotation(a_other.m_cstringPrintingNotation){
}


OutputStreamer& OutputStreamer::operator=(const OutputStreamer& a_other){
    this->m_cstringPrintingNotation = a_other.m_cstringPrintingNotation;

    return *this;
}


OutputStreamer::~OutputStreamer() {
}


OutputStreamer& OutputStreamer::operator<<(const EndLine& a_endl) {
    UNUSED(a_endl);

    putchar('\n');

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const unsigned long& a_unsignedLongNumber) {
    printf("%lu", a_unsignedLongNumber);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const long& a_signedLongNumber) {
    printf("%ld", a_signedLongNumber);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const int& a_signedNumber) {
    printf("%d", a_signedNumber);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const unsigned int& a_unsignedNumber) {
    printf("%u", a_unsignedNumber);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const double& a_floationNumber) {
    printf("%lf", a_floationNumber);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const char& a_char) {
    putchar(a_char);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const char* a_string) {
    if(!a_string) {
        throw std::invalid_argument("Null Pointer Error");
    }

    if(this->m_cstringPrintingNotation == NORMALCASE) {
        printf("%s", a_string);
    }
    else {
        // Buffer creation
        size_t strLengthAndTerminatorLength = strlen(a_string) + 1;
        char* modifyBuffer = new char[strLengthAndTerminatorLength];

        // Buffer initialization
        if(this->m_cstringPrintingNotation == UPPERCASE) {
            for(size_t i = 0; i < strLengthAndTerminatorLength - 1 /* -1 for terminator */; ++i) {
                modifyBuffer[i] = toupper(a_string[i]); // Skip the checking if is an Uppercase already
            }
        }
        else { // this->m_cstringPrintingNotation == LOWERCASE
            for(size_t i = 0; i < strLengthAndTerminatorLength - 1 /* -1 for terminator */; ++i) {
                modifyBuffer[i] = tolower(a_string[i]); // Skip the checking if is a Lowercase already
            }
        }

        modifyBuffer[strLengthAndTerminatorLength - 1] = '\0';

        // Print modified buffer
        printf("%s", modifyBuffer);
        delete[] modifyBuffer;
    }

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const void* a_pointer) {
    if(!a_pointer) {
        throw std::invalid_argument("Null Pointer Error");
    }
    printf("%p", (void*)a_pointer);

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const Uppercase& a_uppercase) {
    this->m_cstringPrintingNotation = UPPERCASE;

    return *this;
}


OutputStreamer& OutputStreamer::operator<<(const Lowercase& a_lowercase) {
    this->m_cstringPrintingNotation = LOWERCASE;

    return *this;
}

OutputStreamer& OutputStreamer::operator<<(const Normalcase& a_normalcase) {
    this->m_cstringPrintingNotation = NORMALCASE;

    return *this;
}