// DMXPhysicsList.cc
// Updated for Geant4 v11.x (uses modular physics list)
// Author: Monalisa, based on original by Shubham Dutta

// DMXPhysicsList.cc
// Geant4 v11 compatible version

#include "DMXPhysicsList.hh"
#include "G4SystemOfUnits.hh"

// Additional physics modules
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

DMXPhysicsList::DMXPhysicsList()
{
  defaultCutValue = 1.0 * mm;

  // Add Optical Physics
  RegisterPhysics(new G4OpticalPhysics());

  // Add Radioactive Decay Physics
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

DMXPhysicsList::~DMXPhysicsList() = default;

void DMXPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}


