
#include "PhotonCathodeHit.hh"

PhotonCathodeHit::PhotonCathodeHit( const G4ThreeVector& pos,  
				    const G4ThreeVector& dir,  
				    double time,               
				    double energy,           
				    const G4ThreeVector& inipos,  
				    unsigned PMTnum ) 
  : G4VHit(), fPos( pos ), fDir( dir ), fTime( time ),
    fEne( energy ), fIniPos( inipos ), fPMTNum( PMTnum )
{

}
