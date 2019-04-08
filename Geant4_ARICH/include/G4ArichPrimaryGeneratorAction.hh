// Modified based on OpNovice example
#ifndef G4ArichPrimaryGeneratorAction_h
#define G4ArichPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class G4ArichPrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ArichPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    G4ArichPrimaryGeneratorAction();
    virtual ~G4ArichPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);

    void SetSourceType( G4String type ){ fSourceType = type; }
  
  private:
    G4String       fSourceType;
    G4ParticleGun* fParticleGun;
    G4GeneralParticleSource* fParticleGPS;
    G4ArichPrimaryGeneratorMessenger* fGunMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*G4ArichPrimaryGeneratorAction_h*/
