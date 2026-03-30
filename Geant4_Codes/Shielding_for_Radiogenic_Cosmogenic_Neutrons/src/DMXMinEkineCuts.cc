//
// DMXMinEkineCuts.cc 
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
//
// --------------------------------------------------------------

#include "DMXMinEkineCuts.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4LossTableManager.hh"

DMXMinEkineCuts::DMXMinEkineCuts(const G4String& aName)
  : DMXSpecialCuts(aName)
{
  if (verboseLevel>1)
    G4cout << GetProcessName() << " is created "<< G4endl;
   
  SetProcessType(fUserDefined);

}

DMXMinEkineCuts::~DMXMinEkineCuts()
{}

DMXMinEkineCuts::DMXMinEkineCuts(DMXMinEkineCuts&)
  : DMXSpecialCuts()
{}


G4double DMXMinEkineCuts::PostStepGetPhysicalInteractionLength(
                             const G4Track& aTrack,
			     G4double ,
			     G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;
  
  G4double     proposedStep = DBL_MAX;

  // get the pointer to UserLimits
  G4UserLimits* pUserLimits = aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
  const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
  G4ParticleDefinition* aParticleDef = aTrack.GetDefinition();
  
  if (pUserLimits && aParticleDef->GetPDGCharge() != 0.0)
    {
      //min kinetic energy
      G4double temp = DBL_MAX;
      G4double    eKine     = aParticle->GetKineticEnergy();
      const G4MaterialCutsCouple* couple = aTrack.GetMaterialCutsCouple();
      G4double eMin = pUserLimits->GetUserMinEkine(aTrack);

      if (eKine < eMin)
	return 0.;
      
      G4double    rangeNow = DBL_MAX;
      
      G4LossTableManager* lossManager = G4LossTableManager::Instance();
      rangeNow = lossManager->GetRange(aParticleDef,eKine,couple);
      
      // charged particles only      
      G4double rangeMin = lossManager->GetRange(aParticleDef,eMin,couple); 
      temp = rangeNow - rangeMin;
      if (proposedStep > temp) proposedStep = temp;  
    }

  return proposedStep;
}
