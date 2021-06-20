#ifndef __NM_CDR_CDRFILEPARSER_HPP__
#define __NM_CDR_CDRFILEPARSER_HPP__


#include <unordered_map>
#include <vector>
#include <string>
#include "../../Infrastructure/inc/Cdr.hpp"


namespace nm {

namespace cdr {

class CdrFileParser {
public:
    CdrFileParser();

    std::vector<Cdr> ParseCdrFileToCdrs(const std::string& a_cdrFilePath) const;

private:
    void InitializeTypesMap();
    Cdr ConvertSingleLineToCdr(const std::string& a_singleLine) const;

    std::unordered_map<std::string, nm::cdr::Cdr::UsageType> m_cdrTypesMap;
};

} // cdr

} // nm


#endif // __NM_CDR_CDRFILEPARSER_HPP__