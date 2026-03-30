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

#include <vector>
#include "G4Types.hh"


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

//   //**For Lab Space**//
//   G4bool   flag_lab1;
//   G4bool   flag_rock;
// //  G4bool   flag_flo;
//   G4bool   flag_lab2;

//   //**For Detector Space**//
//   G4bool   flag_det1;
//   G4bool   flag_drock;
//   G4bool   flag_det2;
  //**For Lab Space**//
  G4bool   flag_air2;
  // G4bool   flag_pb;
  // G4bool   flag_air3;
  // G4bool   flag_pp2;
  // G4bool   flag_air4;
  // G4bool   flag_hdpe;
//  G4bool   flag_flo;
  // G4bool   flag_air2;

  //**For Detector Space**//
  G4bool   flag_det1;
  // G4bool   flag_dsteel;
  // G4bool   flag_dhdpe;
  // G4bool   flag_det2;

  G4bool   firstStep;

  G4int ptID;

  //***Lab Vectors***//
  //****Normal****//
  //****** air2 ***** *//
  std::vector<G4int> air2_eID;
  std::vector<G4int> air2_pID;
  std::vector<G4int> air2_tID;

  std::vector<G4double> air2_KE;

  std::vector<G4double> air2_px;
  std::vector<G4double> air2_py;
  std::vector<G4double> air2_pz;

  std::vector<G4double> air2_mx;
  std::vector<G4double> air2_my;
  std::vector<G4double> air2_mz;

  std::vector<G4double> air2_vz;

  //****** pb ***** *//
  std::vector<G4int> pb_eID;
  std::vector<G4int> pb_pID;
  std::vector<G4int> pb_tID;

  std::vector<G4double> pb_KE;

  std::vector<G4double> pb_px;
  std::vector<G4double> pb_py;
  std::vector<G4double> pb_pz;

  std::vector<G4double> pb_mx;
  std::vector<G4double> pb_my;
  std::vector<G4double> pb_mz;

  std::vector<G4double> pb_vz;


  //****** air3 ***** *//
  std::vector<G4int> air3_eID;
  std::vector<G4int> air3_pID;
  std::vector<G4int> air3_tID;

  std::vector<G4double> air3_KE;

  std::vector<G4double> air3_px;
  std::vector<G4double> air3_py;
  std::vector<G4double> air3_pz;

  std::vector<G4double> air3_mx;
  std::vector<G4double> air3_my;
  std::vector<G4double> air3_mz;

  std::vector<G4double> air3_vz;

  //****** pp2 ***** *//
  std::vector<G4int> pp2_eID;
  std::vector<G4int> pp2_pID;
  std::vector<G4int> pp2_tID;

  std::vector<G4double> pp2_KE;

  std::vector<G4double> pp2_px;
  std::vector<G4double> pp2_py;
  std::vector<G4double> pp2_pz;

  std::vector<G4double> pp2_mx;
  std::vector<G4double> pp2_my;
  std::vector<G4double> pp2_mz;

  std::vector<G4double> pp2_vz;

  //****** air4 ***** *//
  std::vector<G4int> air4_eID;
  std::vector<G4int> air4_pID;
  std::vector<G4int> air4_tID;

  std::vector<G4double> air4_KE;

  std::vector<G4double> air4_px;
  std::vector<G4double> air4_py;
  std::vector<G4double> air4_pz;

  std::vector<G4double> air4_mx;
  std::vector<G4double> air4_my;
  std::vector<G4double> air4_mz;

  std::vector<G4double> air4_vz;

  //****BackScatter****//
  // std::vector<G4int> elBeID;
  // std::vector<G4int> elBpID;
  // std::vector<G4int> elBtID;

  // std::vector<G4double> elBKE;

  // std::vector<G4double> elBpx;
  // std::vector<G4double> elBpy;
  // std::vector<G4double> elBpz;

  // std::vector<G4double> elBmx;
  // std::vector<G4double> elBmy;
  // std::vector<G4double> elBmz;

  // std::vector<G4double> elBvz;


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
  // std::vector<G4int> edBeID;
  // std::vector<G4int> edBpID;
  // std::vector<G4int> edBtID;

  // std::vector<G4double> edBKE;

  // std::vector<G4double> edBpx;
  // std::vector<G4double> edBpy;
  // std::vector<G4double> edBpz;

  // std::vector<G4double> edBmx;
  // std::vector<G4double> edBmy;
  // std::vector<G4double> edBmz;

  // std::vector<G4double> edBvz;


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

