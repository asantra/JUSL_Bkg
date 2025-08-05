// B1RunConfig.hh
#ifndef B1_RUN_CONFIG_HH
#define B1_RUN_CONFIG_HH

#include "G4Run.hh"

class G4Run;

class B1RunConfig
{
public:
    static B1RunConfig *Instance(); // singleton
    //void SetRunID(int val) { runID = val; }
    void SetRunID(int val);
    int GetRunID() { return runID; }

private:
    B1RunConfig() = default;
    int runID = 0;
};

#endif
