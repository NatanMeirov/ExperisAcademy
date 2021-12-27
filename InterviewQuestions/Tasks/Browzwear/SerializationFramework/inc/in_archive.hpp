#ifndef SF_IN_ARCHIVE_HPP
#define SF_IN_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include <unordered_map>
#include "iformatted_file_reader.hpp"
#include "ideserialization_formatter.hpp"
#include "iser_fw_obj.hpp"
#include "iser_fw_obj_factory.hpp"
#include "reflector.hpp"


namespace ser_fw
{

// A class that handles reading ISerFwObjs from a data file,
// and loading those objects into a ISerFwObjs Container, using a Reflector class.
// The Reflector is used to demonstrate a "Reflection like action", but in Compile-Time, instead of Run-Time
// (Reflection is missing in C++).
// Concept of IFormattedFileReaderPtr: IFormattedFileReaderPtr must be or std::shared_ptr<IFormattedFileReader> or IFormattedFileReader* type.
// Concept of IDeserializationFormatterPtr: IDeserializationFormatterPtr must be or std::shared_ptr<IDeserializationFormatter> or IDeserializationFormatter* type.
template <typename IFormattedFileReaderPtr = std::shared_ptr<infra::IFormattedFileReader>,
            typename IDeserializationFormatterPtr = std::shared_ptr<infra::IDeserializationFormatter>>
class InArchive
{
    static_assert(std::is_same<IFormattedFileReaderPtr, std::shared_ptr<infra::IFormattedFileReader>>::value
        || std::is_same<IFormattedFileReaderPtr, infra::IFormattedFileReader*>::value, "IFormattedFileReaderPtr must be or std::shared_ptr<IFormattedFileReader> or IFormattedFileReader*");

    static_assert(std::is_same<IDeserializationFormatterPtr, std::shared_ptr<infra::IDeserializationFormatter>>::value
        || std::is_same<IDeserializationFormatterPtr, infra::IDeserializationFormatter*>::value, "IDeserializationFormatterPtr must be or std::shared_ptr<IDeserializationFormatter> or IDeserializationFormatter*");

public:
    InArchive(const std::string& a_datafile, IFormattedFileReaderPtr a_formattedFileReader, IDeserializationFormatterPtr a_formatter);
    InArchive(const InArchive& a_other) = default;
    InArchive& operator=(const InArchive& a_other) = default;
    ~InArchive() = default;

    // Concept of C: C must be a container that implements begin(), end(), push_back() and must define value_type type.
    // C::value_type must be std::shared_ptr<ISerFwObj> or ISerFwObj* type.
    // Concept of ReflectorT: ReflectorT must meet the Reflector<RetPtrT,FactoryPtrT> class requirements,
    // and RetPtrT MUST be the same as typename C::value_type.
    template <typename C, typename ReflectorT>
    void Read(C& a_serFwObjects, const ReflectorT& a_reflector) const;

private:
    std::string m_datafile;
    IFormattedFileReaderPtr m_formattedFileReader;
    IDeserializationFormatterPtr m_formatter;
};


// Inline implementation:
template <typename IFormattedFileReaderPtr, typename IDeserializationFormatterPtr>
InArchive<IFormattedFileReaderPtr,IDeserializationFormatterPtr>::InArchive(const std::string& a_datafile, IFormattedFileReaderPtr a_formattedFileReader, IDeserializationFormatterPtr a_formatter)
: m_datafile(a_datafile)
, m_formattedFileReader(a_formattedFileReader)
, m_formatter(a_formatter)
{
}


template <typename IFormattedFileReaderPtr, typename IDeserializationFormatterPtr>
template <typename C, typename ReflectorT>
void InArchive<IFormattedFileReaderPtr,IDeserializationFormatterPtr>::Read(C& a_serFwObjects, const ReflectorT& a_reflector) const
{
	    static_assert(std::is_same<ReflectorT, Reflector<std::shared_ptr<infra::ISerFwObjFactory<std::shared_ptr<infra::ISerFwObj>>>, std::shared_ptr<infra::ISerFwObj>>>::value
        || std::is_same<ReflectorT, Reflector<std::shared_ptr<infra::ISerFwObjFactory<infra::ISerFwObj*>>, infra::ISerFwObj*>>::value
        || std::is_same<ReflectorT, Reflector<infra::ISerFwObjFactory<std::shared_ptr<infra::ISerFwObj>>*, std::shared_ptr<infra::ISerFwObj>>>::value
        || std::is_same<ReflectorT, Reflector<infra::ISerFwObjFactory<infra::ISerFwObj*>*, infra::ISerFwObj*>>::value, "ReflectorT must meet the Reflector<RetPtrT,FactoryPtrT> class requirements");

    static_assert(std::is_same<typename C::value_type, std::shared_ptr<infra::ISerFwObj>>::value
        || std::is_same<typename C::value_type, infra::ISerFwObj*>::value, "C::value_type must be std::shared_ptr<ISerFwObj> or ISerFwObj*");

    Types::ParsedLinesCollection parsedFormatLines = m_formattedFileReader->ReadFile(m_datafile);

    std::for_each(parsedFormatLines.begin(), parsedFormatLines.end(),
    [&](const std::string& a_formattedStr)
    {
        Types::TypedParamsMap paramsWithTypeIndication = m_formatter->Deserialize(a_formattedStr);
        std::string type = paramsWithTypeIndication.first; // For readability

        typename C::value_type serFwObjPtr = a_reflector.Reflect(type).CreateInstance();
        serFwObjPtr->TransformFrom(paramsWithTypeIndication.second); // paramsWithTypeIndication.second == ParamsMap
        a_serFwObjects.push_back(serFwObjPtr);
    });
}

} // ser_fw


#endif // SF_IN_ARCHIVE_HPP
