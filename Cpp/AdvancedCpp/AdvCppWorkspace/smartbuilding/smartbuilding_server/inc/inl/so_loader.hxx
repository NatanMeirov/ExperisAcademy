#ifndef NM_SO_LOADER_HXX
#define NM_SO_LOADER_HXX


#include <string> // std::string
#include <dlfcn.h> // dynamic libraries api
#include <stdexcept> // std::runtime_error


namespace smartbuilding
{

inline SoLoader::SoLoader(const std::string& a_module)
: m_soModuleHandler(dlopen(a_module.c_str(), RTLD_LAZY))
{
    if(!m_soModuleHandler)
    {
        throw std::runtime_error("Failed to load dynamic library: " + std::string(dlerror()));
    }
}


inline SoLoader::~SoLoader()
{
    dlclose(m_soModuleHandler);
}


template <typename FunctionPtrType>
FunctionPtrType SoLoader::Fetch(const std::string& a_function)
{
    FunctionPtrType funcPtr = reinterpret_cast<FunctionPtrType>(dlsym(m_soModuleHandler, a_function.c_str()));
    if(!funcPtr)
    {
        throw std::runtime_error("Failed to fetch a function: " + std::string(dlerror()));
    }

    return funcPtr;
}

} // smartbuilding


#endif // NM_SO_LOADER_HXX
