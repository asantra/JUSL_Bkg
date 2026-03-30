// B1RunMessenger.hh
#ifndef B1_RUN_MESSENGER_HH
#define B1_RUN_MESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "B1RunConfig.hh"

class B1RunMessenger : public G4UImessenger
{
public:
    B1RunMessenger();
    ~B1RunMessenger();

    void SetNewValue(G4UIcommand *command, G4String value) override;

private:
    G4UIcmdWithAnInteger *runIDCmd;
};

#endif
