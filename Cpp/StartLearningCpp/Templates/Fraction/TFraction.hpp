#ifndef __TFRACTION_HPP__
#define __TFRACTION_HPP__


#include <ostream> // std::ostream


template <typename T>
class Fraction {
public:

    friend std::ostream& operator<<(std::ostream& a_os, const Fraction<T>& a_fraction);
    friend Fraction<T> operator*(const int& a_number, const Fraction<T>& a_fraction);
    friend Fraction<T> operator*(const double& a_number, const Fraction<T>& a_fraction);
    friend Fraction<T> operator+(const int& a_number, const Fraction<T>& a_fraction);
    friend Fraction<T> operator+(const double& a_number, const Fraction<T>& a_fraction);
    friend Fraction<T> operator-(const int& a_number, const Fraction<T>& a_fraction);
    friend Fraction<T> operator-(const double& a_number, const Fraction<T>& a_fraction);


    // Preventing default c'tor (no sense)
    Fraction(const T& a_numerator, const T& a_denominator);
    Fraction(const Fraction<T>& a_other);
    Fraction& operator=(const Fraction<T>& a_other);
    ~Fraction();


    void Print() const;


    // Operator Overloading
    Fraction<T> operator+(const Fraction<T>& a_other) const;
    Fraction<T> operator-(const Fraction<T>& a_other) const;
    Fraction<T>& operator+=(const Fraction<T>& a_other);
    Fraction<T>& operator-=(const Fraction<T>& a_other);
    Fraction<T> operator+(const int& a_number) const;
    Fraction<T> operator+(const double& a_floationNumber) const;
    Fraction<T> operator-(const int& a_number) const;
    Fraction<T> operator-(const double& a_floationNumber) const;
    Fraction<T> operator*(const Fraction<T>& a_other) const;
    Fraction<T> operator*(const int& a_number) const;
    Fraction<T> operator*(const double& a_floationNumber) const;
    Fraction<T> operator/(const Fraction<T>& a_other) const;
    Fraction<T> operator-() const;
    operator double(); // Casting to double

private:

    // Members
    T m_numerator;
    T m_denominator;
};


#endif // __TFRACTION_HPP__