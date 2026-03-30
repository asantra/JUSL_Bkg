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

  NeutronEnTrAir2.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/NeutronTrSpecAir2.dat",
                   std::ios::out | std::fstream::app);
  // NeutronEnTrPb.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/NeutronTrSpecPb.dat",
  //                  std::ios::out | std::fstream::app);
  // NeutronEnTrAir3.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/NeutronTrSpecAir3.dat",
  //                  std::ios::out | std::fstream::app);
  // NeutronEnTrPP2.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/NeutronTrSpecPP2.dat",
  //                  std::ios::out | std::fstream::app);
  // NeutronEnTrAir4.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/NeutronTrSpecAir4.dat",
  //                  std::ios::out | std::fstream::app);
  NeutronEnDet.open("/home/slab/Monalisa/JUSL_Bkg/JUSLFiles/Neutrons/Radiogen/DataFiles/MTFiles/Hemisphere/2.5mThick/SiO2_norm/LowDen/Comp1/hdpe_shield/NeutronDetSpec.dat",
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
  for(int i=0;i<rair2_eID.size();i++)
  {
      NeutronEnTrAir2 << rair2_eID[i] << "\t" << rair2_pID[i] << "\t" << rair2_tID[i] << "\t"
                      << rair2_KE[i] << "\t"
                      << rair2_px[i] << "\t" << rair2_py[i] << "\t" << rair2_pz[i] << "\t"
                      << rair2_mx[i] << "\t" << rair2_my[i] << "\t" << rair2_mz[i] << "\t"
                      << rair2_vz[i] << G4endl;
  }

  // for(int i=0;i<rpb_eID.size();i++)
  // {
  //     NeutronEnTrPb << rpb_eID[i] << "\t" << rpb_pID[i] << "\t" << rpb_tID[i] << "\t"
  //                     << rpb_KE[i] << "\t"
  //                     << rpb_px[i] << "\t" << rpb_py[i] << "\t" << rpb_pz[i] << "\t"
  //                     << rpb_mx[i] << "\t" << rpb_my[i] << "\t" << rpb_mz[i] << "\t"
  //                     << rpb_vz[i] << G4endl;
  // }

  // for(int i=0;i<rair3_eID.size();i++)
  // {
  //     NeutronEnTrAir3 << rair3_eID[i] << "\t" << rair3_pID[i] << "\t" << rair3_tID[i] << "\t"
  //                     << rair3_KE[i] << "\t"
  //                     << rair3_px[i] << "\t" << rair3_py[i] << "\t" << rair3_pz[i] << "\t"
  //                     << rair3_mx[i] << "\t" << rair3_my[i] << "\t" << rair3_mz[i] << "\t"
  //                     << rair3_vz[i] << G4endl;
  // }

  // for(int i=0;i<rpp2_eID.size();i++)
  // {
  //      NeutronEnTrPP2 << rpp2_eID[i] << "\t" << rpp2_pID[i] << "\t" << rpp2_tID[i] << "\t"
  //                     << rpp2_KE[i] << "\t"
  //                     << rpp2_px[i] << "\t" << rpp2_py[i] << "\t" << rpp2_pz[i] << "\t"
  //                     << rpp2_mx[i] << "\t" << rpp2_my[i] << "\t" << rpp2_mz[i] << "\t"
  //                     << rpp2_vz[i] << G4endl;
  // }

  // for(int i=0;i<rair4_eID.size();i++)
  // {
  //     NeutronEnTrAir4 << rair4_eID[i] << "\t" << rair4_pID[i] << "\t" << rair4_tID[i] << "\t"
  //                     << rair4_KE[i] << "\t"
  //                     << rair4_px[i] << "\t" << rair4_py[i] << "\t" << rair4_pz[i] << "\t"
  //                     << rair4_mx[i] << "\t" << rair4_my[i] << "\t" << rair4_mz[i] << "\t"
  //                     << rair4_vz[i] << G4endl;
  // }

  //****BackScatter****//
  // for(int i=0;i<rlBeID.size();i++)
  // {
  //     NeutronBack << rlBeID[i] << "\t" << rlBpID[i] << "\t" << rlBtID[i] << "\t"
  //                 << rlBKE[i] << "\t"
  //                 << rlBpx[i] << "\t" << rlBpy[i] << "\t" << rlBpz[i] << "\t"
  //                 << rlBmx[i] << "\t" << rlBmy[i] << "\t" << rlBmz[i] << "\t"
  //                 << rlBvz[i] << G4endl;
 // }

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
  // for(int i=0;i<rdBeID.size();i++)
  // {
  //     NeutronDetBack << rdBeID[i] << "\t" << rdBpID[i] << "\t" << rdBtID[i] << "\t"
  //                    << rdBKE[i] << "\t"
  //                    << rdBpx[i] << "\t" << rdBpy[i] << "\t" << rdBpz[i] << "\t"
  //                    << rdBmx[i] << "\t" << rdBmy[i] << "\t" << rdBmz[i] << "\t"
  //                    << rdBvz[i] << G4endl;
  // }



  pmtHit.close();
  pmtHitSet.close();
  
}
