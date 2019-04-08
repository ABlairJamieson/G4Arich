#include <iostream>
#include "CathodeSD.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

// print out step info
void PrintG4Step( G4Step* step ){
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


CathodeSD::CathodeSD( unsigned pmtid, G4String aname )
  : G4VSensitiveDetector( aname ), collectionID(-1), pmtID(pmtid), fName( aname )
{
  collectionName.insert( "cathode_hits" );
  G4SDManager * sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector( this );
}

G4bool CathodeSD::ProcessHits( G4Step * step, G4TouchableHistory *  ){
  //std::cout<<"CathodeSD::Processhits for "<<fName<<std::endl;
  const G4Track              * track    = step->GetTrack();
  const G4ParticleDefinition * particle = track->GetParticleDefinition();
  G4String ParticleName =  particle->GetParticleName();

  // PrintG4Step(  step );

  double ene =step->GetTotalEnergyDeposit()/CLHEP::eV;
  if ( ene > 1.0  && ParticleName == "opticalphoton" ){
    const G4StepPoint          * post     = step->GetPostStepPoint();
    const G4ThreeVector        & postpos  = post->GetPosition();
    const G4VPhysicalVolume    * postvol  = post->GetPhysicalVolume();
    double time = post->GetGlobalTime() / CLHEP::ns;
    const G4ThreeVector        & inidir  = track->GetVertexMomentumDirection();
    unsigned pmtnum = postvol->GetCopyNo();
    const G4ThreeVector        & inipos   = track->GetVertexPosition();

    
    fHits->insert( new PhotonCathodeHit( postpos, inidir, time, ene, inipos, pmtnum ) );
  }
  
  
  return true;
}


void CathodeSD::Initialize( G4HCofThisEvent * hce ){
  if (collectionID<0) collectionID = GetCollectionID(0);
  fHits = new PhotonCathodeHC( fName, collectionName[0] );
  hce->AddHitsCollection( collectionID, fHits );
}

void CathodeSD::EndOfEvent( G4HCofThisEvent *  ){
  static long counter=0;

  ++counter;
  if (counter % 100 == 0 ) std::cout<<"CathodeSD::EndOfEvent called "<<counter<<" times"<<std::endl;
  //G4SDManager::GetSDMpointer()->ListTree();
}

