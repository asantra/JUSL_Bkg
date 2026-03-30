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
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <fstream>
#include "G4PhysicalConstants.hh"
#include "G4Threading.hh"
#include "TFile.h"
#include "TH1D.h"
// #include "TMath.hh"
#include <cmath>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(0),
      fEnvelopeBox(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  // z=556.0*m;
  // depth = 554.0 * m;

  fIn = new TFile("/home/slab/Monalisa/JUSL_Bkg/MakingPlots/MuonFlux_815m/preliminaryHistograms_Muons2_secondary3.root", "READ");

  TH1D *hTemp = (TH1D*)fIn->Get("histEnergy");
  // TH1D *hTemp = (TH1D*)fIn.Get("histEnergy");

  hEnergy = (TH1D*)hTemp->Clone();
  hEnergy->SetDirectory(0);  // detach from file

  fIn->Close();
  delete fIn;

  G4int thid = G4Threading::G4GetThreadId();
  // G4cout<< "Thread Id = " << thid << G4endl;
  G4String fn = to_string(thid);
  if (thid <= 9)
    fn = "0" + fn;
  G4String path = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/compute5/terrain_Gen_21_";
  path = path + fn + ".dat";
  G4cout << path << G4endl;

  in.open(path);
  if (in.is_open())
    G4cout << "File readable" << G4endl;
  else
    G4cout << "File unreadable" << G4endl;

  // default particle kinematic
  // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4String particleName;
  // G4ParticleDefinition* particle
  //  = particleTable->FindParticle(particleName="mu-");
  // fParticleGun->SetParticleDefinition(particle);
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  // fParticleGun->SetParticleEnergy(500.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete hEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Gaisser Function Body, try to use inverse cdf
G4double B1PrimaryGeneratorAction::Gais(G4double en_val, G4double th_val)
{
  G4double par1 = 115.0;
  G4double par2 = 850.0;

  G4double d11 = 1 + (1.1 * en_val * cos(th_val) / par1);
  G4double d12 = 1 + (1.1 * en_val * cos(th_val) / par2);

  G4double t11 = (1.0 / d11) + (0.054 / d12);
  G4double val11 = 0.14 * pow(en_val, -2.7) * t11;

  return val11;
}

// fIn = new TFile("/home/slab/Monalisa/JUSL_Bkg/MakingPlots/MuonFlux_815m/preliminaryHistograms_Muons2_secondary3.root", "READ");

// TH1D *hTemp = (TH1D*)fIn->Get("histEnergy");

// hEnergy = (TH1D*)hTemp->Clone();
// hEnergy->SetDirectory(0);  // detach from file

// fIn->Close();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  // this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  //******FOR TAGGING GAISSER FORMULAR*****//
  //*****COMMET WHEN TESTING FOR VERTICAL PROPAGATION*****//
  // G4double val_act = 0.0;
  // do
  // {
  //   enRnd = 0 + (G4UniformRand() * 15000);
  //   thRnd = 0.0 + (G4UniformRand() * (70 * M_PI / 180.0)); // maximum for the muon telescope is 70 degrees, maximum for Gaisser parametrization
  //   valRnd = 0.0 + (G4UniformRand() * (3.0 * pow(10, -9)));  // most likely vertical intensity, DO CHECK

  //   val_act = Gais(enRnd, thRnd);
  // } while (valRnd >= val_act);


  //*********************************************//

  // mu+/mu- ratio implementation
  G4int pid = 0;
  G4double chance = G4UniformRand();
  if (chance < 0.583)
    pid = -13;
  else
    pid = 13;

  G4double side = 1500.0; // HalfXlength of Rock

  // G4double x = -side + (G4UniformRand()*(2.0*side));
  // G4double y = -side + (G4UniformRand()*(2.0*side));
  // G4double length = 50.0 * cm;
  x = (G4UniformRand() - 0.5) * 0.335;
  y = (G4UniformRand() - 0.5) * 0.335;
  z = 10 * m;

  enRnd = hEnergy->GetRandom();

  //  out << pid << "\t"<< enRnd << "\t" << thRnd << "\t"
  //      << x << "\t" << "\t" << y << G4endl;//Generated values to file

  PrPid = pid;
  // PriZen = thRnd;
  priX = x;
  priY = y;
  //  G4double en=0.0, thet=0.0, x1=0.0, y1=0.0;// z=556.0*m;
  //  G4int ppid=0;
  //  string line;

  // Just to test
  //  x = 0.0;
  //  y = 0.0;

  // thRnd = M_PI + thRnd; //-1.0*thRnd;

  // For dummy runs so that muons are stopped...Done when n exceeds
  // the number of lines in the data file. Not needed now since in
  // EventAction AbortRun() is being executed. Uncomment if AbortRun()
  // is not executed.
  //  if(n>27101)
  //  {
  //    pid=13;
  //    en=1.0*MeV;
  //    thet=1;
  //    x=0;
  //    y=0;
  //  }
  // in>>pid>>en>>thet>>x>>y;

  // G4cout<<"Read Values are:"<<G4endl;
  // G4cout<<pid<<"\t"<<en<<"\t"<<thet<<"\t"<<x<<"\t"<<y<<G4endl;
  // G4cout<<"====I am here=========="<<G4endl;
  // G4cout<<G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()<<G4endl;

  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

  if (!fEnvelopeBox)
  {
    G4LogicalVolume *envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if (envLV)
      fEnvelopeBox = dynamic_cast<G4Box *>(envLV->GetSolid());
  }

  if (fEnvelopeBox)
  {
    envSizeXY = fEnvelopeBox->GetXHalfLength() * 2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength() * 2.;
  }
  else
  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
                "MyCode0002", JustWarning, msg);
  }

  G4double a = 120.0 * m;
  G4double size = 0.8;
  // G4double x0 = x*m;//-a+(G4UniformRand()*(2*a));//size * envSizeXY * (G4UniformRand()-0.5);
  // G4double y0 = y*m;//-a+(G4UniformRand()*(2*a));//size * envSizeXY * (G4UniformRand()-0.5);
  G4double phi = 2.0 * pi * (G4UniformRand()); // phi is isotropic
  // G4double z0 = 0.5*z;
  // G4double phi = ((G4UniformRand() - 0.5) * (2*0.93 * M_PI / 180.0)); // phi is isotropic
  // G4double phi = 0.0;

  // setting particle directions
  // G4double thRnd_mom = (26.38* M_PI / 180.0) + (G4UniformRand() * (0.25 * M_PI / 180.0)); 
  // G4double thRnd_mom = (26.505* M_PI / 180.0); 
  // G4double thRnd_mom = ((G4UniformRand() - 0.5) * (0.90 * M_PI / 180.0)); 
  G4double thRnd_mom = ((G4UniformRand() - 0.5) * (2.8* M_PI / 180.0)); 
  PriZen = thRnd_mom;
  thRnd_mom = M_PI + thRnd_mom;
  G4double px = sin(thRnd_mom)* cos(phi);
  G4double py = sin(thRnd_mom)* sin(phi);
  G4double pz = cos(thRnd_mom);
  // setting particle directions
  // G4double px = sin(thRnd) * cos(phi);
  // G4double py = sin(thRnd) * sin(phi);
  // G4double pz = cos(thRnd);

  if (x < -1500.0 || x > 1500.0 || y < -1500.0 || y > 1500.0)
    G4cout << "bad shooting points are :" << x << "\t" << y << G4endl;
  // G4cout<<(px*px+py*py+pz*pz)<<G4endl;

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(pid);
  // G4cout<<en/GeV<<"\t"<<particle->GetParticleName()<<endl;

  // x = 0; y = 0; px = 0; py = 0; pz = -1; 
  // x = 0; y = 0; 
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticlePosition(G4ThreeVector(x * m, y * m, z));

  //***FOR VERTICAL TEST******//
  // fParticleGun->SetParticleEnergy(408*GeV);
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));

  //************FOR GAISSER FORMULA PROPAGATION****//
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
  fParticleGun->SetParticleEnergy(enRnd * GeV);
  // fParticleGun->SetParticleEnergy(4 * GeV);

  fParticleGun->GeneratePrimaryVertex(anEvent);

  energy_pri = fParticleGun->GetParticleEnergy();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
