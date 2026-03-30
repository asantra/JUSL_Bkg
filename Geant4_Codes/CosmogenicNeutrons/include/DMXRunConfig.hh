// DMXRunConfig.hh
#ifndef DMX_RUN_CONFIG_HH
#define DMX_RUN_CONFIG_HH

class DMXRunConfig
{
public:
    static DMXRunConfig *Instance(); // singleton
    void SetRunID(int val) { runID = val; }
    int GetRunID() const { return runID; }

private:
    DMXRunConfig() = default;
    int runID = 0;
};

#endif