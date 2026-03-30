//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
// --------------------------------------------------------------
//
// SHUBHAM DUTTA
//
// DetectorConstruction.hh
// --------------------------------------------------------------


#ifndef DMXDetectorConstruction_h
#define DMXDetectorConstruction_h 1


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"


class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;


class G4UserLimits;


class DMXDetectorConstruction : public G4VUserDetectorConstruction
{
public:


 DMXDetectorConstruction();
 ~DMXDetectorConstruction();


public:


 G4VPhysicalVolume* Construct();


 G4LogicalVolume* GetScoringVolumePMT() const {return fScoringVolumePMT;}
 G4LogicalVolume* GetScoringVolumeLab() const {return fScoringVolumeLab;}
 G4LogicalVolume* GetScoringVolumeCav() const {return fScoringVolumeCav;}
 G4LogicalVolume* GetScoringVolumeFlo() const {return fScoringVolumeFlo;}
//  G4LogicalVolume* GetScoringVolumeSteel() const {return fScoringVolumeSteel;}
//  G4LogicalVolume* GetScoringVolumeHDPE() const {return fScoringVolumeHDPE;}
 G4LogicalVolume* GetScoringVolumeGlass() const {return fScoringVolumeGlass;}
 G4LogicalVolume* GetScoringVolumeAir1() const {return fScoringVolumeAir1;}
 G4LogicalVolume* GetScoringVolumeAir2() const {return fScoringVolumeAir2;}
//  G4LogicalVolume* GetScoringVolumeAir3() const {return fScoringVolumeAir3;}
//  G4LogicalVolume* GetScoringVolumeAir4() const {return fScoringVolumeAir4;}
 G4LogicalVolume* GetScoringVolumePP1() const {return fScoringVolumePP1;}
//  G4LogicalVolume* GetScoringVolumePP2() const {return fScoringVolumePP2;}
//  G4LogicalVolume* GetScoringVolumePb() const {return fScoringVolumePb;}




 void SetRoomEnergyCut(G4double);
 void SetEnergyCut(G4double);
 void SetTimeCut(G4double);
 void SetRoomTimeCut(G4double);


private:


 void DefineMaterials();


 G4UserLimits*    theUserLimitsForRoom;
 G4UserLimits*    theUserLimitsForDetector;
 G4double         theMaxTimeCuts;
 G4double         theMaxStepSize;
 G4double         theDetectorStepSize;
 G4double         theMinEkine;
 G4double         theRoomMinEkine;
 G4double         theRoomTimeCut;
 #include "DMXDetectorMaterial.ihh"  // materials used


 G4LogicalVolume*   world_log;     // pointers
 G4VPhysicalVolume* world_phys; 
 G4LogicalVolume*   cavern_log;
 G4VPhysicalVolume* cavern_phys;
 G4LogicalVolume*   lab_log;
 G4VPhysicalVolume* lab_phys; 
 G4LogicalVolume*   pmt_log;  
 G4VPhysicalVolume* pmt_phys;
 G4LogicalVolume*   floor_log;
 G4VPhysicalVolume* floor_phys;
 // G4LogicalVolume*   pb_log;
 // G4VPhysicalVolume* pb_phys;
//  G4LogicalVolume*   steel_log;
//  G4VPhysicalVolume* steel_phys;
//  G4LogicalVolume*   hdpe_log;
//  G4VPhysicalVolume* hdpe_phys;
 G4LogicalVolume*   air1_log;
 G4VPhysicalVolume* air1_phys;
 G4LogicalVolume*   air2_log;
 G4VPhysicalVolume* air2_phys;
//  G4LogicalVolume*   air3_log;
//  G4VPhysicalVolume* air3_phys;
//  G4LogicalVolume*   air4_log;
//  G4VPhysicalVolume* air4_phys;
 G4LogicalVolume*   pp1_log;
 G4VPhysicalVolume* pp1_phys;
//  G4LogicalVolume*   pp2_log;
//  G4VPhysicalVolume* pp2_phys;
//  G4LogicalVolume*   pb_log;
//  G4VPhysicalVolume* pb_phys;
 G4LogicalVolume*   glass_log;
 G4VPhysicalVolume* glass_phys;


 G4LogicalVolume* fScoringVolumePMT;
 G4LogicalVolume* fScoringVolumeLab;
 G4LogicalVolume* fScoringVolumeCav;
 G4LogicalVolume* fScoringVolumeFlo;
 G4LogicalVolume* fScoringVolumePb;
//  G4LogicalVolume* fScoringVolumeSteel;
//  G4LogicalVolume* fScoringVolumeHDPE;
 G4LogicalVolume* fScoringVolumeAir1;
 G4LogicalVolume* fScoringVolumeAir2;
//  G4LogicalVolume* fScoringVolumeAir3;
//  G4LogicalVolume* fScoringVolumeAir4;
 G4LogicalVolume* fScoringVolumePP1;
//  G4LogicalVolume* fScoringVolumePP2;
 G4LogicalVolume* fScoringVolumeGlass;


};


#endif

