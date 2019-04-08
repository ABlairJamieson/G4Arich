
// Modified from: OpNoviceSteppingAction.cc 
//


#include "G4ArichSteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "CathodeSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4ArichSteppingAction::G4ArichSteppingAction()
: G4UserSteppingAction()
{ 
  fScintillationCounter = 0;
  fCerenkovCounter      = 0;
  fEventNumber = -1;
  //fSD = new CathodeSD( "Cathode" );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichSteppingAction::~G4ArichSteppingAction()
{ ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// print out step info
void PrintG4Step( const G4Step* step ){
  const G4Track              * track    = step->GetTrack();
  const G4ParticleDefinition * particle = track->GetParticleDefinition();
  const G4StepPoint          * pre      = step->GetPreStepPoint();
  const G4ThreeVector        & prepos  = pre->GetPosition();
  const G4VPhysicalVolume    * prevol   = pre->GetPhysicalVolume();
  const G4StepPoint          * post     = step->GetPostStepPoint();
  const G4ThreeVector        & postpos  = post->GetPosition();
  const G4VPhysicalVolume    * postvol  = post->GetPhysicalVolume();

  std::cout << "TrackID = "<<track->GetTrackID()<<" Particle = "<<particle->GetParticleName() << std::endl;
  std::cout << "  Step start = ("
     << prepos.x()/CLHEP::cm<<", "
     << prepos.y()/CLHEP::cm<<", "
     << prepos.z()/CLHEP::cm<<") cm"
     <<" in "<<prevol->GetName()
     << std::endl;
  
  std::cout << "  Step end   = ("
     << postpos.x()/CLHEP::cm<<", "
     << postpos.y()/CLHEP::cm<<", "
     << postpos.z()/CLHEP::cm<<") cm"
     <<" in "<<postvol->GetName()
     << std::endl;
  
  std::cout << " Steplength=" << step->GetStepLength()/CLHEP::cm <<" cm "
     << " firststep="<<step->IsFirstStepInVolume()
     << " laststep="<<step->IsLastStepInVolume()
     << " Edep=" <<step->GetTotalEnergyDeposit()/CLHEP::eV <<" eV "
     << std::endl;
	

}



void G4ArichSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventNumber = G4RunManager::GetRunManager()->
                                              GetCurrentEvent()->GetEventID();

  if (eventNumber != fEventNumber) {
    ///G4cout << " Number of Scintillation Photons in previous event: "
    //        << fScintillationCounter << G4endl;
    // G4cout << " Number of Cerenkov Photons in previous event: "
    //        << fCerenkovCounter << G4endl;
     fEventNumber = eventNumber;
     fScintillationCounter = 0;
     fCerenkovCounter = 0;
  }

  G4Track* track = step->GetTrack();

  G4String ParticleName = track->GetDynamicParticle()->
                                 GetParticleDefinition()->GetParticleName();

  if (ParticleName == "opticalphoton") {
    const G4Track              * track    = step->GetTrack();
    const G4ParticleDefinition * particle = track->GetParticleDefinition();
    const G4StepPoint          * pre      = step->GetPreStepPoint();
    const G4ThreeVector        & prepos  = pre->GetPosition();
    const G4VPhysicalVolume    * prevol   = pre->GetPhysicalVolume();
    const G4StepPoint          * post     = step->GetPostStepPoint();
    const G4ThreeVector        & postpos  = post->GetPosition();
    const G4VPhysicalVolume    * postvol  = post->GetPhysicalVolume();

    if ( step->GetTotalEnergyDeposit()/CLHEP::eV > 1.0 ) {
      //fSD->ProcessHits( step, nullptr );
      //fSD-
      //if ( prevol->GetName() == "R3600PCath_1" || prevol->GetName() == "R3600Cath_2" ||
      //	   prevol->GetName() == "R3600PCath_3" || prevol->GetName() == "R3600Cath_4" ) {
      
      //PrintG4Step( step );
	//}
    }
  }

  if(0){

  G4SteppingManager* SM = fpSteppingManager;
  G4Track* theTrack = step->GetTrack();
  G4cout<<"PhotInSteppingAction::UserSteppingAction: *** Material = "
        <<theTrack->GetMaterial()->GetName()<<" ***"<<G4endl;
  G4TrackVector* fSecondary = SM->GetfSecondary();
  G4int nSec = (*fSecondary).size(); // #of secondaries
  G4cout << std::setw( 5) << "#Step#"
         << std::setw( 9) << "X(mm)" << " "
         << std::setw( 9) << "Y(mm)" << " "
         << std::setw( 9) << "Z(mm)" << " "
         << std::setw( 9) << "KineE(MeV)"
         << std::setw( 9) << "dE(MeV)" << " "
         << std::setw( 9) << "StepLeng" << " "
         << std::setw( 9) << "TrackLeng" << " "
         << std::setw( 9) << "Particle" << "  "
         << std::setw( 9) << "ProcName" << G4endl;
  G4cout.precision(3);
  G4cout << std::setw( 5) << theTrack->GetCurrentStepNumber() << " "
         << std::setw( 9) << theTrack->GetPosition().x() / CLHEP::mm << " "
         << std::setw( 9) << theTrack->GetPosition().y() / CLHEP::mm << " "
         << std::setw( 9) << theTrack->GetPosition().z() / CLHEP::mm << " "
         << std::setw( 9) << theTrack->GetKineticEnergy() / CLHEP::MeV << " "
         << std::setw( 9) << step->GetTotalEnergyDeposit() / CLHEP::MeV << " "
         << std::setw( 9) << step->GetStepLength() / CLHEP::mm << " "
         << std::setw( 9) << theTrack->GetTrackLength() / CLHEP::mm << " "
         << std::setw( 9) << theTrack->GetDefinition()->GetParticleName()<< "   ";
         if(step->GetPostStepPoint()->GetProcessDefinedStep() != 0)
           G4cout<<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
         else G4cout<<"User Limit";
  G4cout << G4endl;

  // Dump processes for the particle
  G4ProcessManager* procMan=theTrack->GetDefinition()->GetProcessManager();
  procMan->DumpInfo();
  G4ProcessVector*  procVec = procMan->GetProcessList();
  G4int nofProc=procMan->GetProcessListLength();
  if(nofProc) for(G4int np=0; np<nofProc; np++)
  {
    G4VProcess* proc = (*procVec)[np];
    G4cout<<"PhotInSteppingAction::UserSteppingAction: "<<np<<", ProcName="
          <<proc->GetProcessName()<<", ProcType="<<proc->GetProcessType()<<G4endl;
  }

  }


  
  const std::vector<const G4Track*>* secondaries =
                                            step->GetSecondaryInCurrentStep();

  if (secondaries->size()>0) {
     for(unsigned int i=0; i<secondaries->size(); ++i) {
        if (secondaries->at(i)->GetParentID()>0) {
           if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
               == G4OpticalPhoton::OpticalPhotonDefinition()){
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
               == "Scintillation")fScintillationCounter++;
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
               == "Cerenkov")fCerenkovCounter++;
           }
        }
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
