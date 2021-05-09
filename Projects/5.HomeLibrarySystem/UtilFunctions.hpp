#include <cstddef> // size_t


/**
 * @brief Safe strlen function wrapper, to prevent calling strlen function with NULL pointer
 * @details Throws: std::invalid_argument exception
 * @param[in] _str: A string to get its length
 * @return size_t - the length of the given string
 */
size_t SafeStrlenWrapper(const char* _str);