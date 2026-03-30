//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
// --------------------------------------------------------------
//
// SHUBHAM DUTTA
//
// PrimaryGeneratorAction header
// --------------------------------------------------------------

#ifndef DMXPrimaryGeneratorMessenger_h
#define DMXPrimaryGeneratorMessenger_h 1

//#include "TFile.h"
//#include "TH1.h"
#include "TRandom3.h"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
//#include "DMXRunAction.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>
#include <cmath>

class G4ParticleGun;

class G4Event;

class DMXPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

   public:
      DMXPrimaryGeneratorAction();
      ~DMXPrimaryGeneratorAction();
  
   public:
      void GeneratePrimaries(G4Event* anEvent);
  
   private:
  G4ParticleGun* particleGun;

   private:
  //      const long* seeds;
  long seeds[2];
  G4double energy;

   public:
  const long* GetEventSeeds()  {return seeds;}
  G4double GetEnergyPrimary()  {return energy;}
  G4double GetPosX()  {return x;}
  G4double GetPosY()  {return y;}
  G4double GetPosZ()  {return z;}
  
  std::ifstream neutronSpectrum;
  G4double neutron[80][2];
  G4double x;
  G4double y;
  G4double z;
  G4double rad;
  G4double theta;
  G4double phi;
  G4double max;
  G4double en;
  G4double val_r;
  G4double val_b;
  G4double pos;
  G4double loc;
  G4double s;
  G4double ra;

//  TFile *f;
//  TH1F *h1;

  std::ifstream in;

  std::ofstream energySpectrum;
  std::ofstream particlePosition;
  std::ofstream particleDirection;

};

#endif

