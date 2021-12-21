#ifndef SF_IN_ARCHIVE_HPP
#define SF_IN_ARCHIVE_HPP


#include <string>
#include <memory> // std::shared_ptr
#include <vector>
#include "iformatted_file_reader.hpp"
#include "ideserialization_formatter.hpp"
#include "ser_fw_obj.hpp"


namespace ser_fw
{

// A class that handles reading SerFwObjs from a data file,
// and loading those objects into a InArchive::SerFwObjsContainer.
class InArchive
{
public:
    using SerFwObjsContainer = std::vector<std::shared_ptr<infra::SerFwObj>>;

public:
    InArchive(const std::string& a_datafile, std::shared_ptr<infra::IFormattedFileReader> a_formattedFileReader, std::shared_ptr<infra::IDeserializationFormatter> a_formatter);
    InArchive(const InArchive& a_other) = default;
    InArchive& operator=(const InArchive& a_other) = default;
    ~InArchive() = default;

    void Read(SerFwObjsContainer& a_serFwObjects) const;

private:
    std::string m_datafile;
    std::shared_ptr<infra::IFormattedFileReader> m_formattedFileReader;
    std::shared_ptr<infra::IDeserializationFormatter> m_formatter;
};

} // ser_fw


#endif // SF_IN_ARCHIVE_HPP
