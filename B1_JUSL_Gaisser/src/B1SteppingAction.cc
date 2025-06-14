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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "TMath.h"
#include <cmath>
#include "G4Track.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction *eventAction, B1RunAction *runAction)
    : G4UserSteppingAction(),
      fEventAction(eventAction), fRunAction(runAction),
      fScoringVolume(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step *step)
{
  //    G4cout << "====INSIDE STEPPING ACTION====" << G4endl;
  //  if (!fScoringVolume1 || !fScoringVolume2 || !fScoringVolume3 || !fScoringVolume4)
  //  {
  //    const B1DetectorConstruction* detectorConstruction
  //      = static_cast<const B1DetectorConstruction*>
  //        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //    fScoringVolume1 = detectorConstruction->GetScoringVolume1();
  //    fScoringVolume1 = detectorConstruction->GetScoringVolume1();
  //    fScoringVolume2 = detectorConstruction->GetScoringVolume2();
  //    fScoringVolume3 = detectorConstruction->GetScoringVolume3();
  //    fScoringVolume4 = detectorConstruction->GetScoringVolume4();
  //  }

  if (!fScoringVolume)
  {
    const B1DetectorConstruction *detectorConstruction = static_cast<const B1DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
  }
  // if(step->GetTrack()->GetDefinition() == G4Gamma::GammaDefinition())
  //{
  // G4cout<<"Gamma Found"<<G4endl;
  // }

  // G4cout<<step->GetTrack()->GetParentID()<<"\t"
  //<<step->GetTrack()->GetTrackID()<<"\t"
  //<<step->GetTrack()->GetDefinition()->GetParticleName()<<"\t"
  //<<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<G4endl;

  // get volume of the current step

  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  PreEkin = -999;
  mom = -999;
  momz = 0.0;
  int eid, pid, tid;
  // check if we are in scoring volume
  if (volume == fScoringVolume)
  {
    atrack = step->GetTrack();
    if (atrack->GetDefinition()->GetParticleName() == "mu-" || atrack->GetDefinition()->GetParticleName() == "mu+") // changed to mu- and mu+ both
    {
      PreEkin = atrack->GetKineticEnergy();
      // G4double PostEkin = step->GetPostStepPoint()->GetKineticEnergy();
      // G4double KE = step->GetTrack()->GetKineticEnergy();
      // G4double dedx = 1.0*edepStep/Lstep;
      if (fEventAction->counter == 0)
      {
        eid = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
        tid = atrack->GetTrackID();
        pid = atrack->GetDefinition()->GetPDGEncoding();

        fEventAction->eID = eid;
        fEventAction->tID = tid;
        fEventAction->pID = pid;
        fEventAction->Ken = PreEkin;

        HitPos = atrack->GetPosition();          // Position
        HitVertex = atrack->GetVertexPosition(); // Vertex of track'

        // saving to variables defined in EventAction to further pass to Run vectors
        fEventAction->posx = HitPos.x();
        fEventAction->posy = HitPos.y();
        fEventAction->posz = HitPos.z();

        fEventAction->verx = HitVertex.x();
        fEventAction->very = HitVertex.y();
        fEventAction->verz = HitVertex.z();

        // HitMom=step->GetPreStepPoint()->GetMomentum();//Momentum
        HitMom = atrack->GetMomentum(); // Momentum
        mom = HitMom.mag();
        momx = HitMom.x();
        momy = HitMom.y();
        momz = HitMom.z();
        fEventAction->th = 1.0 * momz / mom;
        fEventAction->ph = atan2(momy, momx);
        // G4cout<<step->GetPreStepPoint()->GetPosition()<<"\t"<<step->GetPostStepPoint()->GetPosition()<<"\t"<<HitPos<<endl;
        // G4cout << "Time " << eid << "\t" << atrack->GetGlobalTime();
      }
      // if(!TMath::IsNaN(dedx))
      // fRunAction->out<<PreEkin/MeV<<"\t"<<edepStep/MeV<<"\t"<<Lstep/cm<<"\t"<<dedx<<std::endl;
      fEventAction->counter++;
    }
  }

  // if (volume == fScoringVolume1)
  //  {
  //  // collect energy deposited in this step
  //    G4double edepStep = step->GetTotalEnergyDeposit();
  //    fEventAction->AddEdep1(edepStep);
  //    if(step->GetTrack()->GetDefinition()->GetParticleName()== "mu-")
  //    {
  //      //G4cout<<"Muon Hit Scintillator"<<G4endl;
  //      //fRunAction->count++;
  //    }
  //  }
  //  else if (volume == fScoringVolume2)
  //  {
  //  // collect energy deposited in this step
  //    G4double edepStep = step->GetTotalEnergyDeposit();
  //    fEventAction->AddEdep2(edepStep);
  //  }
  //  else if (volume == fScoringVolume3)
  //  {
  //  // collect energy deposited in this step
  //    G4double edepStep = step->GetTotalEnergyDeposit();
  //    fEventAction->AddEdep3(edepStep);
  //  }
  //  else if (volume == fScoringVolume4)
  //  {
  //  // collect energy deposited in this step
  //    G4double edepStep = step->GetTotalEnergyDeposit();
  //    fEventAction->AddEdep4(edepStep);
  //  }
  else
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
