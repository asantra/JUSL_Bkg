//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
//   SteppingAction.CC
// --------------------------------------------------------------

#include "DMXSteppingAction.hh"
#include "DMXEventAction.hh"
#include "DMXDetectorConstruction.hh"
#include "DMXRunAction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4Polyline.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

DMXSteppingAction::DMXSteppingAction(DMXRunAction *DMXRun, DMXEventAction *eventAction)
    : runAction(DMXRun), evtAction(eventAction), fScoringVolumeLab(0),
      fScoringVolumePMT(0), fScoringVolumeCav(0), fScoringVolumeFlo(0)
{
  pTrackID = 0;
  pTrackID_pmt = 0;

  //  G4cout << "====Stepping constructor called====" << G4endl;
}

DMXSteppingAction::~DMXSteppingAction()
{
}

void DMXSteppingAction::UserSteppingAction(const G4Step *fStep)
{

  if (evtAction->firstStep)
  {
    pTrackID = 0;
    evtAction->firstStep = false;
  }

  if (!fScoringVolumeCav)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumeCav = detectorConstruction->GetScoringVolumeCav();
  }

  if (!fScoringVolumeFlo)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumeFlo = detectorConstruction->GetScoringVolumeFlo();
  }

  if (!fScoringVolumeLab)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumeLab = detectorConstruction->GetScoringVolumeLab();
  }

  if (!fScoringVolumePMT)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumePMT = detectorConstruction->GetScoringVolumePMT();
    //      G4cout << "=====FOR CHECKING PMT VOLUME======" << G4endl;
    //      G4cout << "Volume name is " << fScoringVolumePMT->GetName() << G4endl;
    //      G4cout << "Volume material is " << fScoringVolumePMT->GetMaterial() << G4endl;
  }

  // get volume of the current step
  G4LogicalVolume *volume = fStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  if (volume != fScoringVolumeLab &&
      volume != fScoringVolumeCav &&
      volume != fScoringVolumeFlo &&
      volume != fScoringVolumePMT)
  {
    return;
  }

  G4String volname = volume->GetName();

  //  if( evtAction->PMThit )
  //      { return; }

  atrack = fStep->GetTrack();

  if (atrack->GetDefinition() == G4Neutron::NeutronDefinition())
  {

    eID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    pID = atrack->GetDefinition()->GetPDGEncoding();

    pos = atrack->GetPosition();
    posx = pos.x();
    posy = pos.y();
    posz = pos.z();

    mom = atrack->GetMomentum();
    momx = mom.x();
    momy = mom.y();
    momz = mom.z();

    dir = atrack->GetMomentumDirection();
    dx = dir.x();
    dy = dir.y();
    dz = dir.z();

    ver = atrack->GetVertexPosition();
    vz = ver.getZ();

    tID = atrack->GetTrackID();
    KE = atrack->GetKineticEnergy();

    //      runAction->stepOut << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << "\t"
    //                         << atrack->GetDefinition()->GetParticleName() << "\t"
    //                         << volname << G4endl;
    //                         << "\t" << atrack->GetPosition().x()/cm << "\t"
    //                         << atrack->GetPosition().y()/cm << "\t"
    //                         << atrack->GetPosition().z()/cm << G4endl;

    if (pTrackID != tID)
    {
      //***Lab***//
      evtAction->flag_lab1 = false;
      evtAction->flag_rock = false;
      //  //        evtAction->flag_flo  = false;
      evtAction->flag_lab2 = false;

      //***Detector***//
      evtAction->flag_det1 = false;
      evtAction->flag_drock = false;
      evtAction->flag_det2 = false;
    }

    pTrackID = tID;

    if (evtAction->flag_lab1 && evtAction->flag_rock && evtAction->flag_lab2)
    {
      //       //   evtAction->flag_lab1 = false;
      evtAction->flag_rock = false;
      evtAction->flag_lab2 = false;
    }

    if (evtAction->flag_det1 && evtAction->flag_drock && evtAction->flag_det2)
    {
      evtAction->flag_drock = false;
      evtAction->flag_det2 = false;
    }

    if (volume == fScoringVolumeLab)
    {
      evtAction->flag_lab1 = true;

      evtAction->eleID.push_back(eID);
      evtAction->elpID.push_back(pID);

      evtAction->elpx.push_back(pos.getX());
      evtAction->elpy.push_back(pos.getY());
      evtAction->elpz.push_back(pos.getZ());

      evtAction->elmx.push_back(mom.getX());
      evtAction->elmy.push_back(mom.getY());
      evtAction->elmz.push_back(mom.getZ());

      evtAction->eltID.push_back(tID);
      evtAction->elKE.push_back(KE / MeV);

      evtAction->elvz.push_back(vz);

      //  //        runAction->NeutronEnTr << eID << "\t" << pID << "\t" << tID << "\t"
      //  //                               << KE/MeV
      //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
      //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
      //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
      //  //                               << G4endl;
      evtAction->labcount++;
    }

    if (volume == fScoringVolumePMT)
    {
      evtAction->PMThit = true;
      evtAction->flag_det1 = true;

      runAction->eventCounts++;
      runAction->pmtHitWorker << runAction->eventCounts << "\t"
                              << evtAction->event_id << G4endl;
      if (evtAction->detcount == 0)
      {
        //              runAction->NeutronEnDet << (atrack->GetKineticEnergy())/MeV
        //                                      << G4endl;
        evtAction->edeID.push_back(eID);
        evtAction->edpID.push_back(pID);

        evtAction->edpx.push_back(pos.getX());
        evtAction->edpy.push_back(pos.getY());
        evtAction->edpz.push_back(pos.getZ());

        evtAction->edmx.push_back(mom.getX());
        evtAction->edmy.push_back(mom.getY());
        evtAction->edmz.push_back(mom.getZ());

        evtAction->edtID.push_back(tID);
        evtAction->edKE.push_back(KE / MeV);

        evtAction->edvz.push_back(vz);
      }
      evtAction->detcount++;
      evtAction->Endet = atrack->GetKineticEnergy() / MeV;
    }

    if (volume == fScoringVolumeFlo || volume == fScoringVolumeCav)
    {
      if (evtAction->flag_lab1)
        evtAction->flag_rock = true;

      if (evtAction->flag_det1)
        evtAction->flag_drock = true;
    }

    if (evtAction->flag_rock && volume == fScoringVolumeLab)
    {
      evtAction->flag_lab2 = true;

      //  //        G4cout << "====Backscattering found====" << G4endl;
      //  //        G4cout << eID << "\t" << tID << "\t" << KE/MeV << G4endl;

      evtAction->elBeID.push_back(eID);
      evtAction->elBpID.push_back(pID);

      evtAction->elBpx.push_back(pos.getX());
      evtAction->elBpy.push_back(pos.getY());
      evtAction->elBpz.push_back(pos.getZ());

      evtAction->elBmx.push_back(mom.getX());
      evtAction->elBmy.push_back(mom.getY());
      evtAction->elBmz.push_back(mom.getZ());

      evtAction->elBtID.push_back(tID);
      evtAction->elBKE.push_back(KE / MeV);

      evtAction->elBvz.push_back(vz);

      //  //        runAction->NeutronBack << eID << "\t" << pID << "\t" << tID << "\t"
      //  //                               << KE/MeV
      //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
      //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
      //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
      //  //                               << G4endl;
    }

    if (evtAction->flag_drock && volume == fScoringVolumePMT)
    {
      evtAction->flag_det2 = true;

      //          G4cout << "====Backscattering found====" << G4endl;
      //          G4cout << eID << "\t" << tID << "\t" << KE/MeV << G4endl;

      evtAction->edBeID.push_back(eID);
      evtAction->edBpID.push_back(pID);

      evtAction->edBpx.push_back(pos.getX());
      evtAction->edBpy.push_back(pos.getY());
      evtAction->edBpz.push_back(pos.getZ());

      evtAction->edBmx.push_back(mom.getX());
      evtAction->edBmy.push_back(mom.getY());
      evtAction->edBmz.push_back(mom.getZ());

      evtAction->edBtID.push_back(tID);
      evtAction->edBKE.push_back(KE / MeV);

      evtAction->edBvz.push_back(vz);
    }

    //      G4cout << eID << "\t" << tID << "\t" << evtAction->flag_det1
    //             << "\t" << evtAction->flag_drock
    //             << "\t" << evtAction->flag_det2 << "\t" << volname << G4endl;
  }
}
