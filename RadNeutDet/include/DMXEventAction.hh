//
//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
// 
// EventAction header
// --------------------------------------------------------------

#ifndef DMXEventAction_h
#define DMXEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ios.hh"

#include <fstream>

class DMXRunAction;
class DMXPrimaryGeneratorAction;

class DMXEventAction : public G4UserEventAction
{

public:
  DMXEventAction(DMXRunAction*, DMXPrimaryGeneratorAction*);
  virtual ~DMXEventAction();
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
  
public:
  void SetPrintModulo  (G4int val)        {printModulo     = val;}

  std::fstream tempStepProcess;
  std::fstream pmtHitTimeTemp;
  G4bool       PMThit;
  G4bool       scintillatorHit;
  G4int        event_id;
  G4int        count;
  G4int        totalCount;
  G4double     timeMax;
  G4bool       countStep;
  G4int labcount;
  G4int detcount;
//  G4double Enlab;
  G4double Endet;

  //**For Lab Space**//
  G4bool   flag_lab1;
  G4bool   flag_rock;
//  G4bool   flag_flo;
  G4bool   flag_lab2;

  //**For Detector Space**//
  G4bool   flag_det1;
  G4bool   flag_drock;
  G4bool   flag_det2;

  G4bool   firstStep;

  G4int ptID;

  //***Lab Vectors***//
  //****Normal****//
  std::vector<G4int> eleID;
  std::vector<G4int> elpID;
  std::vector<G4int> eltID;

  std::vector<G4double> elKE;

  std::vector<G4double> elpx;
  std::vector<G4double> elpy;
  std::vector<G4double> elpz;

  std::vector<G4double> elmx;
  std::vector<G4double> elmy;
  std::vector<G4double> elmz;

  std::vector<G4double> elvz;

  //****BackScatter****//
  std::vector<G4int> elBeID;
  std::vector<G4int> elBpID;
  std::vector<G4int> elBtID;

  std::vector<G4double> elBKE;

  std::vector<G4double> elBpx;
  std::vector<G4double> elBpy;
  std::vector<G4double> elBpz;

  std::vector<G4double> elBmx;
  std::vector<G4double> elBmy;
  std::vector<G4double> elBmz;

  std::vector<G4double> elBvz;


  //****Detector Vectors****//
  //***Normal***//
  std::vector<G4int> edeID;
  std::vector<G4int> edpID;
  std::vector<G4int> edtID;

  std::vector<G4double> edKE;

  std::vector<G4double> edpx;
  std::vector<G4double> edpy;
  std::vector<G4double> edpz;

  std::vector<G4double> edmx;
  std::vector<G4double> edmy;
  std::vector<G4double> edmz;

  std::vector<G4double> edvz;

  //****BackScatter****//
  std::vector<G4int> edBeID;
  std::vector<G4int> edBpID;
  std::vector<G4int> edBtID;

  std::vector<G4double> edBKE;

  std::vector<G4double> edBpx;
  std::vector<G4double> edBpy;
  std::vector<G4double> edBpz;

  std::vector<G4double> edBmx;
  std::vector<G4double> edBmy;
  std::vector<G4double> edBmz;

  std::vector<G4double> edBvz;


  void AddEdep(G4double edep)
  {
    fEdep  += edep;
  }
  
  void AddEdepElectron(G4double edepElectron)
  {
    fEdepElectron  += edepElectron;
  }
  
private:
  
  const long* seeds;
  G4double energy_pri;
//  G4double genx;
//  G4double geny;
//  G4double genz;
  
  G4int printModulo;                         
  DMXRunAction*    runAct;  //pointer to run action
  DMXPrimaryGeneratorAction* genAction; // pointer to particle generator
  
  G4double fEdep;
  G4double fEdepElectron;
  
  
};

#endif

