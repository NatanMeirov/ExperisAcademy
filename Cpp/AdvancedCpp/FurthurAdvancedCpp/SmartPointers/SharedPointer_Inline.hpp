#ifndef __NM_SMARTPOINTERS_SHAREDPOINTER_INLINE_HPP__
#define __NM_SMARTPOINTERS_SHAREDPOINTER_INLINE_HPP__


#include "SharedPointer.hpp"
#include <cstddef> // size_t
#include <stdexcept> // std::logic_error


#define UNUSED(x) ((void)(x))


namespace nm {

namespace smartpointers {


// Must be inline (to tag this function with Weak Symbol Tag)
template <typename T>
inline static size_t* SelectCorrectPointerValue(T* a_innerObjectPointer) {
    return (a_innerObjectPointer) ? new size_t(1) : nullptr;
}


template <typename T>
SharedPointer<T>::SharedPointer(T* a_pointer)
: m_pointer(a_pointer)
, m_sharedReferencesCounter(SelectCorrectPointerValue(a_pointer)) {
}


template <typename T>
SharedPointer<T> SharedPointer<T>::MakeSharedPointer(T a_pointedValue) {
    return SharedPointer<T>(new T(a_pointedValue));
}


template <typename T>
SharedPointer<T> SharedPointer<T>::MakeSharedPointer(const std::nullptr_t& a_nullptr) {
    UNUSED(a_nullptr);
    return SharedPointer<T>(nullptr);
}


template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& a_other)
: m_pointer(a_other.m_pointer)
, m_sharedReferencesCounter(a_other.m_sharedReferencesCounter){
    if(this->m_pointer) { // If the inner object pointer is not nullptr
        ++(*(this->m_sharedReferencesCounter));
    }
}


template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T>& a_other) {
    if(this != &a_other)
    {
        if(this->m_pointer) { // If the inner object pointer is not nullptr
            --(*(this->m_sharedReferencesCounter));

            if(this->IsDeleteRequired()) {
                this->FreeSharedPointerResources();
            }
        }


        this->m_pointer = a_other.m_pointer;
        this->m_sharedReferencesCounter = a_other.m_sharedReferencesCounter;

        if(this->m_pointer) { // If the NEW inner object pointer is not nullptr
            ++(*(this->m_sharedReferencesCounter));
        }
    }

    return *this;
}


template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const std::nullptr_t& a_nullptr) {
    UNUSED(a_nullptr);
    *this = SharedPointer<T>(nullptr);

    return *this;
}


template <typename T>
SharedPointer<T>::~SharedPointer() {
    if(this->m_pointer) {
        --(*(this->m_sharedReferencesCounter));

        if(this->IsDeleteRequired()) {
            this->FreeSharedPointerResources();
        }
    }
}


template <typename T>
T& SharedPointer<T>::operator*() {
    if(!this->m_pointer) {
        throw std::logic_error("Null Pointer Dereferencing Error");
    }

    return *(this->m_pointer);
}


template <typename T>
const T& SharedPointer<T>::operator*() const {
    if(!this->m_pointer) {
        throw std::logic_error("Null Pointer Dereferencing Error");
    }

    return *(this->m_pointer);
}


template <typename T>
T* SharedPointer<T>::operator->() {
    if(!this->m_pointer) {
        throw std::logic_error("Null Pointer Dereferencing Error");
    }

    return this->m_pointer;
}


template <typename T>
const T* SharedPointer<T>::operator->() const {
    if(!this->m_pointer) {
        throw std::logic_error("Null Pointer Dereferencing Error");
    }

    return this->m_pointer;
}


template <typename T>
T* SharedPointer<T>::GetRawPointer() {
    return this->m_pointer;
}


template <typename T>
size_t SharedPointer<T>::GetSharedReferencesCount() const {
    return (this->m_pointer) ? *(this->m_sharedReferencesCounter) : 0;
}


template <typename T>
bool SharedPointer<T>::operator==(const SharedPointer<T>& a_other) {
    return this->m_pointer == a_other->m_pointer;
}


template <typename T>
bool SharedPointer<T>::operator!=(const SharedPointer<T>& a_other) {
    return this->m_pointer != a_other->m_pointer;
}


template <typename T>
bool SharedPointer<T>::operator<=(const SharedPointer<T>& a_other) {
    return this->m_pointer <= a_other->m_pointer;
}


template <typename T>
bool SharedPointer<T>::operator>=(const SharedPointer<T>& a_other) {
    return this->m_pointer >= a_other->m_pointer;
}


template <typename T>
bool SharedPointer<T>::operator<(const SharedPointer<T>& a_other) {
    return this->m_pointer < a_other->m_pointer;
}


template <typename T>
bool SharedPointer<T>::operator>(const SharedPointer<T>& a_other) {
    return this->m_pointer > a_other->m_pointer;
}


template <typename T>
bool SharedPointer<T>::operator==(const std::nullptr_t& a_nullptr) {
    UNUSED(a_nullptr);

    return this->m_pointer == nullptr;
}


template <typename T>
bool SharedPointer<T>::operator!=(const std::nullptr_t& a_nullptr) {
    UNUSED(a_nullptr);

    return this->m_pointer != nullptr;
}


template <typename T>
bool SharedPointer<T>::operator==(const void* a_rawPointer) {
    return static_cast<const void*>(this->m_pointer) == a_rawPointer;
}


template <typename T>
bool SharedPointer<T>::operator!=(const void* a_rawPointer) {
    return static_cast<const void*>(this->m_pointer) != a_rawPointer;
}


template <typename T>
bool SharedPointer<T>::operator<=(const void* a_rawPointer) {
    return static_cast<const void*>(this->m_pointer) <= a_rawPointer;
}


template <typename T>
bool SharedPointer<T>::operator>=(const void* a_rawPointer) {
    return static_cast<const void*>(this->m_pointer) >= a_rawPointer;
}


template <typename T>
bool SharedPointer<T>::operator<(const void* a_rawPointer) {
    return static_cast<const void*>(this->m_pointer) < a_rawPointer;
}


template <typename T>
bool SharedPointer<T>::operator>(const void* a_rawPointer) {
    return static_cast<const void*>(this->m_pointer) > a_rawPointer;
}


template <typename T>
SharedPointer<T>::operator bool() {
    return bool(this->m_pointer);
}


template <typename T>
SharedPointer<T>::operator const T*() {
    return this->m_pointer;
}


template <typename T>
SharedPointer<T>::operator const void*() {
    return static_cast<void*>(this->m_pointer);
}


template <typename T>
bool SharedPointer<T>::IsDeleteRequired() const {
    return *(this->m_sharedReferencesCounter) == 0; // If no references at all
}


template <typename T>
void SharedPointer<T>::FreeSharedPointerResources() {
    // Would work even if pointers are nullptr
    delete this->m_pointer;
    delete this->m_sharedReferencesCounter;

    // For safety
    this->m_pointer = nullptr;
    this->m_sharedReferencesCounter = nullptr;
}


// Must be inline (to tag this function with Weak Symbol Tag)
template <typename U>
inline std::ostream& operator<<(std::ostream& a_os, const SharedPointer<U>& a_sharedPointer) {
    a_os << a_sharedPointer.m_pointer;

    return a_os;
}

} // smartpointers

} // nm


#endif // __NM_SMARTPOINTERS_SHAREDPOINTER_INLINE_HPP__