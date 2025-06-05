//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
// RunAction.cc
// --------------------------------------------------------------

#include "DMXRunAction.hh"
#include "DMXPrimaryGeneratorAction.hh"
#include "DMXDetectorConstruction.hh"

#include "G4Run.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "Randomize.hh"

#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

int l = 0;
std::stringstream filename;

DMXRunAction::DMXRunAction():G4UserRunAction()			    
{}

DMXRunAction::~DMXRunAction()
{}

void DMXRunAction::BeginOfRunAction(const G4Run* aRun)
{

//  G4cout  <<  "========INSIDE RUN ACTION=======" <<  G4endl;
    //For Random number setting the seeds
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);

    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

//  pmtHit.open("/home/sayan/JUSL_Simulation/Neutron/Shielding/NeutronHit.dat",
//	      std::ios::out | std::ios::trunc);

//  pmtHitSet.open("/home/sayan/JUSL_Simulation/Neutron/Shielding/NeutronHitSet.dat",
//		 std::ios::out | std::ios::trunc);

  NeutronEnTr.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/NeutronTrSpec.dat",
                   std::ios::out | std::fstream::app);
  NeutronEnDet.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/NeutronDetSpec.dat",
                    std::ios::out | std::fstream::app);
  NeutronBack.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/NeutronBackScatter.dat",
                   std::ios::out | std::fstream::app);
  NeutronDetBack.open("/home/sayang/JUSL_Simulation/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/DetBackScatter.dat",
                   std::ios::out | std::fstream::app);


  G4cout << "### Run " << aRun->GetRunID() << " start" << G4endl;
  eventCounts = 0;
}

void DMXRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvents = aRun->GetNumberOfEvent();
  if (nofEvents == 0) return;

  G4cout << "******TOTAL EVENTS******\n"
	 << eventCounts << G4endl ;

  //*****Lab data to file****//
  //****All hits****//
  for(int i=0;i<rleID.size();i++)
  {
      NeutronEnTr << rleID[i] << "\t" << rlpID[i] << "\t" << rltID[i] << "\t"
                  << rlKE[i] << "\t"
                  << rlpx[i] << "\t" << rlpy[i] << "\t" << rlpz[i] << "\t"
                  << rlmx[i] << "\t" << rlmy[i] << "\t" << rlmz[i] << "\t"
                  << rlvz[i] << G4endl;
  }
  //****BackScatter****//
  for(int i=0;i<rlBeID.size();i++)
  {
      NeutronBack << rlBeID[i] << "\t" << rlBpID[i] << "\t" << rlBtID[i] << "\t"
                  << rlBKE[i] << "\t"
                  << rlBpx[i] << "\t" << rlBpy[i] << "\t" << rlBpz[i] << "\t"
                  << rlBmx[i] << "\t" << rlBmy[i] << "\t" << rlBmz[i] << "\t"
                  << rlBvz[i] << G4endl;
  }

  //*****Detector data to file****//
  //****1st hits****//
  for(int i=0;i<rdeID.size();i++)
  {
      NeutronEnDet << rdeID[i] << "\t" << rdpID[i] << "\t" << rdtID[i] << "\t"
                   << rdKE[i] << "\t"
                   << rdpx[i] << "\t" << rdpy[i] << "\t" << rdpz[i] << "\t"
                   << rdmx[i] << "\t" << rdmy[i] << "\t" << rdmz[i] << "\t"
                   << rdvz[i] << G4endl;
  }
  //****BackScatter****//
  for(int i=0;i<rdBeID.size();i++)
  {
      NeutronDetBack << rdBeID[i] << "\t" << rdBpID[i] << "\t" << rdBtID[i] << "\t"
                     << rdBKE[i] << "\t"
                     << rdBpx[i] << "\t" << rdBpy[i] << "\t" << rdBpz[i] << "\t"
                     << rdBmx[i] << "\t" << rdBmy[i] << "\t" << rdBmz[i] << "\t"
                     << rdBvz[i] << G4endl;
  }



  pmtHit.close();
  pmtHitSet.close();
  
}
