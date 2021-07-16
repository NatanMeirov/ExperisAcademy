#ifndef NM_BUSY_WORK_HPP
#define NM_BUSY_WORK_HPP


#include "icallable.hpp"


namespace advcpp
{

class BusyWork : public ICallable
{
public:
    virtual void operator()() override;
};

} // advcpp


#endif // NM_BUSY_WORK_HPP