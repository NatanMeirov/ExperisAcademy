#include <cstring> // strlen
#include <stdexcept> // std::invalid_argument


size_t SafeStrlenWrapper(const char* _str) {
    if(! _str) {
        throw std::invalid_argument("Null Pointer Error");
    }

    return strlen(_str);
}