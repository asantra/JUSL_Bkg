//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//   SHUBHAM DUTTA
// --------------------------------------------------------------
// SpecialCuts program.CC
// --------------------------------------------------------------

#include "DMXSpecialCuts.hh"
#include "G4VParticleChange.hh"
#include "G4Track.hh"
#include "G4Step.hh"

DMXSpecialCuts::DMXSpecialCuts(const G4String& aName)
  : G4VProcess(aName)
{
   if (verboseLevel>1)
     {
       G4cout << GetProcessName() << " is created "<< G4endl;
     }

}

DMXSpecialCuts::~DMXSpecialCuts() 
{}                            

G4VParticleChange* DMXSpecialCuts::PostStepDoIt(
			     const G4Track& aTrack,
			     const G4Step& 
			    )

// Stop the current particle, if requested by G4UserLimits 
{
   aParticleChange.Initialize(aTrack);
   aParticleChange.ProposeEnergy(0.) ;
   aParticleChange.ProposeLocalEnergyDeposit (aTrack.GetKineticEnergy()) ;
   aParticleChange.ProposeTrackStatus(fStopButAlive);
   return &aParticleChange;
}

G4double DMXSpecialCuts::PostStepGetPhysicalInteractionLength(
                             const G4Track&,
                             G4double,
                             G4ForceCondition*
                            )
{
  return DBL_MAX;
}

