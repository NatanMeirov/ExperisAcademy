#ifndef SF_REFLECTOR_HPP
#define SF_REFLECTOR_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include <type_traits> // std::is_same
#include <unordered_map>
#include "iser_fw_obj_factory.hpp"


namespace ser_fw
{

// A class that handles "Reflection like action" at Compile-Time instead of Run-Time (Reflection is missing in C++).
// The Reflector "maps" a string (representing a type name) to a pointer to a Factory Method, that creates a concrete ISerFwObj object at Run-Time.
// Concept of FactoryPtrT: FactoryPtrT must be std::shared_ptr<ISerFwObjFactory<RetPtrT>> or ISerFwObjFactory<RetPtrT>* type.
// Concept of RetPtrT: RetPtrT must be std::shared_ptr<ISerFwObj> or ISerFwObj*.
template <typename FactoryPtrT = std::shared_ptr<infra::ISerFwObjFactory<std::shared_ptr<infra::ISerFwObj>>>,
           typename RetPtrT = std::shared_ptr<infra::ISerFwObj>>
class Reflector
{
    static_assert(std::is_same<FactoryPtrT, std::shared_ptr<infra::ISerFwObjFactory<RetPtrT>>>::value
        || std::is_same<FactoryPtrT, infra::ISerFwObjFactory<RetPtrT>*>::value, "FactoryPtrT must be std::shared_ptr<ISerFwObjFactory<RetPtrT>> or ISerFwObjFactory<RetPtrT>*");

    static_assert(std::is_same<RetPtrT, std::shared_ptr<infra::ISerFwObj>>::value
        || std::is_same<RetPtrT, infra::ISerFwObj*>::value, "RetPtrT must be std::shared_ptr<ISerFwObj> or ISerFwObj*");

private:
    class ReflectionProxy
    {
    public:
        explicit ReflectionProxy(FactoryPtrT a_reflectedFactoryMethod)
        : m_reflectedFactoryMethod(a_reflectedFactoryMethod)
        {
        }

        RetPtrT Invoke();

    private:
        FactoryPtrT m_reflectedFactoryMethod;
    };

public:
    Reflector() = default;
    Reflector(const Reflector& a_other) = default;
    Reflector& operator=(const Reflector& a_other) = default;
    ~Reflector() = default;

    void AddReflection(const std::string& a_typeAsString, FactoryPtrT a_relatedTypeFactoryMethod);
    void AddReflection(std::pair<std::string, FactoryPtrT> a_typeToFactoryPair);
    ReflectionProxy Reflect(const std::string& a_typeAsString) const;

private:
    std::unordered_map<std::string, FactoryPtrT> m_reflectionMap;
};


// Inline implementation:
template <typename FactoryPtrT, typename RetPtrT>
void Reflector<FactoryPtrT,RetPtrT>::AddReflection(const std::string& a_typeAsString, FactoryPtrT a_relatedTypeFactoryMethod)
{
    m_reflectionMap.insert({a_typeAsString, a_relatedTypeFactoryMethod});
}


template <typename FactoryPtrT, typename RetPtrT>
void Reflector<FactoryPtrT,RetPtrT>::AddReflection(std::pair<std::string, FactoryPtrT> a_typeToFactoryPair)
{
    m_reflectionMap.insert(a_typeToFactoryPair);
}


template <typename FactoryPtrT, typename RetPtrT>
typename Reflector<FactoryPtrT,RetPtrT>::ReflectionProxy Reflector<FactoryPtrT,RetPtrT>::Reflect(const std::string& a_typeAsString) const
{
    return ReflectionProxy(m_reflectionMap.at(a_typeAsString));
}


template <typename FactoryPtrT, typename RetPtrT>
RetPtrT Reflector<FactoryPtrT,RetPtrT>::ReflectionProxy::Invoke()
{
    return m_reflectedFactoryMethod->Create();
}

} // ser_fw


#endif // SF_REFLECTOR_HPP
