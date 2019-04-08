/* PhotonCathodeHit
   A G4VHit to store when a photon has been absorbed in
   a PMT cathode.  It records position on the cathode and
   direction relative to cathode normal.

   Author: Blair Jamieson (May 2018)
*/

#ifndef _PhotonCathodeHit_h_
#define _PhotonCathodeHit_h_ 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class PhotonCathodeHit : public G4VHit {
public:
  PhotonCathodeHit( const G4ThreeVector& pos,  // position relative to center of pmt
		    const G4ThreeVector& dir,  // direction relative to PMT facing
		    double time,               // photon's time (ns)
		    double energy,             // photon energy (eV)
		    const G4ThreeVector& inipos, // location photon was produced
		    unsigned PMTnum=0 );       // PMT number
  virtual ~PhotonCathodeHit(){ ; }

  virtual void Draw(){ ; }// std::cout<<"PhotonCathodeHit::Draw() not implemented"<<std::endl;}
  virtual void Print(){ ; }// std::cout<<"PhotonCathodeHit::Print() not implemented"<<std::endl;}
  
  G4ThreeVector pos()   const { return fPos;    }
  G4ThreeVector dir()   const { return fDir;    }
  double        time()  const { return fTime;   }
  double        ene()   const { return fEne;    }
  G4ThreeVector inipos()   const { return fIniPos;    }
  unsigned      pmtid() const { return fPMTNum; }
  
private:
  
  G4ThreeVector fPos;
  G4ThreeVector fDir;
  double        fTime;
  double        fEne;
  G4ThreeVector fIniPos;
  unsigned      fPMTNum;
};

typedef G4THitsCollection< PhotonCathodeHit > PhotonCathodeHC;

#endif
