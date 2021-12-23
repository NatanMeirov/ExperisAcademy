#ifndef SF_OUT_ARCHIVE_HPP
#define SF_OUT_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include "iformatted_file_writer.hpp"
#include "iserialization_formatter.hpp"
#include "iser_fw_obj.hpp"


namespace ser_fw
{

// A class that handles writing ISerFwObjs to a data file,
// from a pre-initialized ISerFwObjs Container.
// Concept of IFormattedFileWriterPtr: IFormattedFileWriterPtr must be or std::shared_ptr<IFormattedFileWriter> or IFormattedFileWriter* type.
// Concept of ISerializationFormatterPtr: ISerializationFormatterPtr must be or std::shared_ptr<ISerializationFormatter> or ISerializationFormatter* type.
template <typename IFormattedFileWriterPtr = std::shared_ptr<infra::IFormattedFileWriter>,
            typename ISerializationFormatterPtr = std::shared_ptr<infra::ISerializationFormatter>>
class OutArchive
{
    static_assert(std::is_same<IFormattedFileWriterPtr, std::shared_ptr<infra::IFormattedFileWriter>>::value
        || std::is_same<IFormattedFileWriterPtr, infra::IFormattedFileWriter*>::value, "IFormattedFileWriterPtr must be or std::shared_ptr<IFormattedFileWriter> or IFormattedFileWriter*");

    static_assert(std::is_same<ISerializationFormatterPtr, std::shared_ptr<infra::ISerializationFormatter>>::value
        || std::is_same<ISerializationFormatterPtr, infra::ISerializationFormatter*>::value, "ISerializationFormatterPtr must be or std::shared_ptr<ISerializationFormatter> or ISerializationFormatter*");

public:
    OutArchive(const std::string& a_datafile, IFormattedFileWriterPtr a_formattedFileWriter, ISerializationFormatterPtr a_formatter);
    OutArchive(const OutArchive& a_other) = default;
    OutArchive& operator=(const OutArchive& a_other) = default;
    ~OutArchive() = default;

    // Concept of C: C must be a container that implements begin(), end(), and must define value_type type.
    // C::value_type must be std::shared_ptr<ISerFwObj> or ISerFwObj* type.
    template <typename C>
    void Write(const C& a_serFwObjects) const;

private:
    std::string m_datafile;
    IFormattedFileWriterPtr m_formattedFileWriter;
    ISerializationFormatterPtr m_formatter;
};


// Inline implementation:
template <typename IFormattedFileWriterPtr, typename ISerializationFormatterPtr>
OutArchive<IFormattedFileWriterPtr,ISerializationFormatterPtr>::OutArchive(const std::string& a_datafile, IFormattedFileWriterPtr a_formattedFileWriter, ISerializationFormatterPtr a_formatter)
: m_datafile(a_datafile)
, m_formattedFileWriter(a_formattedFileWriter)
, m_formatter(a_formatter)
{
}


template <typename IFormattedFileWriterPtr, typename ISerializationFormatterPtr>
template <typename C>
void OutArchive<IFormattedFileWriterPtr,ISerializationFormatterPtr>::Write(const C& a_serFwObjects) const
{
    static_assert(std::is_same<typename C::value_type, std::shared_ptr<infra::ISerFwObj>>::value
        || std::is_same<typename C::value_type, infra::ISerFwObj*>::value, "C::value_type must be std::shared_ptr<ISerFwObj> or ISerFwObj*");

    Types::ParsedLinesCollection parsedFormatLines;

    std::for_each(a_serFwObjects.begin(), a_serFwObjects.end(),
    [&](typename C::value_type a_serFwObjPtr)
    {
        parsedFormatLines.push_back(
            m_formatter->Serialize({a_serFwObjPtr->TypeName(), a_serFwObjPtr->TransformTo()}));
    });

    m_formattedFileWriter->WriteFile(parsedFormatLines, m_datafile);
}

} // ser_fw


#endif // SF_OUT_ARCHIVE_HPP
