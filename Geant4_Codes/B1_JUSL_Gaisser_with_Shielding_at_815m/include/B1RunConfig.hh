// B1RunConfig.hh
#ifndef B1_RUN_CONFIG_HH
#define B1_RUN_CONFIG_HH

class B1RunConfig
{
public:
    static B1RunConfig *Instance(); // singleton
    void SetRunID(int val) { runID = val; }
    int GetRunID() const { return runID; }

private:
    B1RunConfig() = default;
    int runID = 0;
};

#endif
