#ifndef SF_JSON_FILE_HANDLER_HPP
#define SF_JSON_FILE_HANDLER_HPP


#include "iformatted_file_reader.hpp"
#include "iformatted_file_writer.hpp"


namespace ser_fw
{

// A concrete JSON file reader/writer class.
class JsonFileHandler : public infra::IFormattedFileReader, public infra::IFormattedFileWriter
{
public:
    virtual ~JsonFileHandler() = default;
    Types::ParsedLinesCollection ReadFile(const std::string& a_datafile) const override { return ReadJsonFile(a_datafile); }
    virtual void WriteFile(const Types::ParsedLinesCollection& a_parsedFormatLines, const std::string& a_datafile) const override { WriteJsonFile(a_parsedFormatLines, a_datafile); }

private:
    Types::ParsedLinesCollection ReadJsonFile(const std::string& a_jsonFile) const;
    void WriteJsonFile(const Types::ParsedLinesCollection& a_jsonLines, const std::string& a_jsonFile) const;
};

} // ser_fw


#endif // SF_JSON_FILE_HANDLER_HPP
