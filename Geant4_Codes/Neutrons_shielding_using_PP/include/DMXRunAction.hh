//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
//
// RunAction header
// --------------------------------------------------------------

#ifndef DMXRunAction_h
#define DMXRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4RunManager.hh"
#include "globals.hh"

#include <fstream>

using namespace std;

class G4Run;

class DMXRunAction : public G4UserRunAction
{

public:
  DMXRunAction();
  ~DMXRunAction();
  
public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
    
public:
  std::ofstream pmtHit;
  std::ofstream pmtHitSet;
  std::ofstream pmtHitWorker;
  std::ofstream pmtcount;
  std::ofstream pmthitProcess;
  std::ofstream energyDeposit;
  std::ofstream energyDepositProcess;
  std::ofstream OPvsEnergy;
  std::ofstream totalOPvsEnergy;
  std::ofstream scinthitgamma;
  std::ofstream OP662;
  std::ofstream totalOP662;
  std::ofstream pmtHitTime;
  std::ofstream photonEnergy;
  std::ofstream allPulses;
  ofstream NeutronEnDet;
  ofstream NeutronEnTrAir2;
  // ofstream NeutronEnTrPb;
  // ofstream NeutronEnTrAir3;
  // ofstream NeutronEnTrPP2;
  // ofstream NeutronEnTrAir4;
  // ofstream NeutronBack;
  // ofstream NeutronDetBack;

  //***Lab Vectors***//

  //****Normal*****//
  // air2 //
  std::vector<G4int> rair2_eID;
  std::vector<G4int> rair2_pID;
  std::vector<G4int> rair2_tID;

  std::vector<G4double> rair2_KE;

  std::vector<G4double> rair2_px;
  std::vector<G4double> rair2_py;
  std::vector<G4double> rair2_pz;

  std::vector<G4double> rair2_mx;
  std::vector<G4double> rair2_my;
  std::vector<G4double> rair2_mz;

  std::vector<G4double> rair2_vz;

  // //****Normal*****//
  // // pb //
  // std::vector<G4int> rpb_eID;
  // std::vector<G4int> rpb_pID;
  // std::vector<G4int> rpb_tID;

  // std::vector<G4double> rpb_KE;

  // std::vector<G4double> rpb_px;
  // std::vector<G4double> rpb_py;
  // std::vector<G4double> rpb_pz;

  // std::vector<G4double> rpb_mx;
  // std::vector<G4double> rpb_my;
  // std::vector<G4double> rpb_mz;

  // std::vector<G4double> rpb_vz;

  // //****Normal*****//
  // // air3 //
  // std::vector<G4int> rair3_eID;
  // std::vector<G4int> rair3_pID;
  // std::vector<G4int> rair3_tID;

  // std::vector<G4double> rair3_KE;

  // std::vector<G4double> rair3_px;
  // std::vector<G4double> rair3_py;
  // std::vector<G4double> rair3_pz;

  // std::vector<G4double> rair3_mx;
  // std::vector<G4double> rair3_my;
  // std::vector<G4double> rair3_mz;

  // std::vector<G4double> rair3_vz;

  // //****Normal*****//
  // // pp //
  // std::vector<G4int> rpp2_eID;
  // std::vector<G4int> rpp2_pID;
  // std::vector<G4int> rpp2_tID;

  // std::vector<G4double> rpp2_KE;

  // std::vector<G4double> rpp2_px;
  // std::vector<G4double> rpp2_py;
  // std::vector<G4double> rpp2_pz;

  // std::vector<G4double> rpp2_mx;
  // std::vector<G4double> rpp2_my;
  // std::vector<G4double> rpp2_mz;

  // std::vector<G4double> rpp2_vz;

  // //****Normal*****//
  // // air4 //
  // std::vector<G4int> rair4_eID;
  // std::vector<G4int> rair4_pID;
  // std::vector<G4int> rair4_tID;

  // std::vector<G4double> rair4_KE;

  // std::vector<G4double> rair4_px;
  // std::vector<G4double> rair4_py;
  // std::vector<G4double> rair4_pz;

  // std::vector<G4double> rair4_mx;
  // std::vector<G4double> rair4_my;
  // std::vector<G4double> rair4_mz;

  // std::vector<G4double> rair4_vz;


  //****BackScatter****//
  // std::vector<G4int> rlBeID;
  // std::vector<G4int> rlBpID;
  // std::vector<G4int> rlBtID;

  // std::vector<G4double> rlBKE;

  // std::vector<G4double> rlBpx;
  // std::vector<G4double> rlBpy;
  // std::vector<G4double> rlBpz;

  // std::vector<G4double> rlBmx;
  // std::vector<G4double> rlBmy;
  // std::vector<G4double> rlBmz;
  // std::vector<G4double> rlBvz;

  //***Detector Vectors***//

  //****Normal*****//
  std::vector<G4int> rdeID;
  std::vector<G4int> rdpID;
  std::vector<G4int> rdtID;

  std::vector<G4double> rdKE;

  std::vector<G4double> rdpx;
  std::vector<G4double> rdpy;
  std::vector<G4double> rdpz;

  std::vector<G4double> rdmx;
  std::vector<G4double> rdmy;
  std::vector<G4double> rdmz;

  std::vector<G4double> rdvz;

  //****BackScatter****//
  // std::vector<G4int> rdBeID;
  // std::vector<G4int> rdBpID;
  // std::vector<G4int> rdBtID;

  // std::vector<G4double> rdBKE;

  // std::vector<G4double> rdBpx;
  // std::vector<G4double> rdBpy;
  // std::vector<G4double> rdBpz;

  // std::vector<G4double> rdBmx;
  // std::vector<G4double> rdBmy;
  // std::vector<G4double> rdBmz;
  // std::vector<G4double> rdBvz;

//  ofstream energySpectrum;
//  ofstream particlePosition;


//  vector <double> labEn;
//  vector <double> detEn;
//  vector <double> genEn;
//  vector <double> posx;
//  vector <double> posy;
//  vector <double> posz;


  G4double time2;
  G4int eventCounts;

  G4int evtCts;
};


#endif

