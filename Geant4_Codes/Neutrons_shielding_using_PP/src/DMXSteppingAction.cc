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
      fScoringVolumePMT(0), fScoringVolumeCav(0), fScoringVolumeFlo(0), 
      fScoringVolumeGlass(0), fScoringVolumeAir1(0), fScoringVolumeAir2(0), fScoringVolumePP1(0)
      // fScoringVolumeAir3(0), fScoringVolumeAir4(0),
      // fScoringVolumePP2(0), fScoringVolumePb(0)
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
  }

  if (!fScoringVolumeAir1)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumeAir1 = detectorConstruction->GetScoringVolumeAir1();
  }

  if (!fScoringVolumeAir2)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumeAir2 = detectorConstruction->GetScoringVolumeAir2();
  }

  // if (!fScoringVolumeAir3)
  // {
  //   const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  //   fScoringVolumeAir3 = detectorConstruction->GetScoringVolumeAir3();
  // }

  // if (!fScoringVolumeAir4)
  // {
  //   const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  //   fScoringVolumeAir4 = detectorConstruction->GetScoringVolumeAir4();
  // }

  if (!fScoringVolumePP1)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumePP1 = detectorConstruction->GetScoringVolumePP1();
  }

  // if (!fScoringVolumePP2)
  // {
  //   const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  //   fScoringVolumePP2 = detectorConstruction->GetScoringVolumePP2();
  // }

  // if (!fScoringVolumePb)
  // {
  //   const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  //   fScoringVolumePb = detectorConstruction->GetScoringVolumePb();
  // }

  if (!fScoringVolumeGlass)
  {
    const DMXDetectorConstruction *detectorConstruction = static_cast<const DMXDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fScoringVolumeGlass = detectorConstruction->GetScoringVolumeGlass();
    //      G4cout << "=====FOR CHECKING PMT VOLUME======" << G4endl;
    //      G4cout << "Volume name is " << fScoringVolumePMT->GetName() << G4endl;
    //      G4cout << "Volume material is " << fScoringVolumePMT->GetMaterial() << G4endl;
  }

  // get volume of the current step
  G4LogicalVolume *volume = fStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  if (//volume != fScoringVolumeLab &&
      volume != fScoringVolumeCav &&
      volume != fScoringVolumeFlo &&
      volume != fScoringVolumePMT &&
      volume != fScoringVolumeAir1 &&
      volume != fScoringVolumeAir2 &&
      // volume != fScoringVolumeAir3 &&
      // volume != fScoringVolumeAir4 &&
      volume != fScoringVolumePP1 &&
      // volume != fScoringVolumePP2 &&
      // volume != fScoringVolumePb &&
      volume != fScoringVolumeGlass)
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
      evtAction->flag_air2 = false;
      // evtAction->flag_air3 = false;
      // evtAction->flag_air4 = false;
      // evtAction->flag_pb = false;
      // evtAction->flag_pp2 = false;
      // evtAction->flag_hdpe = false;
      //  //        evtAction->flag_flo  = false;
      //***Detector***//
      evtAction->flag_det1 = false;
      // evtAction->flag_dhdpe = false;
    }

    pTrackID = tID;

    // if (evtAction->flag_air1 && evtAction->flag_steel && evtAction->flag_air2)
    // {
    //   //       //   evtAction->flag_lab1 = false;
    //   evtAction->flag_steel = false;
    //   // evtAction->flag_hdpe = false;
    //   evtAction->flag_air2 = false;
    // }

    // if (evtAction->flag_det1 && evtAction->flag_dsteel && evtAction->flag_det2)
    // {
    //   evtAction->flag_dsteel = false;
    //   // evtAction->flag_dhdpe = false;
    //   evtAction->flag_det2 = false;
    // }


    // if (volume == fScoringVolumeAir2)
    // {
    //   evtAction->flag_air2 = true;

    //   evtAction->air2_eID.push_back(eID);
    //   evtAction->air2_pID.push_back(pID);

    //   evtAction->air2_px.push_back(pos.getX());
    //   evtAction->air2_py.push_back(pos.getY());
    //   evtAction->air2_pz.push_back(pos.getZ());

    //   evtAction->air2_mx.push_back(mom.getX());
    //   evtAction->air2_my.push_back(mom.getY());
    //   evtAction->air2_mz.push_back(mom.getZ());

    //   evtAction->air2_tID.push_back(tID);
    //   evtAction->air2_KE.push_back(KE / MeV);

    //   evtAction->air2_vz.push_back(vz);

    //   //  //        runAction->NeutronEnTr << eID << "\t" << pID << "\t" << tID << "\t"
    //   //  //                               << KE/MeV
    //   //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
    //   //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
    //   //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
    //   //  //                               << G4endl;
    //   evtAction->labcount++;
    // }

    if (volume == fScoringVolumeAir2 &&
    fStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
    {
    evtAction->flag_air2 = true;

    evtAction->air2_eID.push_back(eID);
    evtAction->air2_pID.push_back(pID);

    evtAction->air2_px.push_back(pos.getX());
    evtAction->air2_py.push_back(pos.getY());
    evtAction->air2_pz.push_back(pos.getZ());

    evtAction->air2_mx.push_back(mom.getX());
    evtAction->air2_my.push_back(mom.getY());
    evtAction->air2_mz.push_back(mom.getZ());

    evtAction->air2_tID.push_back(tID);
    evtAction->air2_KE.push_back(KE / MeV);

    evtAction->air2_vz.push_back(vz);

    evtAction->labcount++;
    }

    // if (volume == fScoringVolumePb)
    // {
    //   evtAction->flag_pb = true;

    //   evtAction->pb_eID.push_back(eID);
    //   evtAction->pb_pID.push_back(pID);

    //   evtAction->pb_px.push_back(pos.getX());
    //   evtAction->pb_py.push_back(pos.getY());
    //   evtAction->pb_pz.push_back(pos.getZ());

    //   evtAction->pb_mx.push_back(mom.getX());
    //   evtAction->pb_my.push_back(mom.getY());
    //   evtAction->pb_mz.push_back(mom.getZ());

    //   evtAction->pb_tID.push_back(tID);
    //   evtAction->pb_KE.push_back(KE / MeV);

    //   evtAction->pb_vz.push_back(vz);

    //   //  //        runAction->NeutronEnTr << eID << "\t" << pID << "\t" << tID << "\t"
    //   //  //                               << KE/MeV
    //   //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
    //   //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
    //   //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
    //   //  //                               << G4endl;
    //   evtAction->labcount++;
    // }

    // if (volume == fScoringVolumeAir3)
    // {
    //   evtAction->flag_air3 = true;

    //   evtAction->air3_eID.push_back(eID);
    //   evtAction->air3_pID.push_back(pID);

    //   evtAction->air3_px.push_back(pos.getX());
    //   evtAction->air3_py.push_back(pos.getY());
    //   evtAction->air3_pz.push_back(pos.getZ());

    //   evtAction->air3_mx.push_back(mom.getX());
    //   evtAction->air3_my.push_back(mom.getY());
    //   evtAction->air3_mz.push_back(mom.getZ());

    //   evtAction->air3_tID.push_back(tID);
    //   evtAction->air3_KE.push_back(KE / MeV);

    //   evtAction->air3_vz.push_back(vz);

    //   //  //        runAction->NeutronEnTr << eID << "\t" << pID << "\t" << tID << "\t"
    //   //  //                               << KE/MeV
    //   //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
    //   //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
    //   //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
    //   //  //                               << G4endl;
    //   evtAction->labcount++;
    // }

    // if (volume == fScoringVolumePP2)
    // {
    //   evtAction->flag_pp2 = true;

    //   evtAction->pp2_eID.push_back(eID);
    //   evtAction->pp2_pID.push_back(pID);

    //   evtAction->pp2_px.push_back(pos.getX());
    //   evtAction->pp2_py.push_back(pos.getY());
    //   evtAction->pp2_pz.push_back(pos.getZ());

    //   evtAction->pp2_mx.push_back(mom.getX());
    //   evtAction->pp2_my.push_back(mom.getY());
    //   evtAction->pp2_mz.push_back(mom.getZ());

    //   evtAction->pp2_tID.push_back(tID);
    //   evtAction->pp2_KE.push_back(KE / MeV);

    //   evtAction->pp2_vz.push_back(vz);

    //   //  //        runAction->NeutronEnTr << eID << "\t" << pID << "\t" << tID << "\t"
    //   //  //                               << KE/MeV
    //   //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
    //   //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
    //   //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
    //   //  //                               << G4endl;
    //   evtAction->labcount++;
    // }

    // if (volume == fScoringVolumeAir4)
    // {
    //   evtAction->flag_air4 = true;

    //   evtAction->air4_eID.push_back(eID);
    //   evtAction->air4_pID.push_back(pID);

    //   evtAction->air4_px.push_back(pos.getX());
    //   evtAction->air4_py.push_back(pos.getY());
    //   evtAction->air4_pz.push_back(pos.getZ());

    //   evtAction->air4_mx.push_back(mom.getX());
    //   evtAction->air4_my.push_back(mom.getY());
    //   evtAction->air4_mz.push_back(mom.getZ());

    //   evtAction->air4_tID.push_back(tID);
    //   evtAction->air4_KE.push_back(KE / MeV);

    //   evtAction->air4_vz.push_back(vz);

    //   //  //        runAction->NeutronEnTr << eID << "\t" << pID << "\t" << tID << "\t"
    //   //  //                               << KE/MeV
    //   //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
    //   //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
    //   //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
    //   //  //                               << G4endl;
    //   evtAction->labcount++;
    // }

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

    // if (volume == fScoringVolumeSteel)
    // {
    //   if (evtAction->flag_air1)
    //     evtAction->flag_steel = true;

    //   if (evtAction->flag_det1)
    //     evtAction->flag_dsteel = true;
    // }

    // if (evtAction->flag_steel && volume == fScoringVolumeAir)
    // {
    //   evtAction->flag_air2 = true;

    //   //  //        G4cout << "====Backscattering found====" << G4endl;
    //   //  //        G4cout << eID << "\t" << tID << "\t" << KE/MeV << G4endl;

    //   evtAction->elBeID.push_back(eID);
    //   evtAction->elBpID.push_back(pID);

    //   evtAction->elBpx.push_back(pos.getX());
    //   evtAction->elBpy.push_back(pos.getY());
    //   evtAction->elBpz.push_back(pos.getZ());

    //   evtAction->elBmx.push_back(mom.getX());
    //   evtAction->elBmy.push_back(mom.getY());
    //   evtAction->elBmz.push_back(mom.getZ());

    //   evtAction->elBtID.push_back(tID);
    //   evtAction->elBKE.push_back(KE / MeV);

    //   evtAction->elBvz.push_back(vz);

    //   //  //        runAction->NeutronBack << eID << "\t" << pID << "\t" << tID << "\t"
    //   //  //                               << KE/MeV
    //   //  //                               << "\t" << posx << "\t" << posy << "\t" << posz
    //   //  //                               << "\t" << momx << "\t" << momy << "\t" << momz
    //   //  //                               << "\t" << dx << "\t" << dy << "\t" << dz
    //   //  //                               << G4endl;
    // }

    // if (evtAction->flag_dsteel && volume == fScoringVolumePMT)
    // {
    //   evtAction->flag_det2 = true;

    //   //          G4cout << "====Backscattering found====" << G4endl;
    //   //          G4cout << eID << "\t" << tID << "\t" << KE/MeV << G4endl;

    //   evtAction->edBeID.push_back(eID);
    //   evtAction->edBpID.push_back(pID);

    //   evtAction->edBpx.push_back(pos.getX());
    //   evtAction->edBpy.push_back(pos.getY());
    //   evtAction->edBpz.push_back(pos.getZ());

    //   evtAction->edBmx.push_back(mom.getX());
    //   evtAction->edBmy.push_back(mom.getY());
    //   evtAction->edBmz.push_back(mom.getZ());

    //   evtAction->edBtID.push_back(tID);
    //   evtAction->edBKE.push_back(KE / MeV);

    //   evtAction->edBvz.push_back(vz);
    // }

    //      G4cout << eID << "\t" << tID << "\t" << evtAction->flag_det1
    //             << "\t" << evtAction->flag_drock
    //             << "\t" << evtAction->flag_det2 << "\t" << volname << G4endl;
  }
}
