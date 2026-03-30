// B1RunMessenger.cc
#include "B1RunMessenger.hh"
#include "G4UIdirectory.hh"

B1RunMessenger::B1RunMessenger()
{
    auto dir = new G4UIdirectory("/myapp/");
    dir->SetGuidance("Custom app controls.");

    runIDCmd = new G4UIcmdWithAnInteger("/myapp/setRunID", this);
    runIDCmd->SetGuidance("Set custom run ID.");
    runIDCmd->SetParameterName("runID", false);
}

B1RunMessenger::~B1RunMessenger()
{
    delete runIDCmd;
}

void B1RunMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
    if (command == runIDCmd)
    {
        B1RunConfig::Instance()->SetRunID(runIDCmd->GetNewIntValue(value));
    }
}
