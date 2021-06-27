#ifndef __NM_CDR_IFORMATTER_HPP__
#define __NM_CDR_IFORMATTER_HPP__


#include <string>


namespace nm {

namespace cdr {

// An interface of a cdr formatter that each switch should supply to the provider, in order to format their cdr file to the processor's known cdr file format
class IFormatter {
public:
    virtual ~IFormatter() = default;
    virtual void Format(const std::string& a_pathOfFileToFormat) = 0;
};

} // cdr

} // nm


#endif // __NM_CDR_IFORMATTER_HPP__
