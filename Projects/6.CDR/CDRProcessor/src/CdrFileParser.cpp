#include "../inc/CdrFileParser.hpp"
#include <string>
#include <vector>
#include <fstream> // std::ifstream
#include <sstream>
#include "../../Infrastructure/inc/Cdr.hpp"


nm::cdr::CdrFileParser::CdrFileParser()
: m_cdrTypesMap() {
    this->InitializeTypesMap();
}


// Types: MOC, MTC, SMS_MO, SMS_MT, D, U, B, X
void nm::cdr::CdrFileParser::InitializeTypesMap() {
    this->m_cdrTypesMap["MOC"] = Cdr::UsageType::MOC;
    this->m_cdrTypesMap["MTC"] = Cdr::UsageType::MTC;
    this->m_cdrTypesMap["SMS-MO"] = Cdr::UsageType::SMS_MO;
    this->m_cdrTypesMap["SMS-MT"] = Cdr::UsageType::SMS_MT;
    this->m_cdrTypesMap["D"] = Cdr::UsageType::D;
    this->m_cdrTypesMap["U"] = Cdr::UsageType::U;
    this->m_cdrTypesMap["B"] = Cdr::UsageType::B;
    this->m_cdrTypesMap["X"] = Cdr::UsageType::X;
}


std::vector<nm::cdr::Cdr> nm::cdr::CdrFileParser::ParseCdrFileToCdrs(const std::string& a_cdrFilePath) const {
    std::ifstream ifs(a_cdrFilePath);
    std::string singleLine;
    std::vector<nm::cdr::Cdr> parsedCdrs;

    std::getline(ifs, singleLine); // Extract header (file length)
    size_t linesNumber = std::stoul(singleLine); // Not in use here

    while(std::getline(ifs, singleLine)) {
        parsedCdrs.push_back(this->ConvertSingleLineToCdr(singleLine));
    }

    return parsedCdrs;
}


nm::cdr::Cdr nm::cdr::CdrFileParser::ConvertSingleLineToCdr(const std::string& a_singleLine) const {
    std::stringstream lineAsStream(a_singleLine);
    Cdr cdr;
    std::string subStringContainer;
    char delimiter = '|';

    // Sequence Number:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_sequenceNumber = std::stoul(subStringContainer);

    // IMSI:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_imsi = std::stoul(subStringContainer);

    // IMEI:
    std::getline(lineAsStream, cdr.m_imei, delimiter);

    // Type:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_type = this->m_cdrTypesMap.at(subStringContainer);

    // MSISDN:
    std::getline(lineAsStream, cdr.m_msisdn, delimiter);

    // Call Date:
    std::getline(lineAsStream, cdr.m_callDate, delimiter);

    // Call Time:
    std::getline(lineAsStream, cdr.m_callTime, delimiter);

    // Duration:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_duration = std::stoul(subStringContainer);

    // Bytes Received:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_bytesReceived = std::stoul(subStringContainer);

    // Bytes Transmitted:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_bytesTransmitted = std::stoul(subStringContainer);

    // IMEI of second party:
    std::getline(lineAsStream, subStringContainer, delimiter);
    cdr.m_imsiOfSecondParty = std::stoul(subStringContainer);

    // MSISDN of second party:
    std::getline(lineAsStream, cdr.m_msisdnOfSecondParty, delimiter);

    return cdr;
}