//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//      For information related to this code contact: Alex Howard
//      e-mail: alexander.howard@cern.ch
// --------------------------------------------------------------
//
// SHUBHAM DUTTA
//
// PhysicsList header
// --------------------------------------------------------------
// DMXPhysicsList.hh
#ifndef DMXPhysicsList_h
#define DMXPhysicsList_h 1

#include "FTFP_BERT.hh"  // <--- Important

class DMXPhysicsList : public FTFP_BERT  // <--- Now inherits from FTFP_BERT directly
{
public:
  DMXPhysicsList();
  ~DMXPhysicsList() override;

  void SetCuts() override;
};

#endif


