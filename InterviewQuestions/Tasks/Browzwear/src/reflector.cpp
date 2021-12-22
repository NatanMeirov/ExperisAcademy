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


std::shared_ptr<infra::SerFwObjFactory> Reflector::Reflect(const std::string& a_typeAsString) const
{
    return m_reflectionMap.at(a_typeAsString);
}

} // ser_fw
