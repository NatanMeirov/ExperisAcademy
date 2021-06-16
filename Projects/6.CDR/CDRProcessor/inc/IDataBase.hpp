#ifndef __NM_CDR_IDATABASE_HPP__
#define __NM_CDR_IDATABASE_HPP__


#include <string> // std::string


namespace nm{

namespace cdr {

// An interface for a database object
template <typename QueryType, typename DataType> // QueryType can be a string ("SELECT * FROM ...") in MySQL server implementation
class IDataBase {
public:
    virtual ~IDataBase() = default;
    virtual bool Load(const std::string& a_databaseFileName) = 0;
    virtual bool Save(const std::string& a_databaseFileName) = 0;

    virtual DataType Get(const QueryType& a_query) = 0;
    virtual bool Update(const QueryType& a_query) = 0;
    virtual bool Delete(const QueryType& a_query) = 0;
    virtual bool Add(const QueryType& a_query, const DataType& _dataToAdd) = 0;
};

} // cdr

} // nm


#endif // __NM_CDR_IDATABASE_HPP__