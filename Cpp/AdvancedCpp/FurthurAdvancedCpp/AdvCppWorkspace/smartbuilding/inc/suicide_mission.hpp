#ifndef NM_SUICIDE_MISSION_HPP
#define NM_SUICIDE_MISSION_HPP


#include "icallable.hpp"


namespace advcpp
{

class SuicideMission : public ICallable
{
public:
    virtual void operator()() override;
};

} // advcpp


#endif // NM_SUICIDE_MISSION_HPP