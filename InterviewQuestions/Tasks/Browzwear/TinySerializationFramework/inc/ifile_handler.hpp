#ifndef SF_IFILE_HANDLER_HPP
#define SF_IFILE_HANDLER_HPP


#include <string>
#include "iser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface of a File Handler.
// A concrete File Handler should handle reading and writing its unique file format.
class IFileHandler
{
public:
    virtual ~IFileHandler() = default;
    virtual Types::ParsedLinesCollection ReadFile(const std::string& a_datafile) const = 0;
    virtual void WriteFile(const Types::ParsedLinesCollection& a_parsedFormatLines, const std::string& a_datafile) const = 0;
};

} // infra

} // ser_fw


#endif // SF_IFILE_HANDLER_HPP
