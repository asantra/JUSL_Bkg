//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
// --------------------------------------------------------------
//
//  SHUBHAM DUTTA  
//
// SteppingAction header
// --------------------------------------------------------------

#ifndef DMXSteppingAction_h
#define DMXSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4Track.hh"

#include "G4LogicalVolume.hh"


class DMXEventAction;
class DMXRunAction;

class DMXSteppingAction : public G4UserSteppingAction
{

public:
  DMXSteppingAction(DMXRunAction*, DMXEventAction*);
  virtual ~DMXSteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);
  
private:
  DMXRunAction*      runAction;  //pointer to run action
  DMXEventAction*    evtAction;  //pointer to event action
    
  G4LogicalVolume* fScoringVolume;
  G4LogicalVolume* fScoringVolumeLab;
  G4LogicalVolume* fScoringVolumePMT;
  G4LogicalVolume* fScoringVolumeCav;
  G4LogicalVolume* fScoringVolumeFlo;
  G4int pTrackID;
  G4int pTrackID_pmt;
  G4double electronEnergy;
  G4Track* atrack;

  G4int tID;
  G4int eID;
  G4int pID;

  G4double KE;

  G4double posx;
  G4double posy;
  G4double posz;

  G4double momx;
  G4double momy;
  G4double momz;

  G4double dx;
  G4double dy;
  G4double dz;

  G4double vx;
  G4double vy;
  G4double vz;

  G4ThreeVector pos;
  G4ThreeVector mom;
  G4ThreeVector dir;
  G4ThreeVector ver;

};

#endif
