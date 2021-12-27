#ifndef SF_EXAMPLE_FILE_HANDLER_HPP
#define SF_EXAMPLE_FILE_HANDLER_HPP


#include "ifile_handler.hpp"


namespace ser_fw_test
{

// A test class of a file reader/writer that builds a file with lines in form: {type|param1,val1|param2,val2|...}, from a ParsedLinesCollection
class ExampleFileHandler : public ser_fw::infra::IFileHandler
{
public:
    virtual ser_fw::Types::ParsedLinesCollection ReadFile(const std::string& a_datafile) const override;
    virtual void WriteFile(const ser_fw::Types::ParsedLinesCollection& a_parsedFormatLines, const std::string& a_datafile) const override;
};

} // ser_fw_test


#endif // SF_EXAMPLE_FILE_HANDLER_HPP
