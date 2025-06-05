//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//      For information related to this code contact: Alex Howard
//      e-mail: alexander.howard@cern.ch
// --------------------------------------------------------------
//
// SHUBHAM DUTTA
//
// PhysicsList header
// --------------------------------------------------------------

#ifndef DMXPhysicsList_h
#define DMXPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class DMXPhysicsList: public G4VUserPhysicsList
{
public:
  DMXPhysicsList();
  ~DMXPhysicsList();
  //  virtual ~DMXPhysicsList();


public:
  virtual void SetCuts();


protected:
  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
    
  // these methods Construct physics processes and register them
  virtual void ConstructGeneral();
  virtual void ConstructEM();
  virtual void ConstructHad();
  virtual void ConstructOp();


  /*
  // these methods Construct all particles in each category
  virtual void ConstructAllBosons();
  virtual void ConstructAllLeptons();
  virtual void ConstructAllMesons();
  virtual void ConstructAllBaryons();
  virtual void ConstructAllIons();
  virtual void ConstructAllShortLiveds();
  */

  virtual void AddTransportation();

private:
  G4int VerboseLevel;
  G4int OpVerbLevel;

  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;

  // these methods Construct particles 
  void ConstructMyBosons();
  void ConstructMyLeptons();
  void ConstructMyHadrons();
  void ConstructMyShortLiveds();

};

#endif
