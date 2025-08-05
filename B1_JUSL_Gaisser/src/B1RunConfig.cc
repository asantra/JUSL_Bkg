// B1RunConfig.cc
#include "B1RunConfig.hh"
#include "G4RunManager.hh"

B1RunConfig *B1RunConfig::Instance()
{
    static B1RunConfig instance;
    return &instance;
}

void B1RunConfig::SetRunID(int val){
  runID=val;
  //const G4Run* runptr1 = G4RunManager::GetRunManager()->GetCurrentRun();
  //G4cout << "### Current run ID : " << runptr1->GetRunID() << G4endl;
  //((G4Run*)(runptr1))->SetRunID(val);

}
