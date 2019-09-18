
#include <Randomize.hh>
#include <G4String.hh>
#include <G4HCtable.hh>
#include <G4SDManager.hh>
#include <G4Event.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>

#include "PTFDigitizeEvent.hh"
#include "HistoManager.hh"
#include "PTFTTree.hh"
#include "PhotonCathodeHit.hh"
#include "CathodeSD.hh"

#include <TGraph.h>

/// functional form of cathode quantum efficiency as function of energy in eV
double get_pmt_qe( double ene ){
  static bool first = true;
  static TGraph* tg = nullptr;
  if ( first ){
    first = false;
    
    /// data below taken by reading by eye from:
    /// X-C Lei, et. al. "Evaluation of new large area PMT with high
    /// quantum efficiency," Chinese Physics C 40 026002 (2016).
    double lambdas[38] = {
      0.,
      300.,   310.,  320.,  330.,  340.,  350.,  360.,  370.,  380.,  390.,
      400.,   410.,  420.,  430.,  440.,  450.,  460.,  470.,  480.,  490.,
      500.,   510.,  520.,  530.,  540.,  550.,  560.,  570.,  580.,  590.,
      600.,   610.,  620.,  630.,  640.,  650., 1000. };

    double qes[38] = {
      0.,
      0.005, 0.020, 0.085, 0.170, 0.220, 0.250, 0.265, 0.280, 0.285, 0.300,
      0.305, 0.300, 0.295, 0.290, 0.280, 0.265, 0.240, 0.230, 0.210, 0.190,
      0.180, 0.170, 0.140, 0.110, 0.080, 0.065, 0.050, 0.040, 0.025, 0.020,
      0.015, 0.013, 0.010, 0.005, 0.002, 0.000, 0.000 };
    tg = new TGraph( 38, lambdas, qes );
  }
  double lambda = 1239.8 / ene;
  return tg->Eval( lambda );
}

/// Function to apply qe for a given event
/// Below constant should be moved to macro file
const double pmt_qe_factor = 1.0; // multiply qe by 2?
bool apply_pmt_qe( double ene ){
  if ( G4UniformRand() < get_pmt_qe( ene) * pmt_qe_factor ) {
    return true;
  }
  return false;
}


PTFDigitizeEvent::PTFDigitizeEvent() : G4UserEventAction() {
  // std::cout<<"PTFDigitizeEvent -- Initialization underway"<<std::endl;
  HistoManager* hman = HistoManager::GetManager();
  hman->BookHistograms(0);
}

PTFDigitizeEvent::~PTFDigitizeEvent() {

}

void PTFDigitizeEvent::BeginOfEventAction( const G4Event* evt ){
  //std::cout<<"PTFDigitizeEvent -- BeginOfEventAction"<<std::endl;
  HistoManager* hman = HistoManager::GetManager();
  unsigned evid = evt->GetEventID();
  hman->ptf->Init( evid );
  hman->truth->Init( evid );

}

void PTFDigitizeEvent::EndOfEventAction( const G4Event* evt ){
  DigitizeEvent( evt );
  FillTruthTracks( evt );
}


void PTFDigitizeEvent::FillTruthTracks( const G4Event * anEvent ){ 
  HistoManager* hman = HistoManager::GetManager(); 

  // loop over vertices
  for (int ivert = 0 ; ivert < anEvent->GetNumberOfPrimaryVertex(); ++ivert ){
    // get vertex:
    const G4PrimaryVertex * vertex = anEvent->GetPrimaryVertex( ivert );
    // loop over tracks in vertex
    for ( int itrack=0; itrack < vertex->GetNumberOfParticle(); ++itrack ) {
      // get track:
      if ( hman->truth->NTracks + 1 < MAXTRACK ){	
	const G4PrimaryParticle * track = vertex->GetPrimary( itrack );
	hman->truth->trackid[ hman->truth->NTracks ] = track->GetTrackID();
 	hman->truth->PDG[ hman->truth->NTracks ]     = track->GetPDGcode();
	// should I add a check and exclude any PDG?
	G4ThreeVector P = track->GetMomentum();
      	hman->truth->Px[ hman->truth->NTracks ] = P.x() / CLHEP::GeV;
      	hman->truth->Py[ hman->truth->NTracks ] = P.y() / CLHEP::GeV;
      	hman->truth->Pz[ hman->truth->NTracks ] = P.z() / CLHEP::GeV;
	hman->truth->VtxX[ hman->truth->NTracks ] = vertex->GetX0() / CLHEP::cm;
	hman->truth->VtxY[ hman->truth->NTracks ] = vertex->GetY0()/ CLHEP::cm;
	hman->truth->VtxZ[ hman->truth->NTracks ] = vertex->GetZ0()/ CLHEP::cm;
	hman->truth->T[ hman->truth->NTracks ] = vertex->GetT0()/ CLHEP::ns;
	++hman->truth->NTracks;
      }
    }
  }

  //std::cout<<"G4ArichEventAction::FillTree -- "
  //	   <<" NTracks = "<<hman->truth->NTracks
  //	   <<std::endl;

  hman->truth_tree->Fill();
}



