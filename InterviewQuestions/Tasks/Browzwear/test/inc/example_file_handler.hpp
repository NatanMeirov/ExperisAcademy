#ifndef SF_EXAMPLE_FILE_HANDLER_HPP
#define SF_EXAMPLE_FILE_HANDLER_HPP


#include "iformatted_file_reader.hpp"
#include "iformatted_file_writer.hpp"


namespace ser_fw_test
{

// A test class of a file reader/writer that builds a file with CSV line in form: {type:...,param|val...}, from a TypedParamsMap
class ExampleFileHandler
    : public ser_fw::infra::IFormattedFileReader, public ser_fw::infra::IFormattedFileWriter
{
public:
    ser_fw::Types::ParsedLinesCollection ReadFile(const std::string &a_datafile) const override;
    void WriteFile(const ser_fw::Types::ParsedLinesCollection &a_parsedFormatLines, const std::string &a_datafile) const override;
};

} // ser_fw_test


#endif // SF_EXAMPLE_FILE_HANDLER_HPP
