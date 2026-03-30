// DMXRunMessenger.cc
#include "DMXRunMessenger.hh"
#include "G4UIdirectory.hh"

DMXRunMessenger::DMXRunMessenger()
{
    auto dir = new G4UIdirectory("/myapp/");
    dir->SetGuidance("Custom app controls.");

    runIDCmd = new G4UIcmdWithAnInteger("/myapp/setRunID", this);
    runIDCmd->SetGuidance("Set custom run ID.");
    runIDCmd->SetParameterName("runID", false);
}

DMXRunMessenger::~DMXRunMessenger()
{
    delete runIDCmd;
}

void DMXRunMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
    if (command == runIDCmd)
    {
        DMXRunConfig::Instance()->SetRunID(runIDCmd->GetNewIntValue(value));
    }
}