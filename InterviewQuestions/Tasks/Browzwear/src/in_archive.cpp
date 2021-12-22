#include "in_archive.hpp"


namespace ser_fw
{

InArchive::InArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileReader> a_formattedFileReader, std::shared_ptr<infra::IDeserializationFormatter> a_formatter)
: m_datafile(a_datafile)
, m_formattedFileReader(a_formattedFileReader)
, m_formatter(a_formatter)
{
}

} // ser_fw
