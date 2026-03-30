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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"

#include "G4Threading.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include "TMath.h"

#include <vector>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),fGenAction(0),
  fEdep1(0.),fEdep2(0.),fEdep3(0.),fEdep4(0.),en_pri(0.0)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{

  eid  = G4EventManager::GetEventManager()
          ->GetConstCurrentEvent()->GetEventID();

  thid = G4Threading::G4GetThreadId();
  G4cout << "Begin of event " << eid
         << " of Thread " << thid << G4endl;

  fEdep1 = 0.;
  fEdep2 = 0.;
  fEdep3 = 0.;
  fEdep4 = 0.;
  counter = 0;
  Ken = -999;

  //taking the energy of the gun, primary particle energy
  if (!fGenAction)
    fGenAction = dynamic_cast<const B1PrimaryGeneratorAction*>
      (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

  PrPid  = fGenAction->GetPriPid();
  en_pri = fGenAction->GetEnergyPrimary();
  PriZen = fGenAction->GetZenithPrimary();
  priX   = fGenAction->GetPrimaryX();
  priY   = fGenAction->GetPrimaryY();

  fRunAction->count++;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{
  //=======Primaries to vector=========//
  fRunAction->Pid.push_back(PrPid);
  fRunAction->GunE.push_back(en_pri);
  fRunAction->ZeTh.push_back(PriZen);
  fRunAction->GunX.push_back(priX);
  fRunAction->GunY.push_back(priY);

  // accumulate statistics in run action passing to vectors to print to file
  G4double eCut=50.0*keV;
  //if(fEdep1>eCut && fEdep2>eCut && fEdep3>eCut && fEdep4>eCut)
  //fRunAction->out<<fEdep1/MeV<<"\t"<<fEdep2/MeV<<"\t"<<fEdep3/MeV<<"\t"<<fEdep4/MeV<<std::endl;
  if(Ken>0 && !TMath::IsNaN(th))
  {
    fRunAction->feID.push_back(eID);
    fRunAction->ftID.push_back(tID);
    fRunAction->fpID.push_back(pID);
    fRunAction->En.push_back(en_pri);
    fRunAction->Edec.push_back(Ken);
    fRunAction->xp.push_back(posx);
    fRunAction->yp.push_back(posy);
    fRunAction->zp.push_back(posz);
    fRunAction->thet.push_back(th);
    fRunAction->phi.push_back(ph);
    fRunAction->xv.push_back(verx);
    fRunAction->yv.push_back(very);
    fRunAction->zv.push_back(verz);


  }

  

//  G4cout << "Events Processed = " << eid << G4endl;

//  ****CHECKING FOR NUMBER OF EVENTS TO ABORT*******//
  if(fRunAction->count==5000)
  {
    G4RunManager* run=G4RunManager::GetRunManager();
    run->AbortRun();
  }

  fRunAction->AddEdep(fEdep1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
