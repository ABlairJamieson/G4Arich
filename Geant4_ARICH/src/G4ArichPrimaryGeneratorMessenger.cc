// Modified from OpNovice example

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ArichPrimaryGeneratorMessenger.hh"

#include "G4ArichPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichPrimaryGeneratorMessenger::
  G4ArichPrimaryGeneratorMessenger(G4ArichPrimaryGeneratorAction* G4ArichGun)
  : G4UImessenger(),
    fG4ArichAction(G4ArichGun)
{
  fGunDir = new G4UIdirectory("/G4Arich/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fSourceCmd = new G4UIcmdWithAString("/G4Arich/gun/optSource",this);
  fSourceCmd->SetGuidance("Set source type to either gun, gps, or root");
  fSourceCmd->SetDefaultValue("gun");
  
  fPolarCmd =
           new G4UIcmdWithADoubleAndUnit("/G4Arich/gun/optPhotonPolar",this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle",true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichPrimaryGeneratorMessenger::~G4ArichPrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fGunDir;
  delete fSourceCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ArichPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if ( command == fSourceCmd ){
    if ( newValue == "gps" || newValue == "gun" || newValue == "root" ) {
      fG4ArichAction->SetSourceType( newValue );
    } else {
      std::cout<<"G4ArichPrimaryGeneratorMessenger::SetNewValue "
	       <<newValue<<" is neither gps nor gun" <<std::endl;
    }
  } else if( command == fPolarCmd ) {
      G4double angle = fPolarCmd->GetNewDoubleValue(newValue);
      if ( angle == -360.0*deg ) {
         fG4ArichAction->SetOptPhotonPolar();
      } else {
         fG4ArichAction->SetOptPhotonPolar(angle);
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
