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
      fScoringVolume(0)
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
  G4double depth = 554.0 * m, hillmax = 132.0 * m;
  G4double env_sizeXY = 3000.1 * m, env_sizeZ = 556.0 * m + (2.0 * hillmax);
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

  //
  // *************Shape 1****NOT PLACED*******************
  //
  G4Material *shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -7 * cm);

  // Conical section shape
  G4double shape1_rmina = 0. * cm, shape1_rmaxa = 2. * cm;
  G4double shape1_rminb = 0. * cm, shape1_rmaxb = 4. * cm;
  G4double shape1_hz = 3. * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  G4Cons *solidShape1 =
      new G4Cons("Shape1",
                 shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb, shape1_hz,
                 shape1_phimin, shape1_phimax);

  G4LogicalVolume *logicShape1 =
      new G4LogicalVolume(solidShape1, // its solid
                          shape1_mat,  // its material
                          "Shape1");   // its name

  // new G4PVPlacement(0,                       //no rotation
  //                   pos1,                    //at position
  //                   logicShape1,             //its logical volume
  //                   "Shape1",                //its name
  //                   logicEnv,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

  //
  //**************RockMaterial**************************
  //

  G4Element *Si = nist->FindOrBuildElement("Si");
  G4Element *O = nist->FindOrBuildElement("O");
  G4Element *Al = nist->FindOrBuildElement("Al");
  G4Element *Fe = nist->FindOrBuildElement("Fe");
  G4Element *Ca = nist->FindOrBuildElement("Ca");
  G4Element *Mg = nist->FindOrBuildElement("Mg");
  G4Element *Na = nist->FindOrBuildElement("Na");
  G4Element *K = nist->FindOrBuildElement("K");
  G4Element *Ti = nist->FindOrBuildElement("Ti");
  G4Element *P = nist->FindOrBuildElement("P");
  G4Element *Mn = nist->FindOrBuildElement("Mn");
  G4Element *U = nist->FindOrBuildElement("U");
  G4Element *Mo = nist->FindOrBuildElement("Mo");
  G4Element *H = nist->FindOrBuildElement("H");
  G4Element *S = nist->FindOrBuildElement("S");
  G4Element *Cu = nist->FindOrBuildElement("Cu");

  G4Material *rock_mat = new G4Material("Rock", 2.89 * g / cm3, 16); //******DENSITY CHANGED T0 2.85 FOR VARIATION PURPOSE*********//
  // G4Material* rock_mat = new G4Material
  //("Rock", 8.96*g/cm3, 1);
  //  vary the rock material composition by changing the fraction for uncertinty
  //*****This is SiO2-10%*****//
  rock_mat->AddElement(Si, 30.79 * perCent);
  rock_mat->AddElement(O, 48.02 * perCent);
  rock_mat->AddElement(Al, 9.525 * perCent);
  rock_mat->AddElement(Fe, 3.751 * perCent);
  rock_mat->AddElement(Ca, 1.300 * perCent);
  rock_mat->AddElement(Mg, 0.834 * perCent);
  rock_mat->AddElement(Na, 1.187 * perCent);
  rock_mat->AddElement(K, 2.166 * perCent);
  rock_mat->AddElement(Ti, 0.341 * perCent);
  rock_mat->AddElement(P, 0.079 * perCent);
  rock_mat->AddElement(Mn, 0.023 * perCent);
  rock_mat->AddElement(U, 0.004 * perCent);
  rock_mat->AddElement(Mo, 0.002 * perCent);
  rock_mat->AddElement(H, 0.028 * perCent);
  rock_mat->AddElement(S, 0.225 * perCent);
  rock_mat->AddElement(Cu, 1.725 * perCent);
  // rock_mat->AddElement(Cu, 1);

  //**AIR taken for testing...change to above rock//
  // G4Material* rock_mat_air =
  //       nist->FindOrBuildMaterial("G4_AIR");

  // ***************ROCK VOLUME**********************

  G4double rock_dx = 3000. * m, rock_dy = 3000. * m, rock_dz = depth + hillmax;

  G4Box *rock =
      new G4Box("DMRock",                                     // its name
                0.5 * rock_dx, 0.5 * rock_dy, 0.5 * rock_dz); // its size

  G4LogicalVolume *Rock1 =
      new G4LogicalVolume(rock,      // its solid
                          rock_mat,  // its material
                          "DMRock"); // its name

  G4VisAttributes *rock_color = new G4VisAttributes(color->Red());
  rock_color->SetForceSolid(false);
  rock_color->SetVisibility(true);
  Rock1->SetVisAttributes(rock_color);

  new G4PVPlacement(0,                                  // no rotation
                    G4ThreeVector(0, 0, 0.5 * hillmax), // at (0,0,0)
                    Rock1,                              // its logical volume
                    "DMRock",                           // its name
                    logicEnv,                           // its mother  volume
                    false,                              // no boolean operation
                    0,                                  // copy number
                    checkOverlaps);                     // overlaps checking

  //
  // ***************Shape 2****PLASTIC SCINTILLATOR TO DETECT MUONS*******************
  //
  G4Material *shape2_mat = nist->FindOrBuildMaterial("G4_AIR");

  //  G4ThreeVector pos3 = G4ThreeVector(0, 0, 9*cm);
  //  G4ThreeVector pos4 = G4ThreeVector(0, 0, 18*cm);
  //  G4ThreeVector pos5 = G4ThreeVector(0, 0, 27*cm);

  // Trapezoid shape
  // G4double shape2_dxa = 96.4*cm, shape2_dxb = 12*cm;
  G4double shape2_dxa = 3000. * m, shape2_dya = 3000. * m;
  // G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
  G4double shape2_dz = 0.2 * cm;
  G4ThreeVector pos2 = G4ThreeVector(0, 0, ((-0.5 * (rock_dz - hillmax)) - (0.5 * shape2_dz))); // placed below the rock

  G4Box *solidShape2 =
      new G4Box("Shape2",                                             // its name
                0.5 * shape2_dxa, 0.5 * shape2_dya, 0.5 * shape2_dz); // its size

  G4LogicalVolume *Scint =
      new G4LogicalVolume(solidShape2, // its solid
                          shape2_mat,  // its material
                          "Shape2");   // its name

  new G4PVPlacement(0,              // no rotation
                    pos2,           // at position
                    Scint,          // its logical volume
                    "Shape2",       // its name
                    logicEnv,       // its mother  volume
                    false,          // no boolean operation
                    0,              // copy number
                    checkOverlaps); // overlaps checking

  /*G4LogicalVolume* Scint1 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name


  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    Scint1,                  //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4LogicalVolume* Scint2 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

  /*new G4PVPlacement(0,                       //no rotation
                    pos3,                    //at position
                    Scint2,                  //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  /*G4LogicalVolume* Scint3 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

  new G4PVPlacement(0,                       //no rotation
                    pos4,                    //at position
                    Scint3,                  //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4LogicalVolume* Scint4 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

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
  //  fScoringVolume2 = Scint2;
  //  fScoringVolume3 = Scint3;
  //  fScoringVolume4 = Scint4;

  //
  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
