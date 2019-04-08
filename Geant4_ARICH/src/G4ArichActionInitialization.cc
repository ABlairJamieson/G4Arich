// modified from OpNovice example

#include "G4ArichActionInitialization.hh"
#include "G4ArichPrimaryGeneratorAction.hh"
#include "G4ArichRunAction.hh"
#include "G4ArichSteppingAction.hh"
#include "G4ArichStackingAction.hh"
#include "G4ArichSteppingVerbose.hh"
#include "PTFDigitizeEvent.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichActionInitialization::G4ArichActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichActionInitialization::~G4ArichActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ArichActionInitialization::BuildForMaster() const
{
  SetUserAction(new G4ArichRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ArichActionInitialization::Build() const
{
  SetUserAction(new G4ArichPrimaryGeneratorAction());
  SetUserAction(new G4ArichRunAction());
  SetUserAction(new G4ArichSteppingAction());
  SetUserAction(new G4ArichStackingAction());
  SetUserAction(new PTFDigitizeEvent());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose*
               G4ArichActionInitialization::InitializeSteppingVerbose() const
{
  return new G4ArichSteppingVerbose();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
