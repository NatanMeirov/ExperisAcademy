#ifndef SF_REFLECTOR_HPP
#define SF_REFLECTOR_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include <unordered_map>
#include "ser_fw_obj_factory.hpp"


namespace ser_fw
{

// A class that handles "Reflection like action" at Compile-Time instead of Run-Time (Reflection is missing in C++).
// The Reflector "maps" a string (representing a type name) to a pointer to a Factory Method, that creates a concrete SerFwObj object at Run-Time.
class Reflector
{
public:
    Reflector() = default;
    Reflector(const Reflector& a_other) = default;
    Reflector& operator=(const Reflector& a_other) = default;
    ~Reflector() = default;

    void AddReflection(const std::string& a_typeAsString, std::shared_ptr<infra::SerFwObjFactory> a_relatedTypeFactoryMethod);
    void AddReflection(std::pair<std::string, std::shared_ptr<infra::SerFwObjFactory>> a_typeToFactoryPair);
    std::shared_ptr<infra::SerFwObjFactory> Reflect(const std::string& a_typeAsString) const;

private:
    std::unordered_map<std::string, std::shared_ptr<infra::SerFwObjFactory>> m_reflectionMap;
};

} // ser_fw


#endif // SF_REFLECTOR_HPP
