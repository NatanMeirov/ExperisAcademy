#ifndef __NM_CDR_REQUESTBUILDERFACTORY_HPP__
#define __NM_CDR_REQUESTBUILDERFACTORY_HPP__


#include "IRequestBuilder.hpp"
#include "RequestBuilderParams.hpp"
#include "UrlBuilder.hpp" // Concrete class
#include "UrlBuilderParams.hpp" // Concrete class' params


namespace nm {

namespace cdr {

class RequestBuilderFactory {
public:
    static IRequestBuilder* CreateRequestBuilder(RequestBuilderParams* a_params) { return new UrlBuilder(static_cast<UrlBuilderParams*>(a_params)); } // Change would be only in this place
};

} // cdr

} // nm


#endif // __NM_CDR_REQUESTBUILDERFACTORY_HPP__