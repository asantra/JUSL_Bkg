//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fScoringVolume(0), fScoringVolume2(0), fScoringVolume3(0), fScoringVolume4(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  // Envelope parameters
  //
  // G4double depth = 554.0 * m, hillmax = 132.0 * m;
  G4double env_sizeXY = 50.1 * m, env_sizeZ = 50.1 * m;
  G4Material *env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  G4Color *color = new G4Color();
  //
  // ******************World********************
  //
  G4double world_sizeXY = 1.0 * env_sizeXY;
  G4double world_sizeZ = 1.0 * env_sizeZ;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidWorld =
      new G4Box("World",                                                    // its name
                0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ); // its size

  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, // its solid
                          world_mat,  // its material
                          "World");   // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        logicWorld,      // its logical volume
                        "World",         // its name
                        0,               // its mother  volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  G4VisAttributes *world_color = new G4VisAttributes(color->Blue());
  world_color->SetForceSolid(false);
  world_color->SetVisibility(false);
  logicWorld->SetVisAttributes(world_color);

  //
  // *************************Envelope***************************
  //
  G4Box *solidEnv =
      new G4Box("Envelope",                                           // its name
                0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ); // its size

  G4LogicalVolume *logicEnv =
      new G4LogicalVolume(solidEnv,    // its solid
                          env_mat,     // its material
                          "Envelope"); // its name

  new G4PVPlacement(0,               // no rotation
                    G4ThreeVector(), // at (0,0,0)
                    logicEnv,        // its logical volume
                    "Envelope",      // its name
                    logicWorld,      // its mother  volume
                    false,           // no boolean operation
                    0,               // copy number
                    checkOverlaps);  // overlaps checking


  // // ********************** Muon Shielding with Pb ****************************** // 

  G4Material* Pb_mat = nist->FindOrBuildMaterial("G4_Pb");
  G4double Pb_X = 33.5 *cm;
  G4double Pb_Y = 33.5 *cm;
  G4double Pb_Z = 28.0 *cm;

  G4Box* solidPb = new G4Box("Pb_shield",
                             0.5 * Pb_X,
                             0.5 * Pb_Y,
                             0.5 * Pb_Z);

  G4LogicalVolume* Pb_shield = new G4LogicalVolume(solidPb,
                                                    Pb_mat,
                                                    "Pb_shield");

  new G4PVPlacement(0,
                  G4ThreeVector(),
                  Pb_shield,
                  "Pb_shield",
                  logicEnv,
                  false,
                  0,
                  checkOverlaps);


  //*********** Air inside the pb shield */
  G4double shield_thickness = 5.1 *cm;
  G4double Air_X = Pb_X - 2 * shield_thickness;
  G4double Air_Y = Pb_Y - 2 * shield_thickness;
  G4double Air_Z = Pb_Z - (1.0 *cm  + shield_thickness);

  G4Box* solidAir = new G4Box("Air",
                             0.5 * Air_X,
                             0.5 * Air_Y,
                             0.5 * Air_Z);

  G4LogicalVolume* Air = new G4LogicalVolume(solidAir,
                                             env_mat,
                                             "Air");

  new G4PVPlacement(0,
                  G4ThreeVector(0, 0, -3.05 *cm),
                  Air,
                  "Air",
                  Pb_shield,
                  false,
                  0,
                  checkOverlaps);


  // ***************Shape 2****PLASTIC SCINTILLATOR TO DETECT MUONS*******************
  // ----------- SCINTILLATOR ON SURFACE -------------

  G4Material* scint_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  G4double scintXY = 16.0 * cm;
  G4double scintZ  = 1 * cm;

  // Create box
  G4Box* solidScint = new G4Box("Scint",
                                0.5 * scintXY,
                                0.5 * scintXY,
                                0.5 * scintZ);

  G4LogicalVolume* Scint = new G4LogicalVolume(solidScint,
                                                    scint_mat,
                                                    "Scint");
  // // ************** with  positions wrt the air inside the env volume ********************************
  // new G4PVPlacement(0,
  //                 G4ThreeVector(0, 0, 6.15 *cm),  // without shield position= 6.15cm
  //                 Scint,
  //                 "Scint",
  //                 logicEnv,
  //                 false,
  //                 0,
  //                 checkOverlaps);

  // G4LogicalVolume* Scint2 =
  //   new G4LogicalVolume(solidScint,         //its solid
  //                       scint_mat,          //its material
  //                       "Scint2");           //its name

  // new G4PVPlacement(0,                       //no rotation
  //                   G4ThreeVector(0, 0, -2.55 *cm),                   // without shield position= -2.55cm
  //                   Scint2,                  //its logical volume
  //                   "Scint2",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

  // G4LogicalVolume* Scint3 =
  //   new G4LogicalVolume(solidScint,         //its solid
  //                       scint_mat,          //its material
  //                       "Scint3");           //its name

  // new G4PVPlacement(0,                       //no rotation
  //                   G4ThreeVector(0, 0, -11.25 *cm),                    // without shield position= -11.25cm
  //                   Scint3,                  //its logical volume
  //                   "Scint3",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking


  // ************** with shield positions wrt the air inside the pb shield ********************************
  new G4PVPlacement(0,
                  G4ThreeVector(0, 0, 9.2 *cm),  // without shield position= 6.15cm
                  Scint,
                  "Scint",
                  Air,
                  false,
                  0,
                  checkOverlaps);

  G4LogicalVolume* Scint2 =
    new G4LogicalVolume(solidScint,         //its solid
                        scint_mat,          //its material
                        "Scint2");           //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, 0.5 *cm),                   // without shield position= -2.55cm
                    Scint2,                  //its logical volume
                    "Scint2",                //its name
                    Air,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4LogicalVolume* Scint3 =
    new G4LogicalVolume(solidScint,         //its solid
                        scint_mat,          //its material
                        "Scint3");           //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, -8.2 *cm),                    // without shield position= -11.25cm
                    Scint3,                  //its logical volume
                    "Scint3",                //its name
                    Air,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  // ********************************************************************************************//
  // G4LogicalVolume* Scint4 =
  //   new G4LogicalVolume(solidShape2,         //its solid
  //                       shape2_mat,          //its material
  //                       "Shape2");           //its name

  /*new G4PVPlacement(0,                       //no rotation
                    pos5,                    //at position
                    Scint4,                  //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking*/
  // Setting Air beneath Rock as Scoring Volume
  // Set Scint as the scoring volume
  fScoringVolume = Scint;
  // Set Shape2 as scoring volume
  //
  //  fScoringVolume1 = Scint1;
   fScoringVolume2 = Scint2;
   fScoringVolume3 = Scint3;
   fScoringVolume4 = Pb_shield;
  //  fScoringVolume4 = Scint4;

  //
  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
