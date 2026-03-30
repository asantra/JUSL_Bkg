//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
// PrimaryGeneratorAction.CC
// --------------------------------------------------------------

// #include "TRandom3.h"
// #include "TFile.h"
// #include "TH1.h"

#include "DMXPrimaryGeneratorAction.hh"
// #include "DMXRunAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4Threading.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include <fstream>
#include <cmath>
#include <ctime>
#include "TFile.h"
#include "TH1D.h"

// TRandom3* r = new TRandom3(0);
// TRandom3* chance = new TRandom3(0);
// TRandom3* rpos =  new TRandom3(0);
double thetaPos;
double phiPos;
double zDir;
double slope;
double accept;
double enRnd;
double valRnd;

int i;
G4int pid;

using namespace std;

DMXPrimaryGeneratorAction::DMXPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{

  particleGun = new G4ParticleGun();
  //  G4cout  <<  "========INSIDE PARTICLE GUN=======" <<  G4endl;
  /*  neutronSpectrum.open("/home/shubhamdutta/G4WORK/NeutronSpectrum_short.dat",
             std::ios::in);

    for(i=0; i<80; i++)
      {
        neutron[i][0] = 0;
        neutron[i][1] = 0;
      }

    i=0;
    while( !neutronSpectrum.eof() )
      {
        neutronSpectrum >> neutron[i][0];
        neutronSpectrum >> neutron[i][1];
        i++;
      }
  */
  //  runAction=new DMXRunAction();
  // G4int thid = G4Threading::G4GetThreadId();
  // G4String fn = to_string(thid);
  // if (thid <= 9)
  //   fn = "0" + fn;
  // /// uranium and thorium components to USDC website, this gives you final neutron flux
  // G4String path = "/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Comp2/FullSpec_";
  // path = path + fn + ".dat";
  // G4cout << path << G4endl;

  // in.open(path);
  // if (in.is_open())
  //   G4cout << "File Readable" << G4endl;
  // else
  //   G4cout << "File Unreadable" << G4endl;

  //  f=new TFile("/home/sayan/JUSL_Simulation/Nuetron/Paper1/Neu_histspec.root");
  //  h1=new TH1F("h1","Energy",150,0,15);

  //  h1=(TH1F*)f->Get("En_spec_full");
  //  max=h1->GetMaximum();

  
  // energySpectrum.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/CosmNeuSpec_generated150CM.dat",
  //                     std::ios::out | std::fstream::app);
  // particlePosition.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/CosmParticlePosition_generated150CM.dat",
  //                       std::ios::out | std::fstream::app);
  // particleDirection.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/CosmParticleDirection_generated150CM.dat",
  //                        std::ios::out | std::fstream::app);

  energySpectrum.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/CosmNeuSpec_2Muon.dat",
                      std::ios::out | std::fstream::app);
  particlePosition.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/CosmParticlePosition_2Muon.dat",
                        std::ios::out | std::fstream::app);
  particleDirection.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/CosmNeut/Cosmogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/CosmParticleDirection_2Muon.dat",
                         std::ios::out | std::fstream::app);
  
  
  i = 0;
  //pid = 13;
}

DMXPrimaryGeneratorAction::~DMXPrimaryGeneratorAction()
{
  delete particleGun;
}

// Gaisser Function Body, try to use inverse cdf
G4double DMXPrimaryGeneratorAction::Gais(G4double en_val, G4double th_val)
{
  G4double par1 = 115.0;
  G4double par2 = 850.0;

  G4double d11 = 1 + (1.1 * en_val * cos(th_val) / par1);
  G4double d12 = 1 + (1.1 * en_val * cos(th_val) / par2);

  G4double t11 = (1.0 / d11) + (0.054 / d12);
  G4double val11 = 0.14 * pow(en_val, -2.7) * t11;

  return val11;
}
// G4int thid = G4Threading::G4GetThreadId();
// // G4cout<< "Thread Id = " << thid << G4endl;
// G4String fn = to_string(thid);
// if (thid <= 9)
//   fn = "0" + fn;

// G4String path = "/home/slab/Monalisa/JUSL_Bkg/MakingPlots/FluxHistogram/preliminaryHistograms_Muons_secondary_";
// path = path + fn + ".root";
// G4cout << "root file: " << path << G4endl;

