#include "String.hpp"
#include <cstddef> // size_t
#include <iostream> // std::cout, std::endl
#include <cstring> // strlen, memcpy
#include <stdexcept> // std::invalid_argument, std::out_of_range


static size_t SafeStrlenWrapper(const char* a_str) {
    if(!a_str) {
        throw std::invalid_argument("Null Pointer Error");
    }

    return strlen(a_str);
}


String::String()
: m_strLength(0)
, m_str(new char[1]) {
    this->m_str[0] = '\0';
}


String::String(const char* a_str)
: m_strLength(SafeStrlenWrapper(a_str))
, m_str(new char[this->m_strLength + 1]) /* +1 for '\0' */ {
    memcpy(this->m_str, a_str, this->m_strLength + 1);
}


String::String(const String& a_other)
: m_strLength(a_other.m_strLength)
, m_str(new char[this->m_strLength + 1]) /* +1 for '\0' */ {
    memcpy(this->m_str, a_other.m_str, this->m_strLength + 1);
}


String& String::operator=(const String& a_other) {
    if (this == &a_other) {
        return *this;
    }

    delete[] this->m_str;
    this->m_strLength = a_other.m_strLength;
    this->m_str = new char[this->m_strLength + 1]; /* +1 for '\0' */
    memcpy(this->m_str, a_other.m_str, this->m_strLength + 1);

    return *this;
}


String::~String() {
    delete[] this->m_str;
}


String& String::operator+=(const String& a_other) {
    return *this = this->operator+(a_other);
}


const size_t String::Length() const {
    return this->m_strLength;
}


void String::Print() const {
    std::cout << this->m_str;
}


bool String::IsEqual(const char* a_str) const {
    if(!a_str) {
        throw std::invalid_argument("Null Pointer Error");
    }

    return strcmp(this->m_str, a_str) == 0;
}


bool String::operator==(const char* a_str) const {
    return this->IsEqual(a_str);
}


String String::operator+(const String& a_other) const {
    char* newStrPtr = new char[this->m_strLength + a_other.m_strLength + 1]; // +1 for '\0'
    memcpy(newStrPtr, this->m_str, this->m_strLength); // more efficient then strcpy
    memcpy(newStrPtr + this->m_strLength, a_other.m_str, (a_other.m_strLength + 1)); // more efficient then strcat

    return String(newStrPtr);
}


char& String::operator[](const size_t a_index) const {
    if(a_index > this->m_strLength - 1) { // size_t cannot be negative
        throw std::out_of_range("Index Out Of Bounds Error");
    }

    return this->m_str[a_index];
}