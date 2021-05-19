#ifndef __TFRACTION_HPP__
#define __TFRACTION_HPP__


#include <ostream> // std::ostream


// Supports data types: Fraction<int>, Fraction<double>, Fraction<Fraction<int>>, Fraction<Fraction<double>>
template <typename T>
class Fraction {
public:

    template <typename K>
    friend std::ostream& operator<<(std::ostream& a_os, const Fraction<K>& a_fraction); // Prints the number in floating point "style"

    // Preventing default C'tor (no sense)
    explicit Fraction(const T& a_numerator, const T& a_denominator);
    Fraction(const Fraction<T>& a_other);
    Fraction<T>& operator=(const Fraction<T>& a_other);
    ~Fraction();

    // Template C'tor and Copy-Assignment
    // template <typename K>
    // Fraction<T>(const Fraction<K> &a_other);

    // template <typename K>
    // Fraction<T>& operator=(const Fraction<K> &a_other);

    // Const Methods
    void Print() const; // Prints the number in fraction "style" (example: "x/y")


    // Operator Overloading
    Fraction<T> operator+(const Fraction<T>& a_other) const;
    Fraction<T> operator-(const Fraction<T>& a_other) const;
    Fraction<T>& operator+=(const Fraction<T>& a_other);
    Fraction<T>& operator-=(const Fraction<T>& a_other);
    Fraction<T>& operator*=(const Fraction<T>& a_other);
    Fraction<T>& operator/=(const Fraction<T>& a_other);
    // Fraction<T> operator+(const int& a_number) const;
    // Fraction<T> operator+(const double& a_floationNumber) const;
    // Fraction<T> operator-(const int& a_number) const;
    // Fraction<T> operator-(const double& a_floationNumber) const;
    Fraction<T> operator*(const Fraction<T>& a_other) const;
    Fraction<T> operator*(const int& a_number) const;
    Fraction<T> operator*(const double& a_floationNumber) const;
    Fraction<T> operator/(const Fraction<T>& a_other) const;
    Fraction<T> operator/(const int& a_number) const;
    Fraction<T> operator/(const double& a_floationNumber) const;
    Fraction<T> operator-() const;
    explicit operator double(); // Casting to double


private:

    // Members
    T m_numerator;
    T m_denominator;
};


#endif // __TFRACTION_HPP__