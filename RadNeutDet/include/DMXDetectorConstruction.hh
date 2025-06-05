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

 
  G4LogicalVolume* fScoringVolumePMT;
  G4LogicalVolume* fScoringVolumeLab;
  G4LogicalVolume* fScoringVolumeCav;
  G4LogicalVolume* fScoringVolumeFlo;

 };

#endif

