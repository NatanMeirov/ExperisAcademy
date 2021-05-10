#include "TFraction.hpp"
#include <iostream> // std::ostream, std::cout
#include <stdexcept> // std::logic_error
#include <cmath> // abs, floor


#define BASE_CASE 0.001


template <typename T>
T ZeroDenominatorCheck(T a_denominator) {
    if(a_denominator == 0) { // CHECK IF IS A LEGAL STEP OR NOT (T == 0 ...?) !!!!!!
        throw std::logic_error("Zero Division Error");
    }

    return a_denominator;
}

template <typename T>
T GCD(T a_firstNumber, T a_secondNumber) {
    return (fabs((double)a_secondNumber) < BASE_CASE) ? a_firstNumber : GCD<T>(a_secondNumber, (a_firstNumber - floor((double)a_firstNumber / (double)a_secondNumber) * a_secondNumber));
}


// Specialization for <int> type
template<>
int GCD<int>(int a_firstNumber, int a_secondNumber) {
    return (a_secondNumber == 0) ? a_firstNumber : GCD<int>(a_secondNumber, (a_firstNumber % a_secondNumber));
}


template <typename T>
T FindGreatestCommonDivisor(T a_firstNumber, T a_secondNumber) {
    if(a_firstNumber < a_secondNumber) {
        return GCD<T>(a_secondNumber, a_firstNumber);
    }

    return GCD<T>(a_firstNumber, a_secondNumber);
}


template <typename T>
T LCM(T a_firstNumber, T a_secondNumber) {
    return (a_firstNumber * a_secondNumber) / FindGreatestCommonDivisor<T>(a_firstNumber, a_secondNumber);
}


template <typename T>
T FindCommonDenominatorBase(T a_firstNumber, T a_secondNumber) {
    return LCM<T>(a_firstNumber, a_secondNumber);
}


// Friend Function

template <typename T>
std::ostream& operator<<(std::ostream& a_os, const Fraction<T>& a_fraction) {
    a_os << (a_fraction.m_numerator / a_fraction.m_denominator);

    return a_os;
}


template <typename T>
Fraction<T> operator*(const int& a_number, const Fraction<T>& a_fraction) {
    return a_fraction * a_number;
}


template <typename T>
Fraction<T> operator*(const double& a_floationNumber, const Fraction<T>& a_fraction) {
    return a_fraction * a_floationNumber;
}


template <typename T>
Fraction<T> operator+(const int& a_number, const Fraction<T>& a_fraction) {
    return a_fraction + a_number;
}


template <typename T>
Fraction<T> operator+(const double& a_floationNumber, const Fraction<T>& a_fraction) {
    return a_fraction + a_floationNumber;
}


template <typename T>
Fraction<T> operator-(const int& a_number, const Fraction<T>& a_fraction) {
    return (-a_fraction) + a_number;
}


template <typename T>
Fraction<T> operator-(const double& a_floationNumber, const Fraction<T>& a_fraction) {
    return (-a_fraction) + a_floationNumber;
}


// Methods

template <typename T>
Fraction<T>::Fraction(const T& a_numerator, const T& a_denominator)
: m_numerator(a_numerator)
, m_denominator(ZeroDenominatorCheck<T>(a_denominator)){
}


template <typename T>
Fraction<T>::Fraction(const Fraction<T>& a_other)
: m_numerator(a_other.m_numerator)
, m_denominator(a_other.m_denominator) {
}


template <typename T>
Fraction<T>& Fraction<T>::operator=(const Fraction<T>& a_other){
    this->m_numerator = a_other.m_numerator;
    this->m_denominator = a_other.m_denominator;
}


template <typename T>
Fraction<T>::~Fraction() {
}


template <typename T>
void Fraction<T>::Print() const {
    std::cout << *this;
}


template <typename T>
Fraction<T> Fraction<T>::operator+(const Fraction<T>& a_other) const {
    T commonDivisor = FindCommonDenominatorBase(this->m_denominator, a_other.m_denominator);
    // TODO
}


template <typename T>
Fraction<T> Fraction<T>::operator-(const Fraction<T>& a_other) const {
    // TODO
}


template <typename T>
Fraction<T> Fraction<T>::operator+(const int& a_number) const {
    return *this + Fraction<T>(a_number, 1);
}


template <typename T>
Fraction<T> Fraction<T>::operator+(const double& a_floationNumber) const {
    return *this + Fraction<T>(a_floationNumber, 1);
}


template <typename T>
Fraction<T> Fraction<T>::operator-(const int& a_number) const {
    return *this - Fraction<T>(a_number, 1);
}


template <typename T>
Fraction<T> Fraction<T>::operator-(const double& a_floationNumber) const {
    return *this + Fraction<T>(a_floationNumber, 1);
}


template <typename T>
Fraction<T>& Fraction<T>::operator+=(const Fraction<T>& a_other) {
    return *this = this + a_other;
}


template <typename T>
Fraction<T>& Fraction<T>::operator-=(const Fraction<T>& a_other) {
    return *this = this - a_other;
}


template <typename T>
Fraction<T> Fraction<T>::operator*(const Fraction<T>& a_other) const {
    return Fraction<T>((this->m_numerator * a_other.m_numerator), (this->m_denominator * a_other.m_denominator));
}


template <typename T>
Fraction<T> Fraction<T>::operator*(const int& a_number) const {
    return Fraction<T>((this->m_numerator * a_number), this->m_denominator);
}


template <typename T>
Fraction<T> Fraction<T>::operator*(const double& a_floationNumber) const {
    return Fraction<T>((this->m_numerator * a_floationNumber), this->m_denominator);
}


// Expensive way:
// template <typename T>
// Fraction<T> Fraction<T>::operator/(const Fraction<T>& a_other) const {
//     return *this * Fraction<T>(a_other.m_denominator, a_other.m_numerator);
// }
template <typename T>
Fraction<T> Fraction<T>::operator/(const Fraction<T>& a_other) const {
    return Fraction<T>((this->m_numerator * a_other.m_denominator), (this->m_denominator * a_other.m_numerator));
}


template <typename T>
Fraction<T> Fraction<T>::operator-() const {
    return Fraction<T>(-(this->m_numerator), this->m_denominator);
}


// Returns a casting to double type
template <typename T>
Fraction<T>::operator double() {
    return ((double)this->m_numerator / (double)this->m_denominator);
}