#include "in_archive.hpp"
#include <algorithm> // std::for_each
#include "ser_fw_types.hpp"
#include "ser_fw_obj.hpp"


namespace ser_fw
{

InArchive::InArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileReader> a_formattedFileReader, std::shared_ptr<infra::IDeserializationFormatter> a_formatter)
: m_datafile(a_datafile)
, m_formattedFileReader(a_formattedFileReader)
, m_formatter(a_formatter)
{
}


void InArchive::Read(SerFwObjsContainer& a_serFwObjects) const
{
    Types::ParsedLinesCollection parsedFormatLines = m_formattedFileReader->ReadFile(m_datafile);

    // TODO: think how to convert (deserialize) each line - to the EXACT concrete object
    //? Maybe use std::string m_type in SerFwObj and a factory method create() or something...
}

} // ser_fw
