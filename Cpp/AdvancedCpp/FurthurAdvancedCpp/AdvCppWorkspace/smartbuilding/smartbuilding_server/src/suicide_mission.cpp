#include "suicide_mission.hpp"
#include <stdexcept>


void advcpp::SuicideMission::operator()()
{
    throw std::runtime_error("Kamikaze!!!");
}
