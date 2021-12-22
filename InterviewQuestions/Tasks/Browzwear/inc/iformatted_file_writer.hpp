#ifndef SF_IFORMATTED_FILE_WRITER_HPP
#define SF_IFORMATTED_FILE_WRITER_HPP


#include <string>
#include "ser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface of a Formatted File Writer.
// A concrete Formatted File Writer should handle the insertion
// of each pre-initialized parsed line (represented as string) - to its unique file format.
class IFormattedFileWriter
{
public:
    virtual ~IFormattedFileWriter() = default;
    virtual void WriteFile(const Types::ParsedLinesCollection& a_parsedFormatLines, const std::string& a_datafile) const = 0;
};

} // infra

} // ser_fw


#endif // SF_IFORMATTED_FILE_WRITER_HPP

