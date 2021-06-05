#include "TFraction.hpp"
#include <iostream> // std::ostream, std::cout
#include <stdexcept> // std::logic_error
#include <cmath> // abs, floor


#define BASE_CASE 0.001


// Helper Template Functions


template <typename T>
T ZeroDenominatorCheck(T a_denominator) {
    if((double)a_denominator == 0) {
        throw std::logic_error("Math Error: Zero Division");
    }

    return a_denominator;
}

template <typename T>
T GCD(T a_firstNumber, T a_secondNumber) {
    return (fabs((double)a_secondNumber) < BASE_CASE) ? a_firstNumber : GCD<T>(a_secondNumber, (a_firstNumber - floor((double)a_firstNumber / (double)a_secondNumber) * a_secondNumber));
}


// Specialization for <int> type
template<>
inline int GCD<int>(int a_firstNumber, int a_secondNumber) {
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


// Friend Template Functions

template <typename K>
std::ostream& operator<<(std::ostream& a_os, const Fraction<K>& a_fraction) {
    a_os << ((double)(a_fraction.m_numerator) / (double)(a_fraction.m_denominator));

    return a_os;
}


// Global Template Functions

template <typename K>
Fraction<K> operator*(const int& a_number, const Fraction<K>& a_fraction) {
    return a_fraction * a_number;
}


template <typename K>
Fraction<K> operator*(const double& a_floationNumber, const Fraction<K>& a_fraction) {
    return a_fraction * a_floationNumber;
}


// template <typename K>
// Fraction<K> operator+(const int& a_number, const Fraction<K>& a_fraction) {
//     return a_fraction + a_number; // BUG - the used operator+ cannot handle Fraction<Fraction<T>> + int/double
// }


// template <typename K>
// Fraction<K> operator+(const double& a_floationNumber, const Fraction<K>& a_fraction) {
//     return a_fraction + a_floationNumber; // BUG - the used operator+ cannot handle Fraction<Fraction<T>> + int/double
// }


// template <typename K>
// Fraction<K> operator-(const int& a_number, const Fraction<K>& a_fraction) {
//     return (-a_fraction) + a_number; // BUG - the used operator+ cannot handle Fraction<Fraction<T>> - int/double
// }


// template <typename K>
// Fraction<K> operator-(const double& a_floationNumber, const Fraction<K>& a_fraction) {
//     return (-a_fraction) + a_floationNumber; // BUG - the used operator+ cannot handle Fraction<Fraction<T>> - int/double
// }


template <typename K>
Fraction<K> operator/(const int& a_number, const Fraction<K>& a_fraction) {
    return Fraction<K>((a_number * a_fraction.m_denominator), (1 * a_fraction.m_numerator));
}


template <typename K>
Fraction<K> operator/(const double& a_floationNumber, const Fraction<K>& a_fraction) {
    return Fraction<K>((a_floationNumber * a_fraction.m_denominator), (1.0 * a_fraction.m_numerator));
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

    return *this;
}


template <typename T>
Fraction<T>::~Fraction() {
}


// template <typename T>
// template <typename K>
// Fraction<T>::Fraction<K>(const Fraction<K> &a_other);

// template <typename K>
// template <typename T>
// Fraction<T>& Fraction<T>::operator=(const Fraction<K> &a_other);


template <typename T>
void Fraction<T>::Print() const {
    std::cout << (double)this->m_numerator << "/" << (double)this->m_denominator;
}


// Specialization for <int> type
template <>
inline void Fraction<int>::Print() const {
    std::cout << this->m_numerator << "/" << this->m_denominator;
}


template <typename T>
Fraction<T> Fraction<T>::operator+(const Fraction<T>& a_other) const {
    T commonDivisor = FindCommonDenominatorBase(this->m_denominator, a_other.m_denominator);

    T thisFractionMultiplyValue = commonDivisor / this->m_denominator;
    T otherFractionMultiplyValue = commonDivisor / a_other.m_denominator;

    // Fraction<T> newLeftFraction((this->m_numerator * thisFractionMultiplyValue), commonDivisor);
    // Fraction<T> newRightFraction((a_other.m_numerator * otherFractionMultiplyValue), commonDivisor);

    T totalNewNumeratorValue = (this->m_numerator * thisFractionMultiplyValue) + (a_other.m_numerator * otherFractionMultiplyValue);

    return Fraction<T>(totalNewNumeratorValue, commonDivisor);
}


template <typename T>
Fraction<T> Fraction<T>::operator-(const Fraction<T>& a_other) const {
    return (-a_other) + *this;
}


// template <typename T>
// Fraction<T> Fraction<T>::operator+(const int& a_number) const {
//     return *this + Fraction<T>(a_number, 1); // BUG - cannot handle Fraction<Fraction<T>> - int/double
// }


// template <typename T>
// Fraction<T> Fraction<T>::operator+(const double& a_floationNumber) const {
//     return *this + Fraction<T>(a_floationNumber, 1.0); // BUG - cannot handle Fraction<Fraction<T>> - int/double
// }


// template <typename T>
// Fraction<T> Fraction<T>::operator-(const int& a_number) const {
//     return *this - Fraction<T>(a_number, 1);// BUG - cannot handle Fraction<Fraction<T>> - int/double
// }


// template <typename T>
// Fraction<T> Fraction<T>::operator-(const double& a_floationNumber) const {
//     return *this - Fraction<T>(a_floationNumber, 1.0); // BUG - cannot handle Fraction<Fraction<T>> - int/double
// }


template <typename T>
Fraction<T>& Fraction<T>::operator+=(const Fraction<T>& a_other) {
    return *this = *this + a_other;
}


template <typename T>
Fraction<T>& Fraction<T>::operator-=(const Fraction<T>& a_other) {
    return *this = *this - a_other;
}


template <typename T>
Fraction<T>& Fraction<T>::operator*=(const Fraction<T>& a_other) {
    return *this = *this * a_other;
}


template <typename T>
Fraction<T>& Fraction<T>::operator/=(const Fraction<T>& a_other) {
    return *this = *this / a_other;
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
Fraction<T> Fraction<T>::operator/(const int& a_number) const {
    ZeroDenominatorCheck(a_number);

    return Fraction<T>((this->m_numerator * 1), (this->m_denominator * a_number));
}


template <typename T>
Fraction<T> Fraction<T>::operator/(const double& a_floationNumber) const {
    ZeroDenominatorCheck(a_floationNumber);

    return Fraction<T>((this->m_numerator * 1.0), (this->m_denominator * a_floationNumber));
}


template <typename T>
Fraction<T> Fraction<T>::operator-() const {
    return Fraction<T>(-(this->m_numerator), this->m_denominator);
}


// Returns a cast to double type
template <typename T>
Fraction<T>::operator double() {
    return (double)((double)this->m_numerator) / ((double)this->m_denominator);
}