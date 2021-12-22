#include "example_file_handler.hpp"
#include <string> // std::getline
#include <fstream> // std::ofstream, std::ifstream
#include <algorithm> // std::for_each


namespace ser_fw_test
{

ser_fw::Types::ParsedLinesCollection ExampleFileHandler::ReadFile(const std::string& a_datafile) const
{
    ser_fw::Types::ParsedLinesCollection parsedLines;

    std::ifstream file(a_datafile);
    std::string line;

    while(std::getline(file, line, '\n'))
    {
        parsedLines.push_back(line);
    }

    return parsedLines;
}


void ExampleFileHandler::WriteFile(const ser_fw::Types::ParsedLinesCollection& a_parsedFormatLines, const std::string& a_datafile) const
{
    std::ofstream file(a_datafile);
    std::for_each(a_parsedFormatLines.begin(), a_parsedFormatLines.end(),
    [&](const std::string& a_formattedStr)
    {
        file << (a_formattedStr + '\n');
    });
}

} // ser_fw_test
