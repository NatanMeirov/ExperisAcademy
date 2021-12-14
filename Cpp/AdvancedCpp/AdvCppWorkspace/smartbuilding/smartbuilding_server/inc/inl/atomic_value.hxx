#ifndef NM_ATOMIC_VALUE_HXX
#define NM_ATOMIC_VALUE_HXX


#include <type_traits> // std::is_integral


namespace advcpp
{

template <typename T>
AtomicValue<T>::AtomicValue()
: m_atomicValue() // The default value of type T (usually 0)
{
    static_assert(std::is_integral<T>::value, "T must be of an integral type");
}


template <typename T>
AtomicValue<T>::AtomicValue(T a_startingValue)
{
    static_assert(std::is_integral<T>::value, "T must be of an integral type");
    Set(a_startingValue);
}


template <typename T>
void AtomicValue<T>::operator=(T a_otherValue)
{
    Set(a_otherValue);
}


template <typename T>
T AtomicValue<T>::operator++()
{
    return Add(1);
}


template <typename T>
T AtomicValue<T>::operator--()
{
    return Sub(1);
}


template <typename T>
T AtomicValue<T>::operator++(int)
{
    return __sync_fetch_and_add(&m_atomicValue, 1);
}


template <typename T>
T AtomicValue<T>::operator--(int)
{
    return __sync_fetch_and_sub(&m_atomicValue, 1);
}


template <typename T>
T AtomicValue<T>::operator+=(T a_valueToAdd)
{
    return Add(a_valueToAdd);
}


template <typename T>
T AtomicValue<T>::operator-=(T a_valueToSubtract)
{
    return Sub(a_valueToSubtract);
}


template <typename T>
AtomicValue<T>::operator T() const
{
    return Get();
}


template <typename T>
void AtomicValue<T>::Set(T a_value)
{
    while(!(__sync_bool_compare_and_swap(&m_atomicValue, m_atomicValue, a_value))); // Polling - to make sure that the new value has stored successfully (that way is multithreaded-safe)
}


template <typename T>
T AtomicValue<T>::Get() const
{
    return __sync_add_and_fetch(&m_atomicValue, T()); // Not modifing m_atomicValue at all (adding default value (usually 0) to it)
}


template <typename T>
bool AtomicValue<T>::SetIf(T a_conditionalValue, T a_newValue)
{
    return __sync_bool_compare_and_swap(&m_atomicValue, a_conditionalValue, a_newValue);
}


template <typename T>
T AtomicValue<T>::Add(T a_valueToAdd)
{
    return __sync_add_and_fetch(&m_atomicValue, a_valueToAdd);
}


template <typename T>
T AtomicValue<T>::Sub(T a_valueToSubtract)
{
    return __sync_sub_and_fetch(&m_atomicValue, a_valueToSubtract);
}


// Atomic Flag:

inline AtomicValue<bool>::AtomicValue(bool a_startingValue)
{
    Set(a_startingValue);
}


inline AtomicValue<bool>::operator bool() const
{
    return Check();
}


inline void AtomicValue<bool>::operator=(bool a_otherValue)
{
    Set(a_otherValue);
}


inline void AtomicValue<bool>::Set(bool a_otherValue)
{
    while(!(__sync_bool_compare_and_swap(&m_atomicBool, m_atomicBool, Byte(a_otherValue)))); // Polling - to make sure that the new value has stored successfully (that way is multithreaded-safe)
}


inline bool AtomicValue<bool>::SetIf(bool a_conditionalValue, bool a_newValue)
{
    return __sync_bool_compare_and_swap(&m_atomicBool, Byte(a_conditionalValue), Byte(a_newValue));
}


inline void AtomicValue<bool>::True()
{
    while(!(__sync_bool_compare_and_swap(&m_atomicBool, m_atomicBool, 1))); // Polling - to make sure that the new value has stored successfully (that way is multithreaded-safe)
}


inline void AtomicValue<bool>::False()
{
    while(!(__sync_bool_compare_and_swap(&m_atomicBool, m_atomicBool, 0))); // Polling - to make sure that the new value has stored successfully (that way is multithreaded-safe)
}


inline bool AtomicValue<bool>::Check() const
{
    return __sync_or_and_fetch(&m_atomicBool, 0);
}

} // advcpp


#endif // NM_ATOMIC_VALUE_HXX
