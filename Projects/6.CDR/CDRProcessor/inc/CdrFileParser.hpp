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
    static const unsigned int THREADS_NUMBER = 4;
    static const unsigned int WORKING_TASKS_QUEUE_SIZE = 39;

    void InitializeTypesMap();
    Cdr ConvertSingleLineToCdr(const std::string& a_singleLine) const;

    std::unordered_map<std::string, nm::cdr::Cdr::UsageType> m_cdrTypesMap;
};

} // cdr

} // nm


#endif // __NM_CDR_CDRFILEPARSER_HPP__