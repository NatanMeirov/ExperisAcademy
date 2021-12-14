#ifndef __HASHMAP_HASHMAPTYPES_HPP__
#define __HASHMAP_HASHMAPTYPES_HPP__


#include "KeyValuePair.hpp"
#include <vector> // Outer Container type - its iterator MUST be a Random Access Tag iterator (uses: .at(), operator+(size_t offset))
#include <list> // Inner Container type


namespace HashMap {

template <typename K, typename V>
class Types {
public:
    typedef KeyValuePair<K,V> KeyVal;
    typedef std::list<KeyVal> InnerContainer;
    typedef std::vector<InnerContainer> OuterContainer;
};

} // HashMap


#endif // __HASHMAP_HASHMAPTYPES_HPP__