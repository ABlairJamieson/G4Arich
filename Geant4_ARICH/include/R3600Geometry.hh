#ifndef _R3600Geometry_hh_
#define _R3600Geometry_hh_

#include <string>
#include <sstream>
#include <map>
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "CathodeSD.hh"
#include "G4SDManager.hh"

/// R3600Geometry
/// Class to construct Geant4 goemtry of a Hammamatsu R3600 PMT
/// Geometry is not exact, but is a close approximation.
/// Author: Blair Jamieson    Date: April 2018
class R3600Geometry {
public:

  /// Specify location of center of front
  R3600Geometry(
		 G4LogicalVolume * parentvol,    //< Parent volume in which to place this PMT
		 const G4ThreeVector & loc,     //< loc : location of the center of the front glass of the PMT	  
		 const G4ThreeVector & dir, 	//< dir : normal to the center of the front glass of the PMT		  
		 bool add_acrylic = true,       //< acrylic : set true to add the acrylic and FRP cover to the geometry 
		 double acryl_thick = 13.0 * CLHEP::mm,
		 double glass_thick =  4.0 * CLHEP::mm,
		 double cath_thick  = 30.0 * CLHEP::nm );	
  ~R3600Geometry();

  /// Get the PMT geometry with name volname
  G4VPhysicalVolume *  GetPhysicalVolume( std::string& volname ) { return fVols[volname] ; }

  /// Get methods
  int                  GetPMTId()    const { return fPMTId; }
  const G4ThreeVector& get_loc()     const { return fLoc ; }
  const G4ThreeVector& get_dir()     const { return fDir ; }
  bool                 WithAcrylic() const { return fWithAcrylic; }

  /// Method to get overall region, plus the rotation and translation
  /// To subtract from other volumes.
  //void get_solid( G4VSolid & vol, G4RotationMatrix& rm, G4ThreeVector& loc ); 


  /// Attach sensitive detector
  void attachSD( int pmtID );
    
  /// Method to disable sensitive detector of this PMT
  //void disableSD( bool disable=true ){
  //  G4String sdname = fSD->get_name(); 
  //  G4SDManager::GetSDMpointer()->Activate( sdname, !disable );
    // delete fSD;
  //}

  /// Method to remove physical volumes from parent
  ///
  //void Disable(){
  //  fParent->RemoveDaughter( fVols[ "pmtglass" ] );
  //  delete fVols[ "pmtglass"];
  //  if ( fWithAcrylic ) {
  //    fParent->RemoveDaughter( fVols[ "acrylic_dome"] );
  //    fParent->RemoveDaughter( fVols[ "frp_cover"] );
  //    delete fVols["acrylic_dome"];
  //    delete fVols["frp_cover"];
  //  }
    //if ( fSD != nullptr ) disableSD();
  // }
  
private:
  void Init();
  void BuildAcrylic();
  std::ostringstream& reset( std::ostringstream& os ){ os.clear(); os.str(""); return os; }
  
  /// From inputs:
  G4LogicalVolume * fParent;   //< keep pointer to parent volume, but not deleted
  int fPMTId;
  G4ThreeVector fLoc;
  G4ThreeVector fDir;
  bool fWithAcrylic;

  double acrylic_thickness; //< default 13 mm
  double glass_thickness;   //< default  4 mm
  double cathode_thickness; //< default 30 nm
  
  /// Built by initialization
  G4RotationMatrix *fRm;

  /// map of constructed physical volumes
  /// "top" == the top glass
  /// "bottop" == the bottom part of the top glass
  /// "cathode" == the cathode
  /// "side" == the side glass
  /// "bottom" == the bottom curved in section of glass
  /// "pins" == the glass holding the pins
  /// ...
  std::map< std::string, G4VPhysicalVolume * > fVols;

  /// overall volume to use in subtraction from wherever this PMT is placed
  //G4VSolid * fSumSolid;

  /// Sensitive detector
  CathodeSD * fSD;
};































#endif
