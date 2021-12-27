#ifndef SF_IN_ARCHIVE_HPP
#define SF_IN_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include <unordered_map>
#include "ifile_handler.hpp"
#include "iformatter.hpp"
#include "iser_fw_obj.hpp"
#include "iser_fw_obj_factory.hpp"
#include "reflector.hpp"


namespace ser_fw
{

// A class that handles reading ISerFwObjs from a data file,
// and loading those objects into a ISerFwObjs Container, using a Reflector class.
// The Reflector is used to demonstrate a "Reflection like action", but in Compile-Time, instead of Run-Time
// (Reflection is missing in C++).
// Concept of IFileHandlerPtr: must be or std::shared_ptr<IFileHandler> or IFileHandlerPtr* type.
// Concept of IFormatterPtr: must be or std::shared_ptr<IFormatter> or IFormatter* type.
template <typename IFileHandlerPtr = std::shared_ptr<infra::IFileHandler>,
            typename IFormatterPtr = std::shared_ptr<infra::IFormatter>>
class InArchive
{
    static_assert(std::is_same<IFileHandlerPtr, std::shared_ptr<infra::IFileHandler>>::value
        || std::is_same<IFileHandlerPtr, infra::IFileHandler*>::value, "IFileHandlerPtr must be or std::shared_ptr<IFileHandler> or IFileHandler*");

    static_assert(std::is_same<IFormatterPtr, std::shared_ptr<infra::IFormatter>>::value
        || std::is_same<IFormatterPtr, infra::IFormatter*>::value, "IFormatterPtr must be or std::shared_ptr<IFormatter> or IFormatter*");

public:
    InArchive(const std::string& a_datafile, IFileHandlerPtr a_fileHandler, IFormatterPtr a_formatter);
    InArchive(const InArchive& a_other) = default;
    InArchive& operator=(const InArchive& a_other) = default;
    ~InArchive() = default;

    // Concept of C: must be a container that implements begin(), end(), push_back() and must define value_type type.
    // C::value_type must be std::shared_ptr<ISerFwObj> or ISerFwObj* type.
    // Concept of ReflectorT: must meet the Reflector<RetPtrT,FactoryPtrT> class requirements,
    // and RetPtrT MUST be the same as C::value_type.
    template <typename C, typename ReflectorT>
    void Read(C& a_serFwObjects, const ReflectorT& a_reflector) const;

private:
    std::string m_datafile;
    IFileHandlerPtr m_fileHandler;
    IFormatterPtr m_formatter;
};


// Inline implementation:
template <typename IFileHandlerPtr, typename IFormatterPtr>
InArchive<IFileHandlerPtr,IFormatterPtr>::InArchive(const std::string& a_datafile, IFileHandlerPtr a_fileHandler, IFormatterPtr a_formatter)
: m_datafile(a_datafile)
, m_fileHandler(a_fileHandler)
, m_formatter(a_formatter)
{
}


template <typename IFileHandlerPtr, typename IFormatterPtr>
template <typename C, typename ReflectorT>
void InArchive<IFileHandlerPtr,IFormatterPtr>::Read(C& a_serFwObjects, const ReflectorT& a_reflector) const
{
    static_assert(std::is_same<ReflectorT, Reflector<std::shared_ptr<infra::ISerFwObjFactory<std::shared_ptr<infra::ISerFwObj>>>, std::shared_ptr<infra::ISerFwObj>>>::value
        || std::is_same<ReflectorT, Reflector<std::shared_ptr<infra::ISerFwObjFactory<infra::ISerFwObj*>>, infra::ISerFwObj*>>::value
        || std::is_same<ReflectorT, Reflector<infra::ISerFwObjFactory<std::shared_ptr<infra::ISerFwObj>>*, std::shared_ptr<infra::ISerFwObj>>>::value
        || std::is_same<ReflectorT, Reflector<infra::ISerFwObjFactory<infra::ISerFwObj*>*, infra::ISerFwObj*>>::value, "ReflectorT must meet the Reflector<RetPtrT,FactoryPtrT> class requirements");

    static_assert(std::is_same<typename C::value_type, std::shared_ptr<infra::ISerFwObj>>::value
        || std::is_same<typename C::value_type, infra::ISerFwObj*>::value, "C::value_type must be std::shared_ptr<ISerFwObj> or ISerFwObj*");


    Types::ParsedLinesCollection parsedFormatLines = m_fileHandler->ReadFile(m_datafile);

    std::for_each(parsedFormatLines.begin(), parsedFormatLines.end(),
    [&](const std::string& a_formattedStr)
    {
        Types::TypeToParamsMapPair typedParamsMap = m_formatter->Deserialize(a_formattedStr);
        std::string type = typedParamsMap.first; // For readability

        typename C::value_type serFwObjPtr = a_reflector.Reflect(type).CreateInstance();
        serFwObjPtr->FromParamsMap(typedParamsMap.second); // typedParamsMap.second == ParamsMap
        a_serFwObjects.push_back(serFwObjPtr);
    });
}

} // ser_fw


#endif // SF_IN_ARCHIVE_HPP
