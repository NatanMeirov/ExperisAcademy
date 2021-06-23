#ifndef __NM_CDR_INFOOBJ_HPP__
#define __NM_CDR_INFOOBJ_HPP__


namespace nm {

namespace cdr {

// A polymorphic base for all InfoObjects of CDR System
struct InfoObj {
    virtual ~InfoObj() = default;
};

} // cdr

} // nm


#endif // __NM_CDR_INFOOBJ_HPP__