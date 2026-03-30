// B1RunConfig.cc
#include "B1RunConfig.hh"

B1RunConfig *B1RunConfig::Instance()
{
    static B1RunConfig instance;
    return &instance;
}
