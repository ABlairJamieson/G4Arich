#ifndef G4ArichPhysicsListMessenger_h
#define G4ArichPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4ArichPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ArichPhysicsListMessenger: public G4UImessenger
{
  public:
    G4ArichPhysicsListMessenger(G4ArichPhysicsList* );
    virtual ~G4ArichPhysicsListMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:
    G4ArichPhysicsList*  fPhysicsList;
 
    G4UIdirectory*        fG4ArichDir;
    G4UIdirectory*        fPhysDir;
    G4UIcmdWithAnInteger* fVerboseCmd;
    G4UIcmdWithAnInteger* fCerenkovCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
