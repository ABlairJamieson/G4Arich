#ifndef _H12700Geometry_hh_
#define _H12700Geometry_hh_

#include <string>
#include <sstream>
#include <map>
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "CathodeSD.hh"
#include "G4SDManager.hh"

/// H12700Geometry
/// Class to construct Geant4 geomtry of a photon detector panel,
/// with square cross-section of size passed.  PMT quantum efficiency
/// applied is that of Hammamatsu H12700 PMT.
/// Geometry is square of glass with sensitive detector behind.
/// Author: Blair Jamieson, Andrew Sikora    Date: Oct 2018
class H12700Geometry {
public:

  /// Specify location of center of front
  H12700Geometry(
		 G4LogicalVolume * parentvol,    //< Parent volume in which to place this PMT
		 const G4ThreeVector & loc,     //< loc : location of the center of the front glass of the PMT	  
		 const G4ThreeVector & dir,  	//< dir : normal to the center of the front glass of the PMT		  
		 const double size );  	//< size : half-width of the detector panel

  ~H12700Geometry();

  /// Get the PMT geometry with name volname
  G4VPhysicalVolume *  GetPhysicalVolume( std::string& volname ) { return fVols[volname] ; }

  /// Get methods
  int                  GetPMTId()    const { return fPMTId; }
  const G4ThreeVector& get_loc()     const { return fLoc ; }
  const G4ThreeVector& get_dir()     const { return fDir ; }

  /// Attach sensitive detector
  void attachSD( int pmtID );
    
  
private:
  void Init();

  std::ostringstream& reset( std::ostringstream& os ){ os.clear(); os.str(""); return os; }
  
  /// From inputs:
  G4LogicalVolume * fParent;   //< keep pointer to parent volume, but not deleted
  int fPMTId;
  G4ThreeVector fLoc;
  G4ThreeVector fDir;
  double        fSize;  // half-width of PMT face


  /// Built by initialization
  G4RotationMatrix *fRm;

  /// map of constructed physical volumes
  std::map< std::string, G4VPhysicalVolume * > fVols;

  /// Sensitive detector
  CathodeSD * fSD;
};































#endif
