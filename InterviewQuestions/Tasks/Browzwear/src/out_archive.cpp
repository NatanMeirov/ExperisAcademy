#include "out_archive.hpp"
#include <algorithm> // std::for_each
#include "ser_fw_types.hpp"
#include "ser_fw_obj.hpp"


namespace ser_fw
{

OutArchive::OutArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileWriter> a_formattedFileWriter, std::shared_ptr<infra::ISerializationFormatter> a_formatter)
: m_datafile(a_datafile)
, m_formattedFileWriter(a_formattedFileWriter)
, m_formatter(a_formatter)
{
}


void OutArchive::Write(const SerFwObjsContainer& a_serFwObjects) const
{
    Types::ParsedLinesCollection parsedFormatLines;

    std::for_each(a_serFwObjects.begin(), a_serFwObjects.end(),
    [&](std::shared_ptr<infra::SerFwObj> a_serFwObjPtr)
    {
        parsedFormatLines.push_back(a_serFwObjPtr->Serialize(m_formatter));
    });

    m_formattedFileWriter->WriteFile(parsedFormatLines, m_datafile);
}

} // ser_fw
