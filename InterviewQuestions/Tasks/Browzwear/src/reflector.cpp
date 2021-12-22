#include "reflector.hpp"


namespace ser_fw
{

void Reflector::AddReflection(const std::string& a_typeAsString, std::shared_ptr<infra::SerFwObjFactory> a_relatedTypeFactoryMethod)
{
    m_reflectionMap.insert({a_typeAsString, a_relatedTypeFactoryMethod});
}


void Reflector::AddReflection(std::pair<std::string, std::shared_ptr<infra::SerFwObjFactory>> a_typeToFactoryPair)
{
    m_reflectionMap.insert(a_typeToFactoryPair);
}


Reflector::ReflectionProxy Reflector::Reflect(const std::string& a_typeAsString) const
{
    return ReflectionProxy(m_reflectionMap.at(a_typeAsString));
}


std::shared_ptr<infra::SerFwObj> Reflector::ReflectionProxy::Invoke()
{
    return m_reflectedFactoryMethod->Create();
}

} // ser_fw
