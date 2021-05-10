#include "String.hpp"
#include <cstddef> // size_t
#include <iostream> // std::cout, std::endl
#include <cstring> // strlen, memcpy
#include <stdexcept> // std::invalid_argument, std::out_of_range
#include <cctype> // islower, toupper, tolower


static size_t SafeStrlenWrapper(const char* a_str) {
    if(!a_str) {
        throw std::invalid_argument("Null Pointer Error");
    }

    return strlen(a_str);
}


static char* CreateAndDuplicateString(const char* a_sourceString, const size_t a_sourceStringLength) {
    char* str = new char[a_sourceStringLength + 1]; // +1 for '\0'
    memcpy(str, a_sourceString, a_sourceStringLength + 1); // +1 to copy '\0' as well

    return str;
}


static char* InitializeNewEmptyString() {
    char* str = new char[1];
    str[0] = '\0';

    return str;
}


static char* CreateStringAndSetSingleCharAllOverIt(const char a_characterToSet, const size_t a_lengthOfNewString) {
    char* str = new char[a_lengthOfNewString + 1]; // +1 for '\0'
    memset(str, a_characterToSet, a_lengthOfNewString);
    str[a_lengthOfNewString] = '\0';

    return str;
}


String::String()
: m_strLength(0)
, m_str(InitializeNewEmptyString()) {
}


String::String(const char* a_str)
: m_strLength(SafeStrlenWrapper(a_str))
, m_str(CreateAndDuplicateString(a_str, this->m_strLength)) {
}


String::String(const String& a_other)
: m_strLength(a_other.m_strLength)
, m_str(CreateAndDuplicateString(a_other.m_str, this->m_strLength)) /* +1 for '\0' */ {
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


bool String::operator==(const String& a_other) const {
    return strcmp(this->m_str, a_other.m_str) == 0;
}


bool String::operator!=(const String& a_other) const {
    return strcmp(this->m_str, a_other.m_str) != 0;
}


bool String::operator<(const String& a_other) const {
    return strcmp(this->m_str, a_other.m_str) < 0;
}


bool String::operator>(const String& a_other) const {
    return strcmp(this->m_str, a_other.m_str) > 0;
}


bool String::operator<=(const String& a_other) const {
    return strcmp(this->m_str, a_other.m_str) <= 0;
}


bool String::operator>=(const String& a_other) const {
    return strcmp(this->m_str, a_other.m_str) >= 0;
}


String String::operator+(const String& a_other) const {
    char* newStrPtr = new char[this->m_strLength + a_other.m_strLength + 1]; // +1 for '\0'
    memcpy(newStrPtr, this->m_str, this->m_strLength); // more efficient then strcpy
    memcpy(newStrPtr + this->m_strLength, a_other.m_str, (a_other.m_strLength + 1)); // more efficient then strcat

    String strToReturn(newStrPtr);
    delete[] newStrPtr;

    return strToReturn;
}


char& String::operator[](int a_index) const {
    if(a_index > this->m_strLength - 1 || a_index < 0) {
        throw std::out_of_range("Index Out Of Bounds Error");
    }

    return this->m_str[a_index];
}


// Weird operators:

String String::operator-(const String& a_other) const {
    char* cursor = strstr(this->m_str, a_other.m_str);
    if(!cursor) {
        return *this; // Returns the current String without removing the a_other String's letters
    }

    size_t newStrLength = this->m_strLength - a_other.m_strLength;
    char* newStr = new char[newStrLength + 1];

    size_t bytesToCopy = cursor - this->m_str; // Will get the difference between the 2 addresses - the start of the string and the start of the sub-string inside it
    memcpy(newStr, this->m_str, bytesToCopy);

    cursor += a_other.m_strLength;
    size_t remainingBytesToCopy = (this->m_str + this->m_strLength - 1) - cursor;   // To copy the rest of the bytes
    memcpy(newStr + bytesToCopy, cursor, remainingBytesToCopy + 1); // +1 for '\0'

    String strToReturn(newStr);
    delete[] newStr;

    return strToReturn;
}


String& String::operator++() {
    char* newStr = new char[this->m_strLength + 2]; // +2 for '!' and for '\0'
    memcpy(newStr, this->m_str, this->m_strLength);
    newStr[this->m_strLength++] = '!';
    newStr[this->m_strLength] = '\0';

    *this = String(newStr);
    delete[] newStr;

    return *this;
}


String String::operator++(int) {
    String oldString(*this);
    this->operator++();

    return oldString;
}


String String::operator<<(const size_t a_numOfShifts) const {
    if(this->m_strLength < a_numOfShifts) {
        return String(); // An empty new String
    }

    char* newStr = new char[(this->m_strLength - a_numOfShifts) + 1];
    memcpy(newStr, this->m_str + a_numOfShifts, (this->m_strLength - a_numOfShifts) + 1); // +1 for '\0'

    String strToReturn(newStr);
    delete[] newStr;

    return strToReturn;
}


String String::operator>>(const size_t a_numOfShifts) const {
    if(this->m_strLength < a_numOfShifts) {
        return String(); // An empty new String
    }

    char* newStr = new char[(this->m_strLength - a_numOfShifts) + 1];
    memcpy(newStr, this->m_str, (this->m_strLength - a_numOfShifts));
    newStr[this->m_strLength - a_numOfShifts] = '\0';

    String strToReturn(newStr);
    delete [] newStr;

    return strToReturn;
}


String String::operator+() const {
    char* newStr = CreateStringAndSetSingleCharAllOverIt('+', this->m_strLength);

    String strToReturn(newStr);
    delete[] newStr;

    return strToReturn;
}


String String::operator-() const {
    char* newStr = CreateStringAndSetSingleCharAllOverIt('-', this->m_strLength);

    String strToReturn(newStr);
    delete[] newStr;

    return strToReturn;
}


String String::operator!() const {
    char* newStr = new char[this->m_strLength + 1];

    for(size_t i = 0; i < this->m_strLength; ++i) {
        if(islower(this->m_str[i])) {
            newStr[i] = toupper(this->m_str[i]);
        }
        else { // Else - is upper
            newStr[i] = tolower(this->m_str[i]);
        }
    }

    String stringToReturn(newStr);
    delete[] newStr;

    return stringToReturn;
}