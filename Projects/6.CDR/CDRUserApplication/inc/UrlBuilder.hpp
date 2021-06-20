#ifndef __NM_CDR_URLBUILDER_HPP__
#define __NM_CDR_URLBUILDER_HPP__


#include "IRequestBuilder.hpp"
#include "UrlBuilderParams.hpp"


namespace nm {

namespace cdr {

class UrlBuilder : public IRequestBuilder {
public:
    virtual ~UrlBuilder() = default;
    UrlBuilder(UrlBuilderParams* a_params);
    virtual std::string BuildRequest() override;

private:
    UrlBuilderParams* m_params;
};

} // cdr

} // nm


#endif // __NM_CDR_URLBUILDER_HPP__