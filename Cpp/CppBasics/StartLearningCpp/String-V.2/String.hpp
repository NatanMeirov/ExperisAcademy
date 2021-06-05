#ifndef __STRING_HPP__
#define __STRING_HPP__

#include <cstddef> // size_t

class String {
public:

    String();
    String(const char* a_str);
    String(const String& a_other);
    String& operator=(const String& a_other);
    ~String();

    // Methods
    String& operator+=(const String& a_str);

    // Const Methods
    const size_t Length() const;
    void Print() const;
    bool IsEqual(const char* a_str) const;
    bool operator==(const char* a_str) const;
    bool operator==(const String& a_other) const;
    bool operator!=(const String& a_other) const;
    bool operator<(const String& a_other) const;
    bool operator>(const String& a_other) const;
    bool operator<=(const String& a_other) const;
    bool operator>=(const String& a_other) const;
    String operator+(const String& a_str) const;
    String operator-(const String& a_other) const; // Removes the first occurrence of a_other String from the "self" String
    String operator<<(const size_t a_numOfShifts) const; // Removes the number of shift value from the String (from left String's side)
    String operator>>(const size_t a_numOfShifts) const; // Removes the number of shift value from the String (from right String's side)
    char& operator[](int a_index) const;
    char& operator[](int a_index);

    // Additional Weird Operators:


    String& operator++();
    String operator++(int);
    String operator-() const;
    String operator+() const;
    String operator!() const;

private:

    // Members
    size_t m_strLength; // Order is required
    char* m_str;
};

#endif // __STRING_HPP__