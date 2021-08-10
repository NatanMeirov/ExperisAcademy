#ifndef NM_INI_READER_HPP
#define NM_INI_READER_HPP


#include <string> // std::string
#include <deque> // std::deque
#include <unordered_map>
#include "iconfig_reader.hpp"
#include "serialized_object.hpp"


namespace smartbuilding
{

class IniReader : public IConfigReader
{
public:
    using NameValuePair = std::pair<std::string, std::string>;
    using NameValuePairsCollection = std::deque<NameValuePair>;
    using ParsedTable = std::unordered_map<std::string, NameValuePairsCollection>;

    IniReader() = default;
    IniReader(const IniReader& a_other) = delete;
    IniReader& operator=(const IniReader& a_other) = delete;
    ~IniReader() = default;

    virtual SerializedObjectCollection ReadConfig(const std::string& a_configFile) override;

private:
    // Using C Api
    static int IniHandler(void* a_sectionsTable, const char* a_section, const char* a_name, const char* a_value);
    SerializedObjectCollection CreateSerializedObjectsFromTable();

private:
    ParsedTable m_sectionsTable;
};

} // smartbuilding


#endif // NM_INI_READER_HPP
