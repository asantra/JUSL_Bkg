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
/// \file B1EventAction.hh
/// \brief Definition of the B1EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class B1RunAction;
class B1PrimaryGeneratorAction;

/// Event action class
///

class B1EventAction : public G4UserEventAction
{
  public:
    B1EventAction(B1RunAction* runAction);
    virtual ~B1EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep1(G4double edep) { fEdep1 += edep; }
    void AddEdep2(G4double edep) { fEdep2 += edep; }
    void AddEdep3(G4double edep) { fEdep3 += edep; }
    void AddEdep4(G4double edep) { fEdep4 += edep; }  
    G4int counter;
    G4double  Ken;
    G4double posx, posy, posz, verx, very, verz;
    G4double th;
    G4double ph;
    G4int eID, pID, tID;

  private:
    B1RunAction* fRunAction;
    const B1PrimaryGeneratorAction* fGenAction;
    G4double     fEdep1;
    G4double     fEdep2;
    G4double     fEdep3;
    G4double     fEdep4; 
    G4double     en_pri;
    G4int        PrPid;
    G4double     PriZen;
    G4double     priX;
    G4double     priY;
    G4int        eid;
    G4int        thid;
    
};

// Neutron tracking
public:
  std::vector<G4ThreeVector> neutronPos;
  std::vector<G4double> neutronEnergy;
  std::vector<G4String> neutronProcess;
  std::vector<G4int> neutronParentID;

  void AddNeutron(G4ThreeVector pos, G4double energy, G4String proc, G4int pid) {
    neutronPos.push_back(pos);
    neutronEnergy.push_back(energy);
    neutronProcess.push_back(proc);
    neutronParentID.push_back(pid);
  }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
