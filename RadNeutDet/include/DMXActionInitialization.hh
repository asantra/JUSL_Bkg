
// ActionInitialization.hh

#ifndef DMXActionInitialization_h
#define DMXActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class DMXActionInitialization : public G4VUserActionInitialization
{
  public:
    DMXActionInitialization();
    virtual ~DMXActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
