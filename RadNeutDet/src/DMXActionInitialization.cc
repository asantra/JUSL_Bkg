
// ActionInitialization.cc

#include "DMXActionInitialization.hh"
#include "DMXPrimaryGeneratorAction.hh"
#include "DMXRunAction.hh"
#include "DMXEventAction.hh"
#include "DMXSteppingAction.hh"


DMXActionInitialization::DMXActionInitialization()
 : G4VUserActionInitialization()
{}


DMXActionInitialization::~DMXActionInitialization()
{}


void DMXActionInitialization::BuildForMaster() const
{
  DMXRunAction* runAction = new DMXRunAction;
  SetUserAction(runAction);
}



void DMXActionInitialization::Build() const
{
  DMXPrimaryGeneratorAction* primaryGenAction = new DMXPrimaryGeneratorAction; 
  SetUserAction(primaryGenAction);

  DMXRunAction* runAction = new DMXRunAction;
  SetUserAction(runAction);
  
  DMXEventAction* eventAction = new DMXEventAction(runAction, primaryGenAction);
  SetUserAction(eventAction);
  
  SetUserAction(new DMXSteppingAction(runAction, eventAction));
}
