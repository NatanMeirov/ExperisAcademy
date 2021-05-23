#ifndef __HASHMAP_KEYVALUEPAIR_HPP__
#define __HASHMAP_KEYVALUEPAIR_HPP__


namespace HashMap {

template <typename K, typename V>
class KeyValuePair {
public:
    KeyValuePair(const K& a_key, const V& a_value);
    KeyValuePair(const KeyValuePair<K,V>& a_other) = default;
    KeyValuePair<K,V>& operator=(const KeyValuePair<K,V>& a_other) = default;
    ~KeyValuePair() = default;

    K GetKey() const;
    V GetValue() const;
    V& GetValueByRef();

private:
    K m_key;
    V m_value;
};

} // HashMap


#endif // __HASHMAP_KEYVALUEPAIR_HPP__