void PTFDigitizeEvent::DigitizeEvent( const G4Event* evt ){

  // std::cout<<"PTFDigitizeEvent -- EndOfEventAction"<<std::endl;
  HistoManager* hman = HistoManager::GetManager();

  G4HCofThisEvent * HCE = evt->GetHCofThisEvent(); /// all hit collections
  if ( !HCE ){
    std::cerr<<"No hits collection in event!"<<std::endl;
    return;
  }

  /// Get table of hit collections
  G4HCtable * hctable = G4SDManager::GetSDMpointer()->GetHCtable();
  
  /// Loop over hit collections
  for ( unsigned ihc = 0; ihc < hctable->entries(); ++ihc ){
    //std::cout<<"PTFDigitizeEvent -- HitCollection ihc="<<ihc<<std::endl;
    G4String hcname = hctable->GetHCname( ihc );
    G4String sdname = hctable->GetSDname( ihc );
    int collectionid = hctable->GetCollectionID( hcname ); 
    PhotonCathodeHC * fHits = static_cast< PhotonCathodeHC* >( HCE->GetHC( collectionid ) );

    G4VSensitiveDetector* sdptr = G4SDManager::GetSDMpointer()->FindSensitiveDetector( sdname );
    unsigned curpmtid = -1;
    if ( !sdptr ){
      std::cerr<<"Failed to find sensitive detector, setting pmtid=-1"<<std::endl;
    } else {
      CathodeSD * csd = static_cast< CathodeSD* >( sdptr );
      curpmtid = csd->get_pmtid();
    }

    // count number of detected photons hitting each PMT
    // take time of earliest photon as time?
    int pmthitcount=0;
    double pmttime=-9999.0;
    if ( fHits ){
      // process the hits on this PMT, start by filling truth info
      hman->ptf->NPhotons += fHits->entries();
      for ( G4int iphoton = 0; iphoton < fHits->entries(); ++iphoton ){
	//	std::cout<<"PTFDigitizeEvent -- iphoton "<<iphoton<<std::endl;
	PhotonCathodeHit * hit = fHits->operator[](iphoton);
	hman->ptf->true_pmtid[iphoton] = curpmtid;
	hman->ptf->true_used[iphoton] = apply_pmt_qe( hit->ene() );
	hman->ptf->true_x[iphoton]    = hit->pos().x();
	hman->ptf->true_y[iphoton]    = hit->pos().y();
	hman->ptf->true_z[iphoton]    = hit->pos().z();
	hman->ptf->true_dx[iphoton]   = hit->dir().x();
	hman->ptf->true_dy[iphoton]   = hit->dir().y();
	hman->ptf->true_dz[iphoton]   = hit->dir().z();
	hman->ptf->true_t[iphoton]    = hit->time();
	hman->ptf->true_e[iphoton]    = hit->ene();
	hman->ptf->true_ini_z[iphoton]= hit->inipos().z();

	if ( hman->ptf->true_used[iphoton] == true ){
	  if ( hit->time() < pmttime || pmttime == -9999.0 ) {
	    pmttime = hit->time();
	  }
	  if ( pmthitcount == 0 ) hman->ptf->NPMT++;
	  ++pmthitcount;
	} 
      }
      hman->ptf->pmtid[ihc] = curpmtid;
      hman->ptf->dig_T[ihc] = pmttime;
      hman->ptf->dig_Q[ihc] = pmthitcount; 
    }
    
  }

  //  std::cout<<"PTFDigitizeEvent::FillTree -- "
  // 	   <<" Nphotons = "<<hman->ptf->NPhotons
  // 	   <<" NPMT = "<<hman->ptf->NPMT
  // 	   <<std::endl;
  
  hman->ptf_tree->Fill();
  return;
}

