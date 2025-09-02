// DMXRunMessenger.hh
#ifndef DMX_RUN_MESSENGER_HH
#define DMX_RUN_MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "DMXRunConfig.hh"

class DMXRunMessenger : public G4UImessenger
{
public:
    DMXRunMessenger();
    ~DMXRunMessenger();

    void SetNewValue(G4UIcommand *command, G4String value) override;

private:
    G4UIcmdWithAnInteger *runIDCmd;
};

#endif