//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
// --------------------------------------------------------------
//
// MaxTimeCuts.cc
// --------------------------------------------------------------

#include "DMXMaxTimeCuts.hh"

#include "G4PhysicalConstants.hh"
#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"

DMXMaxTimeCuts::DMXMaxTimeCuts(const G4String& aName)
  : DMXSpecialCuts(aName)
{
   if (verboseLevel>1)
     G4cout << GetProcessName() << " is created "<< G4endl;
   
   SetProcessType(fUserDefined);

  
}

DMXMaxTimeCuts::~DMXMaxTimeCuts()
{}

DMXMaxTimeCuts::DMXMaxTimeCuts(DMXMaxTimeCuts&)
  : DMXSpecialCuts()
{}

 
G4double DMXMaxTimeCuts::PostStepGetPhysicalInteractionLength(
			     const G4Track& aTrack,
			     G4double ,
			     G4ForceCondition* condition
			    )
{
  // condition is set to "Not Forced"
  *condition = NotForced;

   G4double proposedStep = DBL_MAX;

   // get the pointer to UserLimits
   G4UserLimits* pUserLimits = aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
   const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();

   // can apply cuts for specific particles - use if(particleDef):
   //   G4ParticleDefinition* aParticleDef = aTrack.GetDefinition();

   //   G4cout << " Time: " << pUserLimits->GetUserMaxTime(aTrack) << G4endl;
  
   if (pUserLimits) {
     G4double temp = DBL_MAX;
     //max time limit
     G4double dTime= (pUserLimits->GetUserMaxTime(aTrack) - aTrack.GetGlobalTime());
     if (dTime < 0. ) {
       proposedStep = 0.;
     } else {  
       G4double beta = (aParticle->GetTotalMomentum())/(aParticle->GetTotalEnergy());
       temp = beta*c_light*dTime;
       if (proposedStep > temp) proposedStep = temp;                  
     }

   }
   return proposedStep;
}
