// Modified from OpNovice

#include <TChain.h>
#include <fstream>
#include <stdexcept>

#include "G4ArichPrimaryGeneratorAction.hh"
#include "G4ArichPrimaryGeneratorMessenger.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "HistoManager.hh"
#include "PTFTTree.hh"


using std::ifstream;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichPrimaryGeneratorAction::G4ArichPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), 
    fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fParticleGPS = new G4GeneralParticleSource();

  fSourceType = "gun";

  //create a messenger for this class
  fGunMessenger = new G4ArichPrimaryGeneratorMessenger(this);

  //default kinematic
  //
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("e+");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->SetParticleEnergy(500.0*keV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TChain* ReadListFile(string list, RecEvent& event){
        G4cout<<"ReadListFile "<<list<<G4endl;
	event.pid = NULL;
	event.xSim = NULL;
	event.ySim = NULL;
	event.txSim = NULL;
	event.tySim = NULL;
	event.qdpSim = NULL;
	event.xRec = NULL;
	event.yRec = NULL;
	event.txRec = NULL;
	event.tyRec = NULL;
	event.qdpRec = NULL;
	event.covMat = NULL;
	
	TChain *chain = new TChain("recTracks");
	
	ifstream listFile (list);

	if (listFile.is_open())
	{
    
		string line;
		while ( getline (listFile,line) )
		{
	 	        G4cout<<"Adding file "<<line.c_str()
			      <<" to list of root vectors to read"<<G4endl; 
			chain->Add(line.c_str());
		}    
		listFile.close();
	}
	else {
		G4cerr << "Unable to open file" << G4endl;	
		exit(100);
	}

	chain->SetBranchAddress("eventID", &(event.eventID));
	chain->SetBranchAddress("pid", &(event.pid));
	chain->SetBranchAddress("xSim", &(event.xSim));
	chain->SetBranchAddress("ySim", &(event.ySim));
	chain->SetBranchAddress("txSim", &(event.txSim));
	chain->SetBranchAddress("tySim", &(event.tySim));
	chain->SetBranchAddress("qdpSim", &(event.qdpSim));
	chain->SetBranchAddress("xRec", &(event.xRec));
	chain->SetBranchAddress("yRec", &(event.yRec));
	chain->SetBranchAddress("txRec", &(event.txRec));
	chain->SetBranchAddress("tyRec", &(event.tyRec));
	chain->SetBranchAddress("qdpRec", &(event.qdpRec));
	chain->SetBranchAddress("covMat", &(event.covMat));

	G4cout<<"Done ReadListFile."<<G4endl;
	return chain;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4ArichPrimaryGeneratorAction::InitializeRootVectors(std::string listfile){
  G4cout<<"G4ArichPrimaryGeneratorAction::InitializeRootVectors( "<<listfile<<")"<<G4endl;
  fChain = ReadListFile( listfile, fRecEvent );
  unsigned long long n = fChain->GetEntries();
  G4cout<<"Chain has"<<n<<" entries"<<G4endl;

  try {
    fIndices.reserve( n );
  }
  catch ( const std::length_error& le) {
    G4cerr << "Length error: " << le.what() <<" Error reserving space for indices"<<G4endl;
    exit(0);
  }

  for (unsigned long long i=0; i<n; ++i){
    if (i%100==0) G4cout<<"Adding index "<<i<<" of "<<n<<G4endl;
    fIndices.push_back( i );
  }
  G4cout<<"InitializeRootVectors Done."<<G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
unsigned long long G4ArichPrimaryGeneratorAction::ReadRandomRootVector(bool reuse_events){
  //G4cout<<"G4ArichPrimaryGeneratorAction::ReadRandomRootVector("<<reuse_events<<")"<<G4endl;
   if ( fIndices.size() == 0 ) return -1;

  unsigned long long index = G4UniformRand() * fIndices.size();  
  //G4cout<<"Reading entrty "<<index<<" of "<<fIndices.size()<<G4endl;
  fChain->GetEntry( fIndices[index] );
  if ( !reuse_events ) {
    //G4cout<<"Erasing index "<<index<<" of "<<fIndices.size()<<G4endl;
      fIndices.erase( fIndices.begin()+index );
  }
  return index;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ArichPrimaryGeneratorAction::~G4ArichPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
  delete fParticleGPS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ArichPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  static bool first = true;

  if ( fSourceType == "gun" ){
    //std::cout<<"G4ArichPrimaryGeneratorAction: using gun source"<<std::endl;
    fParticleGun->GeneratePrimaryVertex(anEvent);
  } else if ( fSourceType == "gps" ) {
    //std::cout<<"G4ArichPrimaryGeneratorAction: using gps source"<<std::endl;
    fParticleGPS->GeneratePrimaryVertex(anEvent);
  } else if ( fSourceType == "root" ) {
    //std::cout<<"G4ArichPrimaryGeneratorAction: using root source"<<std::endl;
    if ( first ) {
      first = false;
      G4cout<<"About to Initialize Root Vectors()"<<G4endl;
      InitializeRootVectors();
    }

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    
    unsigned long long index = ReadRandomRootVector();
    unsigned long long bigulong = -1;
    if ( index == bigulong ){
      G4cout << "--> warning from PrimaryGeneratorAction::ReadRandomRootVector :"
	" ran out of events in root files... not generating any vertices " << G4endl;
    } else {  
      
      //G4cout<<"Try to generate "<<fRecEvent.pid->size()<<" particles for event"<<G4endl;
      int iparmax=0;
      double pmax=0.0;
      for ( int ipar = 0; ipar < fRecEvent.pid->size() ; ++ipar ){
	if ( fRecEvent.qdpSim->at(ipar) != 0.0 &&
	     1.0 / fabs( fRecEvent.qdpSim->at(ipar) ) > pmax ) {
	  pmax = 1.0/fabs(fRecEvent.qdpSim->at(ipar));
	  iparmax = ipar;
	}
      }
      for ( int ipar = 0; ipar < fRecEvent.pid->size() ; ++ipar ){
	//G4cout<<"Looking for PID " << fRecEvent.pid->at(ipar) << G4endl;
        G4ParticleDefinition* Particle = particleTable->FindParticle( fRecEvent.pid->at(ipar) );
	if ( Particle == nullptr ){
	  G4cout<<"Skip particle not found pid="<<fRecEvent.pid->at(ipar)<<G4endl;
	  continue;
	}

        fParticleGun->SetParticleDefinition( Particle );
	double ptot = 0.0;
	if ( fRecEvent.qdpSim->at(ipar) != 0.0 ){
	  ptot = 1.0/ fabs( fRecEvent.qdpSim->at(ipar) ); //momentum in GeV/c
	}
	fParticleGun->SetParticleMomentum( ptot * CLHEP::GeV );
	
	// momentum direction
	double xz = fRecEvent.txSim->at(ipar);
	double yz = fRecEvent.tySim->at(ipar);
	double zz = sqrt( 1.0 / ( 1.0 + xz*xz + yz*yz ) );
	double xx = xz * zz;
	double yy = yz * zz;
	fParticleGun->SetParticleMomentumDirection( G4ParticleMomentum( xx, yy, zz ) );
	
	// position
	double x = fRecEvent.xSim->at(ipar);
	double y = fRecEvent.ySim->at(ipar);
	double z = 0.0;
	fParticleGun->SetParticlePosition( G4ThreeVector(x*cm,y*cm,z*cm) );	
	
	// generate "vertex"
	//G4cout<<"GeneratePrimaryVertex"<<G4endl;
	fParticleGun->GeneratePrimaryVertex(anEvent); 
      } 
      
      // now save info about track in root ttree output
      //G4cout<<"GetHisto Manager to save entry "<<index
      //	    <<" with eventID "<<fRecEvent.eventID<<G4endl;
      HistoManager* hman = HistoManager::GetManager(); 
      hman->ptf->root_eventid = fRecEvent.eventID;
      hman->ptf->root_idx = index;
    }
    //G4cout<<"Done Generating Primary Vertices for event"<<G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ArichPrimaryGeneratorAction::SetOptPhotonPolar()
{
 G4double angle = G4UniformRand() * 360.0*deg;
 SetOptPhotonPolar(angle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ArichPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
 if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
   {
     G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
               "the particleGun is not an opticalphoton" << G4endl;
     return;
   }

 G4ThreeVector normal (1., 0., 0.);
 G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
 G4ThreeVector product = normal.cross(kphoton);
 G4double modul2       = product*product;
 
 G4ThreeVector e_perpend (0., 0., 1.);
 if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
 G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
 
 G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
 fParticleGun->SetParticlePolarization(polar);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
