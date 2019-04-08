// modified from OpNovice Example
#ifndef G4ArichActionInitialization_h
#define G4ArichActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class B4DetectorConstruction;

/// Action initialization class.
///

class G4ArichActionInitialization : public G4VUserActionInitialization
{
  public:
    G4ArichActionInitialization();
    virtual ~G4ArichActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

    virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;
};

#endif
