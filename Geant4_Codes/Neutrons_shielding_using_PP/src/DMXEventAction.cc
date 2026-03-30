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
  // ******** clearing air2 **********//
  air2_eID.clear();
  air2_pID.clear();
  air2_tID.clear();
  air2_KE.clear();
  air2_px.clear();
  air2_py.clear();
  air2_pz.clear();
  air2_mx.clear();
  air2_my.clear();
  air2_mz.clear();
  air2_vz.clear();

  // // ********* clearing pb *************//
  // pb_eID.clear();
  // pb_pID.clear();
  // pb_tID.clear();
  // pb_KE.clear();
  // pb_px.clear();
  // pb_py.clear();
  // pb_pz.clear();
  // pb_mx.clear();
  // pb_my.clear();
  // pb_mz.clear();
  // pb_vz.clear();

  // //****** clearing air3 *//
  // air3_eID.clear();
  // air3_pID.clear();
  // air3_tID.clear();
  // air3_KE.clear();
  // air3_px.clear();
  // air3_py.clear();
  // air3_pz.clear();
  // air3_mx.clear();
  // air3_my.clear();
  // air3_mz.clear();
  // air3_vz.clear();

  // // ********* clearing pp2 *************//
  // pp2_eID.clear();
  // pp2_pID.clear();
  // pp2_tID.clear();
  // pp2_KE.clear();
  // pp2_px.clear();
  // pp2_py.clear();
  // pp2_pz.clear();
  // pp2_mx.clear();
  // pp2_my.clear();
  // pp2_mz.clear();
  // pp2_vz.clear();

  // //****** clearing air4 *//
  // air4_eID.clear();
  // air4_pID.clear();
  // air4_tID.clear();
  // air4_KE.clear();
  // air4_px.clear();
  // air4_py.clear();
  // air4_pz.clear();
  // air4_mx.clear();
  // air4_my.clear();
  // air4_mz.clear();
  // air4_vz.clear();

  // elBeID.clear();
  // elBpID.clear();
  // elBtID.clear();
  // elBKE.clear();
  // elBpx.clear();
  // elBpy.clear();
  // elBpz.clear();
  // elBmx.clear();
  // elBmy.clear();
  // elBmz.clear();
  // elBvz.clear();

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

  // edBeID.clear();
  // edBpID.clear();
  // edBtID.clear();
  // edBKE.clear();
  // edBpx.clear();
  // edBpy.clear();
  // edBpz.clear();
  // edBmx.clear();
  // edBmy.clear();
  // edBmz.clear();
  // edBvz.clear();

  // PMThit = false;
  // flag_lab1 = false;
  // flag_rock = false;
  // //  flag_flo  = false;
  // flag_lab2 = false;

  // flag_det1 = false;
  // flag_drock = false;
  // flag_det2 = false;

  // firstStep = true;
  PMThit = false;
  flag_air2 = false;
  // flag_pb = false;
  // // flag_hdpe = false;
  // //  flag_flo  = false;
  // flag_air3 = false;
  // flag_pp2 = false;
  // flag_air4 = false;

  flag_det1 = false;
  // flag_dsteel = false;
  // flag_dhdpe = false;
  // flag_det2 = false;

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

  //****Storing air2 volume data*****//
  for (int i = 0; i < air2_eID.size(); i++)
  {
    runAct->rair2_eID.push_back(air2_eID[i]);
    runAct->rair2_pID.push_back(air2_pID[i]);
    runAct->rair2_tID.push_back(air2_tID[i]);

    runAct->rair2_KE.push_back(air2_KE[i]);

    runAct->rair2_px.push_back(air2_px[i]);
    runAct->rair2_py.push_back(air2_py[i]);
    runAct->rair2_pz.push_back(air2_pz[i]);

    runAct->rair2_mx.push_back(air2_mx[i]);
    runAct->rair2_my.push_back(air2_my[i]);
    runAct->rair2_mz.push_back(air2_mz[i]);

    runAct->rair2_vz.push_back(air2_vz[i]);
  }

  // //****Storing pb volume data*****//
  // for (int i = 0; i < pb_eID.size(); i++)
  // {
  //   runAct->rpb_eID.push_back(pb_eID[i]);
  //   runAct->rpb_pID.push_back(pb_pID[i]);
  //   runAct->rpb_tID.push_back(pb_tID[i]);

  //   runAct->rpb_KE.push_back(pb_KE[i]);

  //   runAct->rpb_px.push_back(pb_px[i]);
  //   runAct->rpb_py.push_back(pb_py[i]);
  //   runAct->rpb_pz.push_back(pb_pz[i]);

  //   runAct->rpb_mx.push_back(pb_mx[i]);
  //   runAct->rpb_my.push_back(pb_my[i]);
  //   runAct->rpb_mz.push_back(pb_mz[i]);

  //   runAct->rpb_vz.push_back(pb_vz[i]);
  // }

  // //****Storing air3 volume data*****//
  // for (int i = 0; i < air3_eID.size(); i++)
  // {
  //   runAct->rair3_eID.push_back(air3_eID[i]);
  //   runAct->rair3_pID.push_back(air3_pID[i]);
  //   runAct->rair3_tID.push_back(air3_tID[i]);

  //   runAct->rair3_KE.push_back(air3_KE[i]);

  //   runAct->rair3_px.push_back(air3_px[i]);
  //   runAct->rair3_py.push_back(air3_py[i]);
  //   runAct->rair3_pz.push_back(air3_pz[i]);

  //   runAct->rair3_mx.push_back(air3_mx[i]);
  //   runAct->rair3_my.push_back(air3_my[i]);
  //   runAct->rair3_mz.push_back(air3_mz[i]);

  //   runAct->rair3_vz.push_back(air3_vz[i]);
  // }

  // //****Storing pp2 volume data*****//
  // for (int i = 0; i < pp2_eID.size(); i++)
  // {
  //   runAct->rpp2_eID.push_back(pp2_eID[i]);
  //   runAct->rpp2_pID.push_back(pp2_pID[i]);
  //   runAct->rpp2_tID.push_back(pp2_tID[i]);

  //   runAct->rpp2_KE.push_back(pp2_KE[i]);

  //   runAct->rpp2_px.push_back(pp2_px[i]);
  //   runAct->rpp2_py.push_back(pp2_py[i]);
  //   runAct->rpp2_pz.push_back(pp2_pz[i]);

  //   runAct->rpp2_mx.push_back(pp2_mx[i]);
  //   runAct->rpp2_my.push_back(pp2_my[i]);
  //   runAct->rpp2_mz.push_back(pp2_mz[i]);

  //   runAct->rpp2_vz.push_back(pp2_vz[i]);
  // }

  // //****Storing air4 volume data*****//
  // for (int i = 0; i < air4_eID.size(); i++)
  // {
  //   runAct->rair4_eID.push_back(air4_eID[i]);
  //   runAct->rair4_pID.push_back(air4_pID[i]);
  //   runAct->rair4_tID.push_back(air4_tID[i]);

  //   runAct->rair4_KE.push_back(air4_KE[i]);

  //   runAct->rair4_px.push_back(air4_px[i]);
  //   runAct->rair4_py.push_back(air4_py[i]);
  //   runAct->rair4_pz.push_back(air4_pz[i]);

  //   runAct->rair4_mx.push_back(air4_mx[i]);
  //   runAct->rair4_my.push_back(air4_my[i]);
  //   runAct->rair4_mz.push_back(air4_mz[i]);

  //   runAct->rair4_vz.push_back(air4_vz[i]);
  // }

  // for (int i = 0; i < elBeID.size(); i++)
  // {
  //   runAct->rlBeID.push_back(elBeID[i]);
  //   runAct->rlBpID.push_back(elBpID[i]);
  //   runAct->rlBtID.push_back(elBtID[i]);

  //   runAct->rlBKE.push_back(elBKE[i]);

  //   runAct->rlBpx.push_back(elBpx[i]);
  //   runAct->rlBpy.push_back(elBpy[i]);
  //   runAct->rlBpz.push_back(elBpz[i]);

  //   runAct->rlBmx.push_back(elBmx[i]);
  //   runAct->rlBmy.push_back(elBmy[i]);
  //   runAct->rlBmz.push_back(elBmz[i]);

  //   runAct->rlBvz.push_back(elBvz[i]);
  // }

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

  // for (int i = 0; i < edBeID.size(); i++)
  // {
  //   runAct->rdBeID.push_back(edBeID[i]);
  //   runAct->rdBpID.push_back(edBpID[i]);
  //   runAct->rdBtID.push_back(edBtID[i]);

  //   runAct->rdBKE.push_back(edBKE[i]);

  //   runAct->rdBpx.push_back(edBpx[i]);
  //   runAct->rdBpy.push_back(edBpy[i]);
  //   runAct->rdBpz.push_back(edBpz[i]);

  //   runAct->rdBmx.push_back(edBmx[i]);
  //   runAct->rdBmy.push_back(edBmy[i]);
  //   runAct->rdBmz.push_back(edBmz[i]);

  //   runAct->rdBvz.push_back(edBvz[i]);
  // }

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
