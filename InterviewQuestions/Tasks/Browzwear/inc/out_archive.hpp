#ifndef SF_OUT_ARCHIVE_HPP
#define SF_OUT_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <vector>
#include "iformatted_file_writer.hpp"
#include "iserialization_formatter.hpp"
#include "ser_fw_obj.hpp"


namespace ser_fw
{

// A class that handles writing SerFwObjs to a data file,
// from a pre-initialized OutArchive::SerFwObjsContainer.
class OutArchive
{
public:
    using SerFwObjsContainer = std::vector<std::shared_ptr<infra::SerFwObj>>;

public:
    OutArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileWriter> a_formattedFileWriter, std::shared_ptr<infra::ISerializationFormatter> a_formatter);
    OutArchive(const OutArchive& a_other) = default;
    OutArchive& operator=(const OutArchive& a_other) = default;
    ~OutArchive() = default;

    void Write(const SerFwObjsContainer& a_serFwObjects) const;

private:
    std::string m_datafile;
    std::shared_ptr<infra::IFormattedFileWriter> m_formattedFileWriter;
    std::shared_ptr<infra::ISerializationFormatter> m_formatter;
};

} // ser_fw


#endif // SF_OUT_ARCHIVE_HPP
