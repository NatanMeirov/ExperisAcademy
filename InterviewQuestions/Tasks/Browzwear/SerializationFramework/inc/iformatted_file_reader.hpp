#ifndef SF_IFORMATTED_FILE_READER_HPP
#define SF_IFORMATTED_FILE_READER_HPP


#include <string>
#include "iser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface of a Formatted File Reader.
// A concrete Formatted File Reader should handle the extraction
// of parsed lines (represented as strings) - from its unique file format.
class IFormattedFileReader
{
public:
    virtual ~IFormattedFileReader() = default;
    virtual Types::ParsedLinesCollection ReadFile(const std::string& a_datafile) const = 0;
};

} // infra

} // ser_fw


#endif // SF_IFORMATTED_FILE_READER_HPP
