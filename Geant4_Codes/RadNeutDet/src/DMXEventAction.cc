//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
//
// EventAction.cc
// --------------------------------------------------------------

#include "DMXEventAction.hh"

#include "DMXRunAction.hh"
#include "DMXPrimaryGeneratorAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include <fstream>
#include <sstream>
#include <iomanip>

DMXEventAction::DMXEventAction(DMXRunAction *DMXRun,
                               DMXPrimaryGeneratorAction *DMXGenerator)
    : runAct(DMXRun), genAction(DMXGenerator)
{
  printModulo = 1;
  energy_pri = 0;
  seeds = NULL;
}

DMXEventAction::~DMXEventAction()
{
}

void DMXEventAction::BeginOfEventAction(const G4Event *evt)
{

  // grab seeds
  seeds = genAction->GetEventSeeds();

  // grab energy of primary
  energy_pri = genAction->GetEnergyPrimary();
  //  //grab positions of primary particle generation
  //  genx=genAction->GetPosX();
  //  geny=genAction->GetPosY();
  //  genz=genAction->GetPosZ();

  event_id = evt->GetEventID();
  ptID = 0;

  runAct->evtCts++;

  // print this information event by event (modulo n)
  if (event_id % printModulo == 0)
  {
    G4cout << "\n---> Begin of event: " << event_id << G4endl;
  }

  labcount = 0;
  detcount = 0;

  //****Clearing Lab****//
  eleID.clear();
  elpID.clear();
  eltID.clear();
  elKE.clear();
  elpx.clear();
  elpy.clear();
  elpz.clear();
  elmx.clear();
  elmy.clear();
  elmz.clear();
  elvz.clear();

  elBeID.clear();
  elBpID.clear();
  elBtID.clear();
  elBKE.clear();
  elBpx.clear();
  elBpy.clear();
  elBpz.clear();
  elBmx.clear();
  elBmy.clear();
  elBmz.clear();
  elBvz.clear();

  //****Clearing Detector****//
  edeID.clear();
  edpID.clear();
  edtID.clear();
  edKE.clear();
  edpx.clear();
  edpy.clear();
  edpz.clear();
  edmx.clear();
  edmy.clear();
  edmz.clear();
  edvz.clear();

  edBeID.clear();
  edBpID.clear();
  edBtID.clear();
  edBKE.clear();
  edBpx.clear();
  edBpy.clear();
  edBpz.clear();
  edBmx.clear();
  edBmy.clear();
  edBmz.clear();
  edBvz.clear();

  PMThit = false;
  flag_lab1 = false;
  flag_rock = false;
  //  flag_flo  = false;
  flag_lab2 = false;

  flag_det1 = false;
  flag_drock = false;
  flag_det2 = false;

  firstStep = true;
}

void DMXEventAction::EndOfEventAction(const G4Event *evt)
{
  //  G4cout << runAct->eventCounts << G4endl;
  //  if (event_id%printModulo == 0)
  //    {
  G4cout << "Event Count = " << runAct->eventCounts << G4endl;
  //  if(event_id<=333333)
  //  {
  //      runAct->posx.push_back(genx);
  //      runAct->posy.push_back(geny);
  //      runAct->posz.push_back(genz);
  //  }
  //  runAct->genEn.push_back(energy_pri);
  //  runAct->labEn.push_back(Enlab);
  //  runAct->detEn.push_back(Endet);

  //    }

  //****Storing lab volume data*****//
  for (int i = 0; i < eleID.size(); i++)
  {
    runAct->rleID.push_back(eleID[i]);
    runAct->rlpID.push_back(elpID[i]);
    runAct->rltID.push_back(eltID[i]);

    runAct->rlKE.push_back(elKE[i]);

    runAct->rlpx.push_back(elpx[i]);
    runAct->rlpy.push_back(elpy[i]);
    runAct->rlpz.push_back(elpz[i]);

    runAct->rlmx.push_back(elmx[i]);
    runAct->rlmy.push_back(elmy[i]);
    runAct->rlmz.push_back(elmz[i]);

    runAct->rlvz.push_back(elvz[i]);
  }

  for (int i = 0; i < elBeID.size(); i++)
  {
    runAct->rlBeID.push_back(elBeID[i]);
    runAct->rlBpID.push_back(elBpID[i]);
    runAct->rlBtID.push_back(elBtID[i]);

    runAct->rlBKE.push_back(elBKE[i]);

    runAct->rlBpx.push_back(elBpx[i]);
    runAct->rlBpy.push_back(elBpy[i]);
    runAct->rlBpz.push_back(elBpz[i]);

    runAct->rlBmx.push_back(elBmx[i]);
    runAct->rlBmy.push_back(elBmy[i]);
    runAct->rlBmz.push_back(elBmz[i]);

    runAct->rlBvz.push_back(elBvz[i]);
  }

  //****Storing detector volume data*****//
  for (int i = 0; i < edeID.size(); i++)
  {
    runAct->rdeID.push_back(edeID[i]);
    runAct->rdpID.push_back(edpID[i]);
    runAct->rdtID.push_back(edtID[i]);

    runAct->rdKE.push_back(edKE[i]);

    runAct->rdpx.push_back(edpx[i]);
    runAct->rdpy.push_back(edpy[i]);
    runAct->rdpz.push_back(edpz[i]);

    runAct->rdmx.push_back(edmx[i]);
    runAct->rdmy.push_back(edmy[i]);
    runAct->rdmz.push_back(edmz[i]);

    runAct->rdvz.push_back(edvz[i]);
  }

  for (int i = 0; i < edBeID.size(); i++)
  {
    runAct->rdBeID.push_back(edBeID[i]);
    runAct->rdBpID.push_back(edBpID[i]);
    runAct->rdBtID.push_back(edBtID[i]);

    runAct->rdBKE.push_back(edBKE[i]);

    runAct->rdBpx.push_back(edBpx[i]);
    runAct->rdBpy.push_back(edBpy[i]);
    runAct->rdBpz.push_back(edBpz[i]);

    runAct->rdBmx.push_back(edBmx[i]);
    runAct->rdBmy.push_back(edBmy[i]);
    runAct->rdBmz.push_back(edBmz[i]);

    runAct->rdBvz.push_back(edBvz[i]);
  }

  /*if ( (event_id+1) % 100000000==0 )
    {
      runAct->pmtHitSet << runAct->eventCounts << "\t"
      << event_id << G4endl;
    }*/
  // may be needed to restrict computational time
  if (runAct->evtCts == 1460845)
  {
    G4RunManager *run = G4RunManager::GetRunManager();
    run->AbortRun();
  }
}
