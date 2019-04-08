/// PTFDigitizeEvent
/// Simple G4UserEventAction to collect photon hits into root output tree
/// and apply a simple QE + Digitization
/// Author: Blair Jamieson (July 2018)

#ifndef _PTFDigitizeEvent_hh_
#define _PTFDigitizeEvent_hh_

#include <G4UserEventAction.hh>
#include <G4Event.hh>

class PTFDigitizeEvent : public G4UserEventAction {
public:
  PTFDigitizeEvent(); // construct output tree and file
  ~PTFDigitizeEvent(); // write file
  void BeginOfEventAction( const G4Event* evt );
  void EndOfEventAction( const G4Event* evt );


private:
  void FillTruthTracks( const G4Event* anEvent );
  void DigitizeEvent( const G4Event* evt );


};



#endif
