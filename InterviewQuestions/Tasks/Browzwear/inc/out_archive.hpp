#ifndef SF_OUT_ARCHIVE_HPP
#define SF_OUT_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include "iformatted_file_writer.hpp"
#include "iserialization_formatter.hpp"
#include "ser_fw_obj.hpp"


namespace ser_fw
{

// A class that handles writing SerFwObjs to a data file,
// from a pre-initialized SerFwObjs Container.
class OutArchive
{
public:
    OutArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileWriter> a_formattedFileWriter, std::shared_ptr<infra::ISerializationFormatter> a_formatter);
    OutArchive(const OutArchive& a_other) = default;
    OutArchive& operator=(const OutArchive& a_other) = default;
    ~OutArchive() = default;

    // Concept of C: C must be a container that implements begin(), end(), and must define value_type type.
    // C::value_type must be std::shared_ptr<SerFwObj> type.
    template <typename C>
    void Write(const C& a_serFwObjects) const;

private:
    std::string m_datafile;
    std::shared_ptr<infra::IFormattedFileWriter> m_formattedFileWriter;
    std::shared_ptr<infra::ISerializationFormatter> m_formatter;
};


// Inline implementation:
template <typename C>
void OutArchive::Write(const C& a_serFwObjects) const
{
    static_assert(std::is_same<typename C::value_type,std::shared_ptr<infra::SerFwObj>>::value, "C::value_type must be std::shared_ptr<infra::SerFwObj>");

    Types::ParsedLinesCollection parsedFormatLines;

    std::for_each(a_serFwObjects.begin(), a_serFwObjects.end(),
    [&](std::shared_ptr<infra::SerFwObj> a_serFwObjPtr)
    {
        parsedFormatLines.push_back(
            m_formatter->Serialize({a_serFwObjPtr->TypeName(), a_serFwObjPtr->TransformTo()}));
    });

    m_formattedFileWriter->WriteFile(parsedFormatLines, m_datafile);
}

} // ser_fw


#endif // SF_OUT_ARCHIVE_HPP