// TFile *fIn = new TFile("/home/slab/Monalisa/JUSL_Bkg/MakingPlots/FluxHistogram/preliminaryHistograms_Muons_secondary_00.root", "READ");  //change the path for 555m and 815m depth accordingly. 
TFile *fIn = new TFile("/home/slab/Monalisa/JUSL_Bkg/MakingPlots/MuonFlux_815m/preliminaryHistograms_Muons2_secondary3.root", "READ");  //change the path for 555m and 815m depth accordingly. 
TH1D *hEnergy = (TH1D*)fIn->Get("histEnergy");

G4double DMXPrimaryGeneratorAction::Custom_flux(G4double en_val){
  // Custom flux obtained from a histogram
  
  G4int binNumber = hEnergy->FindBin(en_val);
  G4double val11 = hEnergy->GetBinContent(binNumber);
  //G4cout << "en_val: " << en_val << " bins :" << binNumber << " content: " << val11 << G4endl;
  return val11;
}

void DMXPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  // G4cout << "Generate primaries " << G4endl;
  /// ASK SAYAN
  G4double val_act = 0.0;
  do
  {
    enRnd = 0.0 + (G4UniformRand() * (10000 - 0));
    valRnd = 0.0 + (G4UniformRand() * (1 * pow(10, -9)));  // DO CHECK

    val_act = Custom_flux(enRnd);
  } while (valRnd >= val_act); 

  // mu+/mu- ratio implementation
  //i = 0; 
  //pid = 0;
  G4double chance = G4UniformRand();
  if (chance < 0.583)
    pid = -13;
  else
    pid = 13;

  //  energy = 5000.0*eV;
//////////NOTE //////
  /*loc = G4UniformRand(); // chance->Uniform(0,1);
  if (loc <= 0.40)
  {
    s = G4UniformRand();                     // rpos->Uniform(0,1);
    ra = sqrt(s) * 470.0;                    // change with outer radius
    thetaPos = G4UniformRand() * 2.0 * M_PI; // rpos->Uniform(0,2.0*M_PI);

    x = ra * cos(thetaPos);
    y = ra * sin(thetaPos);

    z = G4UniformRand() * (-250.0); // rpos->Uniform(0,1)*(-100.0)// Floor thickness;
  }
  else
  */
  
  // rad = 220.0 + G4UniformRand() * (420.0 - 220.0); // change for outer radius
  // rad = 420.0 + G4UniformRand() * (425.0 - 420.0); // change for outer radius
  rad = 420.0 ; // change for outer radius
  // --- Position on the sphere ---
  G4double phiPos   = G4UniformRand() * 2.0 * M_PI;
  G4double costhPos = G4UniformRand();  // uniform in [0,1]
  G4double thetaPos = std::acos(costhPos);

  x = rad * std::sin(thetaPos) * std::cos(phiPos);
  y = rad * std::sin(thetaPos) * std::sin(phiPos);
  z = rad * costhPos;

  //in >> enRnd;

  energy = enRnd;
  //energy = hEnergy->GetRandom() * GeV;
  
  phi = G4UniformRand() * 2.0 * M_PI;  // rpos->Rndm()*2.0*M_PI ;
  // G4double costheta = 2.0 * G4UniformRand() - 1.0; 
  theta = std::acos(costhPos);

  energySpectrum << energy << G4endl;

  // if (i < 100000)
  if (true)
  {
    particlePosition << pid << '\t' << x << '\t' << y << '\t' << z << G4endl;

    particleDirection << pid << '\t' << theta << '\t'
                      << phi << G4endl;
    i++;
  }
  // G4cout << "Event: " << anEvent->GetEventID()
  //      << " | pid = " << pid
  //      << " | enRnd = " << enRnd
  //      << G4endl;

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *particle = particleTable->FindParticle(pid);
  //G4cout<<energy/GeV<<"\t"<<particle->GetParticleName()<<endl;
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticlePosition(G4ThreeVector(x * cm, y * cm, z * cm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta) * cos(phi),
                                                          sin(theta) * sin(phi),
                                                          cos(theta)));
  particleGun->SetParticleEnergy(energy * GeV);
  particleGun->GeneratePrimaryVertex(anEvent);
}
