//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
// DMX.CC
// --------------------------------------------------------------

#include "G4RunManager.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "DMXDetectorConstruction.hh"
//#include "DMXPhysicsList.hh"
#include "QGSP_BIC_HP.hh"
#include "Shielding.hh"
#include "DMXActionInitialization.hh"
#include "DMXPrimaryGeneratorAction.hh"
#include "DMXRunAction.hh"
#include "DMXEventAction.hh"
#include "DMXSteppingAction.hh"
#include "DMXStackingAction.hh"

int main(int argc,char** argv)
{
  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  
  
  #ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(20);
  #else
  G4RunManager* runManager = new G4RunManager;
  #endif
  
  // set mandatory initialization classes
  DMXDetectorConstruction* detector = new DMXDetectorConstruction;
  runManager->SetUserInitialization(detector);
  G4VUserPhysicsList* physics = new Shielding();
  runManager->SetUserInitialization(physics);
  runManager->SetUserInitialization(new DMXActionInitialization); //Uncomment for MT
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  
  
#ifdef DMXENV_GPS_USE
  G4cout << " Using GPS and not DMX gun " << G4endl;
#else
  G4cout << " Using the DMX gun " << G4endl;
#endif
  
  
  //***************COMMENT ALL WHEN MULTI-THREADING IS ENABLED***************
  
  // set user action classes
//  DMXRunAction* DMXRun = new DMXRunAction;
//  runManager->SetUserAction(DMXRun);
  
//  DMXPrimaryGeneratorAction* DMXGenerator = new DMXPrimaryGeneratorAction;
//  runManager->SetUserAction(DMXGenerator);

//  DMXEventAction* DMXEvent = new DMXEventAction(DMXRun, DMXGenerator);
//  runManager->SetUserAction(DMXEvent);
//  runManager->SetUserAction(new DMXSteppingAction(DMXRun, DMXEvent));
//  runManager->SetUserAction(new DMXStackingAction);
  
  //**************************************************************************

  //Initialize G4 kernel
  runManager->Initialize();
    
    
  // get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  // Define UI session for interactive mode.
  if(argc == 1)
    {
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute initInter.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif
    }
  // Batch mode
  else
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }

#ifdef G4VIS_USE
  if(visManager) delete visManager;
#endif
  delete runManager;

  return 0;
}

