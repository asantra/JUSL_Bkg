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
#include "G4Event.hh"
#include "G4Threading.hh"
#include "Randomize.hh"

#include "globals.hh"
#include <fstream>
#include <cmath>
#include <ctime>

// TRandom3* r = new TRandom3(0);
// TRandom3* chance = new TRandom3(0);
// TRandom3* rpos =  new TRandom3(0);
double thetaPos;
double phiPos;
double zDir;
double slope;
double accept;

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
  G4int thid = G4Threading::G4GetThreadId();
  G4String fn = to_string(thid);
  if (thid <= 9)
    fn = "0" + fn;
  /// uranium and thorium components to USDC website, this gives you final neutron flux
  G4String path = "/home/sayang/JUSL_Simulation/Neutrons/Radiogen/MTInput/Hemisphere/2mThick/SiO2_norm/Comp2/FullSpec_";
  path = path + fn + ".dat";
  G4cout << path << G4endl;

  in.open(path);
  if (in.is_open())
    G4cout << "File Readable" << G4endl;
  else
    G4cout << "File Unreadable" << G4endl;

  //  f=new TFile("/home/sayan/JUSL_Simulation/Nuetron/Paper1/Neu_histspec.root");
  //  h1=new TH1F("h1","Energy",150,0,15);

  //  h1=(TH1F*)f->Get("En_spec_full");
  //  max=h1->GetMaximum();

  energySpectrum.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/NeuSpec_generated.dat",
                      std::ios::out | std::fstream::app);
  particlePosition.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/ParticlePostion_generated.dat",
                        std::ios::out | std::fstream::app);
  particleDirection.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/ParticleDirection_generated.dat",
                         std::ios::out | std::fstream::app);
  i = 0;
  pid = 2112;
}

DMXPrimaryGeneratorAction::~DMXPrimaryGeneratorAction()
{
  delete particleGun;
}

void DMXPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  //  energy = 5000.0*eV;

  loc = G4UniformRand(); // chance->Uniform(0,1);
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
  {

    rad = 220.0 + G4UniformRand() * (470.0 - 220.0); // change for outer radius
    //  z        = rpos->Rndm()*320.0 ;
    //  zmin     = rad*cos(thetaMax);
    z = G4UniformRand() * (rad); // zmin + G4UniformRand()*(rad-zmin) ;
    phiPos = G4UniformRand() * 2.0 * M_PI;

    thetaPos = acos(z / rad);

    x = rad * sin(thetaPos) * cos(phiPos);
    y = rad * sin(thetaPos) * sin(phiPos);
  }

  in >> en;

  energy = en * MeV;

  zDir = -1.0 + G4UniformRand() * 2.0; // rpos->Rndm()*2 ;
  phi = G4UniformRand() * 2.0 * M_PI;  // rpos->Rndm()*2.0*M_PI ;
  theta = acos(zDir / 1.0);

  energySpectrum << energy << G4endl;

  if (i < 100000)
  {
    particlePosition << x << '\t' << y << '\t' << z << G4endl;

    particleDirection << theta << '\t'
                      << phi << G4endl;
    i++;
  }
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *particle = particleTable->FindParticle(pid);
  // G4cout<<en/GeV<<"\t"<<particle->GetParticleName()<<endl;
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticlePosition(G4ThreeVector(x * cm, y * cm, z * cm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(sin(theta) * cos(phi),
                                                          sin(theta) * sin(phi),
                                                          cos(theta)));
  particleGun->SetParticleEnergy(energy);
  particleGun->GeneratePrimaryVertex(anEvent);
}
