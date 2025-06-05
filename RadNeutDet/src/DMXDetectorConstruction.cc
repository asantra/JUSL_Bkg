
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------

//
// DetectorConstruction.cc
// --------------------------------------------------------------


#include "DMXDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4TransportationManager.hh"
#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"


DMXDetectorConstruction::DMXDetectorConstruction()  
{
  theUserLimitsForRoom     = 0; 
  theMaxTimeCuts      = DBL_MAX;
  theMaxStepSize      = DBL_MAX;
  theRoomTimeCut      = 1000. * nanosecond;
  theMinEkine         = 250.0*eV; // minimum kinetic energy required in volume
  theRoomMinEkine     = 250.0*eV; // minimum kinetic energy required in volume

}


DMXDetectorConstruction::~DMXDetectorConstruction() 
{
  delete theUserLimitsForRoom;
}


void DMXDetectorConstruction::DefineMaterials() 
{

#include "DMXDetectorMaterial.icc"

}


G4VPhysicalVolume* DMXDetectorConstruction::Construct()
{
  DefineMaterials();

  // make colours
  G4Colour  white   (1.0, 1.0, 1.0) ;
  G4Colour  grey    (0.5, 0.5, 0.5) ;
  G4Colour  lgrey   (.85, .85, .85) ;
  G4Colour  red     (1.0, 0.0, 0.0) ;
  G4Colour  blue    (0.0, 0.0, 1.0) ;
  G4Colour  cyan    (0.0, 1.0, 1.0) ;
  G4Colour  magenta (1.0, 0.0, 1.0) ; 
  G4Colour  yellow  (1.0, 1.0, 0.0) ;
  G4Colour  orange  (.75, .55, 0.0) ;
  G4Colour  lblue   (0.0, 0.0, .75) ;
  G4Colour  lgreen  (0.0, .75, 0.0) ;
  G4Colour  green   (0.0, 1.0, 0.0) ;
  G4Colour  brown   (0.7, 0.4, 0.1) ;
  
  // World - Vacuum **************************************************


  G4double world_innerRadius = 0.*cm;
  G4double world_outerRadius = 440.*cm;
  
  /*G4Sphere* world = new G4Sphere("World",
				 world_innerRadius,
				 world_outerRadius,
				 0.*deg, 360.*deg,
                                 0.*deg, 90.*deg);*/
  G4Box* world = new G4Box("World",
                           480.*cm,480.*cm,480.*cm);//CHANGE HERE


  /*
  G4double worldLength = 700.0*cm;
  G4double worldWidth  = 700.0*cm;
  G4double worldHeight = 400.0*cm;
  
  G4Box* world = new G4Box( "World",
			    0.5*worldLength,
			    0.5*worldWidth,
			    0.5*worldHeight );
  */
  
  world_log  = new G4LogicalVolume(world, world_mat, "world_log");
  world_phys = new G4PVPlacement(0,
				 G4ThreeVector(0.,0.,0.),
				 "world_phys",
				 world_log,
				 NULL,
				 false,
				 0);

  G4VisAttributes* world_vat= new G4VisAttributes(cyan);
  world_vat->SetVisibility(true);
  world_vat->SetForceSolid(false);
  world_log->SetVisAttributes(world_vat);
  
  // Cavern - Rock ***********************************************************


  G4double cavern_innerRadius = 220.1*cm;
  G4double cavern_outerRadius = 470.1*cm;//CHANGE HERE

  G4Sphere* cavern = new G4Sphere("Cavern",
				  cavern_innerRadius,
				  cavern_outerRadius,
				  0.*deg, 360.*deg,
                                  0.*deg, 90.*deg);


  /*
  G4double cavernThickness = 100.0*cm;
  
  G4double cavernLength = 500.0*cm + 2.0*cavernThickness;
  G4double cavernWidth  = 500.0*cm + 2.0*cavernThickness;
  G4double cavernHeight = 200.0*cm + 2.0*cavernThickness;
  
  G4Box* cavern = new G4Box( "Cavern",
			     0.5*cavernLength,
			     0.5*cavernWidth,
			     0.5*cavernHeight );
  */

  cavern_log  = new G4LogicalVolume(cavern, cavern_mat, "cavern_log");
  cavern_phys = new G4PVPlacement(0,
				  G4ThreeVector(0.,0.,0.),
				  "cavern_phys",
				  cavern_log,
				  world_phys,
				  false,
				  0);

  G4VisAttributes* cavern_vat= new G4VisAttributes(red);
  cavern_vat->SetVisibility(true);
  cavern_vat->SetForceSolid(false);
  cavern_log->SetVisAttributes(cavern_vat);
  fScoringVolumeCav = cavern_log;


  // Lab Space - AIR ********************************************************


  G4double lab_innerRadius = 0.*cm;
  G4double lab_outerRadius = 220.1*cm;
  
  G4Sphere* lab  = new G4Sphere( "lab",
				 lab_innerRadius,
				 lab_outerRadius,
				 0.*deg, 360.*deg,
                                 0.*deg, 90.*deg );


  /*
  G4double labLength = cavernLength - 2.0*cavernThickness;
  G4double labWidth  = cavernWidth  - 2.0*cavernThickness;
  G4double labHeight = cavernHeight - 2.0*cavernThickness;

  G4Box* lab = new G4Box( "lab",
			  0.5*labLength,
			  0.5*labWidth,
			  0.5*labHeight );
  */


  lab_log  = new G4LogicalVolume(lab, lab_mat, "lab_log");
  lab_phys = new G4PVPlacement(0,
			       G4ThreeVector(0.,0.,0.),
			       "lab_phys",
			       lab_log,
			       world_phys,
			       false,
			       0);

  
  G4VisAttributes* lab_vat= new G4VisAttributes(white);
  lab_vat->SetVisibility(true);
  lab_vat->SetForceSolid(false);
  lab_log->SetVisAttributes(lab_vat);
  fScoringVolumeLab = lab_log;

  //Floor***************************************

  G4double floor_innerRadius = 0.*cm;
  G4double floor_outerRadius = 470.1*cm;//CHANGE HERE

  G4Tubs* floor = new G4Tubs("floor",
                             floor_innerRadius,
                             floor_outerRadius,
                             125.*cm,
                             0.0*deg, 360.*deg);
  floor_log = new G4LogicalVolume(floor, cavern_mat, "floor_log");
  floor_phys= new G4PVPlacement(0,G4ThreeVector(0.,0.,-125.*cm),
                                "floor_phys",
                                floor_log,
                                world_phys,
                                false,
                                0);

  G4VisAttributes *floor_vat = new G4VisAttributes(red);
  floor_vat->SetVisibility(true);
  floor_vat->SetForceSolid(true);
  floor_log->SetVisAttributes(floor_vat);
  fScoringVolumeFlo = floor_log;


  
  //Detector ************************************************************
  
  /*
  G4double pmt_innerRadius = 0.0;
  G4double pmt_outerRadius = 2.5*cm;

  G4Sphere* pmt_sol = new G4Sphere("PMT",
                                   pmt_innerRadius,
                                   pmt_outerRadius,
                                   0.*deg, 360.*deg,
                                   0.*deg, 180.*deg);

  */

  G4double pmt_innerRadius = 0.*mm;
  G4double pmt_outerRadius = 70.*mm;
  G4double pmt_halfz       = 300.*mm;

  G4Tubs* pmt_sol = new G4Tubs("PMT",
                                pmt_innerRadius,
                                pmt_outerRadius,
                                pmt_halfz,
                                0.*deg,
                                360.*deg);
  
  G4ThreeVector pmt_pos = G4ThreeVector(0.*cm , 0.*cm , 60.*cm);

  G4RotationMatrix *rotm = new G4RotationMatrix();
  rotm->rotateY(90.0*deg);

  
  pmt_log  = new G4LogicalVolume( pmt_sol, pmt_mat, "pmt_log" );
  pmt_phys = new G4PVPlacement(rotm,
                   pmt_pos,
                   "pmt_phys",
                   pmt_log,
                   lab_phys,
                   false,
                   0);
  
  G4VisAttributes* pmt_vat= new G4VisAttributes(yellow);
  pmt_vat->SetForceSolid(true);
  pmt_vat->SetVisibility(true);
  pmt_log->SetVisAttributes(pmt_vat);

  //Setting PMT tube as scoring volume for tracking
  fScoringVolumePMT = pmt_log;
 

  // attach user limits ...................................................
  
  G4cout << G4endl << "User Limits: " << G4endl 
	 << "\t theMaxTimeCuts:     " << G4BestUnit(theMaxTimeCuts,"Time")  
	 << G4endl
	 << "\t theRoomTimeCut:     " << G4BestUnit(theRoomTimeCut,"Time")  
	 << G4endl
	 << "\t theMaxStepSize:     " << G4BestUnit(theMaxStepSize,"Length")
	 << G4endl
	 << "\t theMinEKine:        " << G4BestUnit(theMinEkine,"Energy")   
	 << G4endl
	 << "\t minRoomMinEKine:    " << G4BestUnit(theRoomMinEkine,"Energy")
	 << G4endl << G4endl;
  
  if (theUserLimitsForRoom != 0) delete theUserLimitsForRoom;
  
  theUserLimitsForRoom = new G4UserLimits(theMaxStepSize,   // step length max
					  DBL_MAX,          // track length max
					  theRoomTimeCut,   // Time cut
					  theRoomMinEkine); // min energy

  world_log->SetUserLimits(theUserLimitsForRoom);
  lab_log->SetUserLimits(theUserLimitsForRoom);
  cavern_log->SetUserLimits(theUserLimitsForRoom);
  pmt_log->SetUserLimits(theUserLimitsForRoom);
  return world_phys;
  
}

// specific method to G4UserLimits:= SetUserMinEkine
void DMXDetectorConstruction::SetRoomEnergyCut(G4double val)
{
  // set minimum charged particle energy cut - NB: for ROOM
  theRoomMinEkine = val;
  if (theUserLimitsForRoom != 0) 
    {
      theUserLimitsForRoom->SetUserMinEkine(val); 
      G4cout << " Changing Room energy cut to: " << G4BestUnit(val,"Energy")
	     << G4endl;
    }
}  

// specific method to G4UserLimits:= SetUserMaxTime
void DMXDetectorConstruction::SetRoomTimeCut(G4double val)
{
  // set room time cut:
  theRoomTimeCut = val;
  if (theUserLimitsForRoom != 0) 
    {
      theUserLimitsForRoom->SetUserMaxTime(val);
      G4cout << " Changing Room Time cut to: " << G4BestUnit(val,"Time")
	     << G4endl;
    }
}  






