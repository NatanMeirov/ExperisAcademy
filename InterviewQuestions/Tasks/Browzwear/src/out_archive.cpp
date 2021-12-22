#include "out_archive.hpp"


namespace ser_fw
{

OutArchive::OutArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileWriter> a_formattedFileWriter, std::shared_ptr<infra::ISerializationFormatter> a_formatter)
: m_datafile(a_datafile)
, m_formattedFileWriter(a_formattedFileWriter)
, m_formatter(a_formatter)
{
}

} // ser_fw
