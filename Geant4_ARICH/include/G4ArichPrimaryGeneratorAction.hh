// Modified based on OpNovice example
#ifndef G4ArichPrimaryGeneratorAction_h
#define G4ArichPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "RecEventReader.hh"

#include <vector>
#include <string>
using std::vector;
using std::string;

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class G4ArichPrimaryGeneratorMessenger;
class TChain;


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
  // InitializeRootVectors( listfile )
  // sets up chain of TTree for root event vectors
  // listfile is textfile with root file names to read
  // fills fIndices to keep track of number of unused events in root files
  void InitializeRootVectors(std::string listfile="root_vector_files.txt"); 

  // Read a random vector into fRecEvent from the chain of root files
  // if reuse_events is set to true, allow reusing an event that was
  // used before, otherwise will keep track of which events are left
  // to pick. Return index of event if there is another event, and -1
  // (largest unsigned) if all the events are used!
  unsigned long long ReadRandomRootVector(bool reuse_events = false);
  
  G4String       fSourceType;
  G4ParticleGun* fParticleGun;
  G4GeneralParticleSource* fParticleGPS;
  G4ArichPrimaryGeneratorMessenger* fGunMessenger;
  
  RecEvent  fRecEvent;  // Current event to generate, filled by reading fChain
  //unsigned long long fNumEvents; // Number of events in root files
  vector<unsigned long long> fIndices; // indices to unused events
  TChain*   fChain;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrintTrackPar(RecEvent &event);


#endif /*G4ArichPrimaryGeneratorAction_h*/
