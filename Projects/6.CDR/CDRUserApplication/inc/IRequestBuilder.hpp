#ifndef __NM_CDR_IREQUESTBUILDER_HPP__
#define __NM_CDR_IREQUESTBUILDER_HPP__


#include <string> // std::string


namespace nm {

namespace cdr {

class IRequestBuilder {
public:
    virtual ~IRequestBuilder() = default;
    virtual std::string BuildRequest() = 0;
};

} // cdr

} // nm


#endif // __NM_CDR_IREQUESTBUILDER_HPP__