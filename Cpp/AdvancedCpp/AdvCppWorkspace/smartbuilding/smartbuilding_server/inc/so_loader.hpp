#ifndef NM_SO_LOADER_HPP
#define NM_SO_LOADER_HPP


#include <string> // std::string


namespace smartbuilding
{

// An RAII .so loader class wrapper for dyn-lib C api
class SoLoader
{
public:
    explicit SoLoader(const std::string& a_module);
    SoLoader(const SoLoader& a_other) = default;
    SoLoader& operator=(const SoLoader& a_other) = default;
    ~SoLoader();

    // Concept of FunctionPtrType: FunctionPtrType must be a pointer to a function (of any type)
    template <typename FunctionPtrType>
    FunctionPtrType Fetch(const std::string& a_function);

public:
    void* m_soModuleHandler;
};

} // smartbuilding


#include "inl/so_loader.hxx"


#endif // NM_SO_LOADER_HPP
