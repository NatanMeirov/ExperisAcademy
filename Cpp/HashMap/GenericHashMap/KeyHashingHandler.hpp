#ifndef __HASHMAP_KEYHASHER_HPP__
#define __HASHMAP_KEYHASHER_HPP__


#include <cstddef> // size_t


namespace HashMap {

template <typename K>
class KeyHashingHandler {
public:
    virtual ~KeyHashingHandler() = default;
    virtual size_t Hash(K a_key) const = 0;
};

} // HashMap


#endif // __HASHMAP_KEYHASHER_HPP__