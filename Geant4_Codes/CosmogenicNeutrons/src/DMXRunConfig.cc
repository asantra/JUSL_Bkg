// DMXRunConfig.cc
#include "DMXRunConfig.hh"

DMXRunConfig *DMXRunConfig::Instance()
{
    static DMXRunConfig instance;
    return &instance;
}