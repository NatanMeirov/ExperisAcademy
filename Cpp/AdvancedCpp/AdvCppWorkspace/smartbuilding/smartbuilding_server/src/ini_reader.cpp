#include "ini_reader.hpp"
#include <string> // std::string, std::stoul
#include <unordered_map>
#include <algorithm> // std::for_each
#include "serialized_object.hpp"
#include "ini.h"


namespace smartbuilding
{

IConfigReader::SerializedObjectCollection IniReader::ReadConfig(const std::string& a_configFile)
{
    ini_parse(a_configFile.c_str(), IniHandler, &m_sectionsTable);

    return CreateSerializedObjectsFromTable();
}


int IniReader::IniHandler(void* a_sectionsTable, const char* a_section, const char* a_name, const char* a_value)
{
    ParsedTable* parsedTable = static_cast<ParsedTable*>(a_sectionsTable);
    std::string section(a_section);
    std::string name(a_name);
    std::string value(a_value);

    try
    {
        if(parsedTable->find(section) == parsedTable->end()) // A new section (device) has parsed - create new entry in the table
        {
            parsedTable->insert({section, NameValuePairsCollection()});
        }

        (*parsedTable)[section].push_back(std::make_pair(name, value));
    }
    catch(...)
    {
        return 0; // Error
    }

    return 1; // Success
}


IConfigReader::SerializedObjectCollection IniReader::CreateSerializedObjectsFromTable()
{
    IConfigReader::SerializedObjectCollection serializedObjects;

    for(const auto& tableEntry : m_sectionsTable)
    {
        SerializedObject object;
        object.m_id = tableEntry.first;

        std::for_each(tableEntry.second.begin(), tableEntry.second.end(), [&](const NameValuePair& a_nameValuePair)
        {
            if(a_nameValuePair.first == "type")
            {
                object.m_type = a_nameValuePair.second;
            }
            else if(a_nameValuePair.first == "room")
            {
                object.m_room = std::stoul(a_nameValuePair.second);
            }
            else if(a_nameValuePair.first == "floor")
            {
                object.m_floor = std::stoul(a_nameValuePair.second);
            }
            else if(a_nameValuePair.first == "log")
            {
                object.m_logFileName = a_nameValuePair.second;
            }
            else if(a_nameValuePair.first == "config")
            {
                object.m_configurations = a_nameValuePair.second;
            }
            else if(a_nameValuePair.first == "soname")
            {
                object.m_soName = a_nameValuePair.second;
            }
        });

        serializedObjects.push_back(object);
    }

    return serializedObjects;
}

} // smartbuilding
