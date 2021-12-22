#ifndef SF_IN_ARCHIVE_HPP
#define SF_IN_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include <unordered_map>
#include "iformatted_file_reader.hpp"
#include "ideserialization_formatter.hpp"
#include "ser_fw_obj.hpp"
#include "ser_fw_obj_factory.hpp"
#include "reflector.hpp"


namespace ser_fw
{

// A class that handles reading SerFwObjs from a data file,
// and loading those objects into a SerFwObjs Container.
//! ADD DOC!
class InArchive
{
public:
    InArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileReader> a_formattedFileReader, std::shared_ptr<infra::IDeserializationFormatter> a_formatter);
    InArchive(const InArchive& a_other) = default;
    InArchive& operator=(const InArchive& a_other) = default;
    ~InArchive() = default;

    // Concept of C: C must be a container that implements begin(), end(), and must define value_type type.
    // C::value_type must be std::shared_ptr<SerFwObj> type.
    template <typename C>
    void Read(C& a_serFwObjects, const Reflector& a_reflector) const;

private:
    std::string m_datafile;
    std::shared_ptr<infra::IFormattedFileReader> m_formattedFileReader;
    std::shared_ptr<infra::IDeserializationFormatter> m_formatter;
};


// Inline implementation:
template <typename C>
void InArchive::Read(C& a_serFwObjects, const Reflector& a_reflector) const
{
    static_assert(std::is_same<typename C::value_type,std::shared_ptr<infra::SerFwObj>>::value, "C::value_type must be std::shared_ptr<infra::SerFwObj>");

    Types::ParsedLinesCollection parsedFormatLines = m_formattedFileReader->ReadFile(m_datafile);

    std::for_each(parsedFormatLines.begin(), parsedFormatLines.end(),
    [&](const std::string& a_formattedStr)
    {
        Types::TypedParamsMap paramsWithTypeIndication = m_formatter->Deserialize(a_formattedStr);
        std::string type = paramsWithTypeIndication.first; // For readability

        std::shared_ptr<infra::SerFwObj> serFwObjPtr = a_reflector.Reflect(type)->Create();
        serFwObjPtr->TransformFrom(paramsWithTypeIndication.second); // paramsWithTypeIndication.second == ParamsMap
        a_serFwObjects.push_back(serFwObjPtr);
    });
}

} // ser_fw


#endif // SF_IN_ARCHIVE_HPP
