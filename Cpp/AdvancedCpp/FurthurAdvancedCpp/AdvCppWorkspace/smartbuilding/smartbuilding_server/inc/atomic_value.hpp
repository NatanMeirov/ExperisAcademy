#ifndef NM_ATOMIC_VALUE_HPP
#define NM_ATOMIC_VALUE_HPP



namespace advcpp
{

/**
 * An atomic value class, that does operations on value of type T (protected by supported compiler atomic operations (for sync) - that uses the CPU)
 * The atomic value SHOULD be used as a global value shared for all the threads that want to read/write that value of type T
 * Concept of T: T MUST be a numeric type
 */
template <typename T>
class AtomicValue
{
public:
    AtomicValue();
    explicit AtomicValue(T a_startingValue);
    AtomicValue(const AtomicValue& a_other) = delete; // An atomic value SHOULD be passed by reference, and NOT by copy
    AtomicValue& operator=(const AtomicValue& a_other) = delete; // An atomic value SHOULD be passed by reference, and NOT by copy
    ~AtomicValue() = default;

    void operator=(T a_otherValue);
    T operator++();
    T operator--();
    T operator++(int);
    T operator--(int);
    T operator+=(T a_valueToAdd);
    T operator-=(T a_valueToSubtract);

    // Setters
    void Set(T a_value);

    // Conditional Set - to set a new value ONLY if the conditional value matches the current atomic value
    // Set new value, if the previous value is a_conditionalValue
    // Returns true if the condition has matched successfully (and it perform set operation), else false
    bool SetIf(T a_conditionalValue, T a_newValue);

    // Getters
    T Get() const;
    operator T() const;

    T Add(T a_valueToAdd);
    T Sub(T a_valueToSubtract);

private:
    mutable T m_atomicValue;
};


// A specialization of AtomicValue as a bool flag
// The boolean atomic value SHOULD be used as a global value shared for all the threads that want to read/write that flag
template <>
class AtomicValue<bool>
{
    using Byte = unsigned char;

public:
    explicit AtomicValue(bool a_startingValue = false);
    AtomicValue(const AtomicValue<bool>& a_other) = delete; // An atomic value SHOULD be passed by reference, and NOT by copy
    AtomicValue<bool>& operator=(const AtomicValue<bool>& a_other) = delete; // An atomic value SHOULD be passed by reference, and NOT by copy
    ~AtomicValue() = default;

    // Setters
    void operator=(bool a_otherValue);
    void Set(bool a_otherValue);

    // To explicitly set a specific bool value
    void True();
    void False();

    // Conditional setters (Returns true or false if the previous value matches)
    // Set new value, if the previous value is a_conditionalValue
    // Returns true if the condition has matched successfully (and it perform set operation), else false
    bool SetIf(bool a_conditionalValue, bool a_newValue);

    // Getters
    bool Check() const;
    operator bool() const;

private:
    mutable Byte m_atomicBool;
};


using AtomicFlag = AtomicValue<bool>;

} // advcpp


#include "inl/atomic_value.hxx"


#endif // NM_ATOMIC_VALUE_HPP
