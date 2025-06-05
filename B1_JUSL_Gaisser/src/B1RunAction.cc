//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <fstream>
#include <ctime>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction(),
  fEdep(0.),
  fEdep2(0.)
{ 

  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);

  //=========FILES TO STORE FINAL STATES AND PRIMARIES=============//
  final.open("/home/sayang/JUSL_Simulation/Muons/555mData/NewRock/SiO2_norm/Muons_554m_NR_terr_c5_1.dat",fstream::app);
  primaries.open("/home/sayang/JUSL_Simulation/Muons/555mData/NewRock/SiO2_norm/GeantGeneratedMuons_554m_NR_terr_c5_1.dat",fstream::app);
  //out2.open("/home/shubham/G4WORK/Sayan/MuonKinetic.dat");

  count=0;

  for(int i=0;i<100;i++)
  {
    arr1[i]=0.0;
    arr2[i]=0.0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
  // Time based random seed for each run
  G4cout << "====BEGIN OF RUN===" << G4endl;
  long seeds[2];
  time_t systime = time(NULL);
  seeds[0] = (long) systime;
  seeds[1] = (long) (systime*G4UniformRand());
  G4Random::setTheSeeds(seeds);
  //G4Random::showEngineStatus();

  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  //G4cout<<count<<G4endl;
  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();
  
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  

//  const B1DetectorConstruction* detectorConstruction
//   = static_cast<const B1DetectorConstruction*>
//     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
//  G4double mass = detectorConstruction->GetScoringVolume1()->GetMass();
//  G4double dose = edep/mass;
//  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------"
     << G4endl;
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------"
     << G4endl;
  }
  
  /**G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated dose per run, in scoring volume : " 
     << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;*/

  //========Saving Primaries to file===========//
  G4cout << "=============Number of Primaries= " << Pid.size() <<G4endl;
  for(int i=0;i<Pid.size();i++)
  {
      primaries << Pid[i] << "\t" << GunE[i]/GeV << "\t"
                << ZeTh[i] << "\t" << GunX[i] << "\t"
                << GunY[i] << G4endl;
  }

  //==========Saving final values to file========//
  G4cout << "======Number of final events " << En.size() << G4endl;
  for(int i=0;i<En.size();i++)
  {
    final << feID[i] << "\t" << ftID[i] << "\t" << fpID[i] << "\t"
          << En[i]/GeV << "\t" << Edec[i]/GeV << "\t"
          << thet[i] << "\t" << phi[i] << "\t" 
	  << xp[i]/m << "\t" << yp[i]/m << "\t" << zp[i]/m << "\t" 
	  << xv[i]/m << "\t" << yv[i]/m << "\t" << zv[i]/m << endl;
  }

  /*for(int i=0;i<En.size();i++)
  {
    out1<<En[i]/GeV<<endl;
  }
  //out.close();
  for(int i=0;i<Edec.size();i++)
  {
    out2<<Edec[i]/GeV<<endl;
    }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

