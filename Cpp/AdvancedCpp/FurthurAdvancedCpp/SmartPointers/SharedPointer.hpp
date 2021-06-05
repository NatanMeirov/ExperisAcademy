#ifndef __NM_SMARTPOINTERS_SHAREDPOINTER_HPP__
#define __NM_SMARTPOINTERS_SHAREDPOINTER_HPP__


#include <cstddef> // size_t
#include <ostream> // std::ostream


namespace nm {

namespace smartpointers {

template <typename T>
class SharedPointer {
    template <typename U>
    friend std::ostream& operator<<(std::ostream& a_os, const SharedPointer<U>& a_sharedPointer);
public:
    SharedPointer(const SharedPointer<T>& a_other);
    SharedPointer<T>& operator=(const SharedPointer<T>& a_other);
    SharedPointer<T>& operator=(const std::nullptr_t& a_nullptr);
    ~SharedPointer();

    // Creation Method
    static SharedPointer<T> MakeSharedPointer(T a_pointedValue); // T a_pointedValue must be copy-able (copy constructable) - that is why it must be passed by value to this method
    static SharedPointer<T> MakeSharedPointer(const std::nullptr_t& a_nullptr);

    // Prevented (delete them all is better then putting them on private section (and while using delete - they should be public and not private to make the compiler know that they are deleted and not just tell that they are private))
    // (Note: Cannot prevent using ::new operator (global scope new) - this is a user responsability)
    void* operator new(size_t a_size) = delete;
    void* operator new(size_t a_size, void* a_pointer) = delete; // placement new
    void* operator new[](size_t a_size) = delete;
    void* operator new[](size_t a_size, void* a_pointer) = delete; // placement new[]
    void operator delete(void* a_sharedPointer) = delete;
    void operator delete[](void* a_sharedPointer) = delete;

    // Valid Operations
    T& operator*() const;
    T* operator->();
    T* GetRawPointer(); // Using Get method is the user's responsability (Returns the stored pointer)
    size_t GetSharedReferencesCount() const; // Same as Use count, returns 0 if pointer is nullptr

    // Addresses Comparison
    bool operator==(const SharedPointer<T>& a_other);
    bool operator!=(const SharedPointer<T>& a_other);
    bool operator<=(const SharedPointer<T>& a_other);
    bool operator>=(const SharedPointer<T>& a_other);
    bool operator<(const SharedPointer<T>& a_other);
    bool operator>(const SharedPointer<T>& a_other);
    bool operator==(const std::nullptr_t& a_nullptr);
    bool operator!=(const std::nullptr_t& a_nullptr);
    bool operator==(const void* a_rawPointer);
    bool operator!=(const void* a_rawPointer);
    bool operator<=(const void* a_rawPointer);
    bool operator>=(const void* a_rawPointer);
    bool operator<(const void* a_rawPointer);
    bool operator>(const void* a_rawPointer);

    // Valid Casts
    operator bool();
    explicit operator T*(); // To be able to explicit cast to a T* (Note: Casting to a raw pointer is the user's responsability)
    explicit operator void*(); // To be able to explicit cast to a void* (Note: Casting to a generic raw pointer is the user's responsability)

private:
    explicit SharedPointer(T* a_pointer); // To force using MakeSharedPointer static method
    bool IsDeleteRequired() const;
    void FreeSharedPointerResources(); // Deletes and Sets the pointer to nullptr

    T* m_pointer;
    size_t* m_sharedReferencesCounter;
};

} // smartpointers

} // nm


#endif // __NM_SMARTPOINTERS_SHAREDPOINTER_HPP__