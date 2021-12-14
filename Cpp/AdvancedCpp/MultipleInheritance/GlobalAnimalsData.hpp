#ifndef __GLOBALANIMALSDATA_HPP__
#define __GLOBALANIMALSDATA_HPP__


#include <string>
#include <vector>


#ifdef LIONDATA
//LionData
extern double g_lionLifeExpectency = 10;
extern std::vector<std::string> g_lionsContinent = { "Africa", "Asia" };
extern std::vector<std::string> g_lionsFood = { "meat" , "fish" };
extern double g_lionSpeed = 100;
extern double g_lionPregnancyDuration = 6;
extern size_t g_lionBornInLitter = 4;
#endif // LIONDATA


#ifdef WHALEDATA
//WhaleData
extern double g_whaleLifeExpectency = 10;
extern std::vector<std::string> g_whaleContinent = { "Ocean"};
extern std::vector<std::string> g_whaleFood = { "fish" , "bread" };
extern double g_whaleSpeed = 100;
extern double g_whalePregnancyDuration = 6;
extern size_t g_whaleBornInLitter = 4;
extern double g_whaleLowestDepth = 10;
#endif // WHALEDATA


#endif // __GLOBALANIMALSDATA_HPP__