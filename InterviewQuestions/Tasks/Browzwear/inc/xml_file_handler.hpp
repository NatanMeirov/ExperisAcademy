#ifndef SF_XML_FILE_HANDLER_HPP
#define SF_XML_FILE_HANDLER_HPP


#include "iformatted_file_reader.hpp"
#include "iformatted_file_writer.hpp"


namespace ser_fw
{

// A concrete XML file reader/writer class.
class XMLFileHandler : public infra::IFormattedFileReader, public infra::IFormattedFileWriter
{
public:
    virtual ~XMLFileHandler () = default;
    Types::ParsedLinesCollection ReadFile(const std::string& a_datafile) const override { return ReadXmlFile(a_datafile); }
    virtual void WriteFile(const Types::ParsedLinesCollection& a_parsedFormatLines, const std::string& a_datafile) const override { WriteXmlFile(a_parsedFormatLines, a_datafile); }

private:
    Types::ParsedLinesCollection ReadXmlFile(const std::string& a_xmlFile) const;
    void WriteXmlFile(const Types::ParsedLinesCollection& a_xmlLines, const std::string& a_xmlFile) const;
};

} // ser_fw


#endif // SF_XML_FILE_HANDLER_HPP
