//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
//
// StackingAction header
// --------------------------------------------------------------

#ifndef DMXStackingAction_H
#define DMXStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Navigator;
class G4Track;

class DMXStackingAction : public G4UserStackingAction
{

public:
  DMXStackingAction();
  virtual ~DMXStackingAction();
  
public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();
  
private:
  G4bool killGammasFlag;
  
  G4Navigator* gNavigator; 
  
public:
  inline void SetKillGammasFlag(G4bool val)     {killGammasFlag  = val;};
  
};

#endif

