
#include "H12700Geometry.hh"
#include "MaterialsMap.hh"
#include "OpticalSurfaceDefinitions.hh"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UserLimits.hh"

#include "CathodeSD.hh"
#include "G4SDManager.hh"


H12700Geometry::H12700Geometry( G4LogicalVolume * parentvol,
				const G4ThreeVector & loc,
				const G4ThreeVector & dir, 
				double size ):
		
  fParent(parentvol), fPMTId(0), fLoc(loc), fDir(dir.unit()), fSize(size), fSD(nullptr)
{
  static int pmtcount=0;
  ++pmtcount; fPMTId = pmtcount;
  Init();
  std::cout<<"H12700 finished being built"<<std::endl;
}

H12700Geometry::~H12700Geometry(){

  if ( fRm != nullptr ) delete fRm;
  for ( auto & curpair : fVols ){
    G4VPhysicalVolume * curphysvol = curpair.second;
    delete curphysvol;
  }
}
  
void H12700Geometry::attachSD( int pmtID ){
  // Add sensitive detector to Cathode
  G4VPhysicalVolume * cath_phys = fVols[ "cathode" ];
  G4LogicalVolume * cath_log = cath_phys->GetLogicalVolume();
  std::cout<<"Building sensitive detector with name "<<cath_phys->GetName()<<std::endl;
  fSD = new CathodeSD( pmtID, cath_phys->GetName() );
  cath_log->SetSensitiveDetector( fSD );

  G4SDManager::GetSDMpointer()->ListTree();
}
				   
				   

void H12700Geometry::Init( ){
  std::ostringstream os;
 
  //std::cout<<"H12700Geom:: get materials map"<<std::endl;
  MaterialsMap & fMaterials = MaterialsMap::Get();
  //std::cout<<"H12700Geom:: got materials map"<<std::endl;
  OpticalSurfaceDefinitions & fSurfaces = OpticalSurfaceDefinitions::Get();
  //std::cout<<"H12700Geom:: got surfaces map"<<std::endl; 


  // build rotation matrix
  fRm = new G4RotationMatrix;
  if ( fDir == G4ThreeVector(0.,0.,-1. ) ){
    fRm->rotateX( 180.0*CLHEP::deg );
  } else if ( fDir != G4ThreeVector(0.,0.,1. ) ){
    double norm= std::sqrt( fDir.x()*fDir.x() + fDir.y()*fDir.y() );
    G4ThreeVector nxy( fDir.y()/norm, -fDir.x()/norm, 0 );
    double rotang = std::acos( fDir.z() );
    fRm->rotate( rotang, nxy );
  }

  // PMT Glass Logical Volume.
  os << "H12700_frontglass_" << fPMTId;
  G4Box* front_glass = new G4Box( os.str().c_str(), fSize, fSize, 1.5 * CLHEP::mm);

  G4LogicalVolume * front_glass_log = new G4LogicalVolume( front_glass, fMaterials["pmtglass"], os.str().c_str() );
  
  // Place the glass
  // include the -210mm offset to put the top of the front glass at 0, instead of center of PMT dome
  G4ThreeVector offset = fLoc + G4ThreeVector( 0., 0., 1.5*CLHEP::mm );

  //std::cout<<"Building pmtglass"<<std::endl;
  fVols[ "pmtglass" ] = new G4PVPlacement( fRm, offset, front_glass_log, os.str().c_str(), fParent, false, 0 );
  fSurfaces.skin_surface( "pmtglass", fVols[ "pmtglass" ] );

  //std::cout<<"Built pmtglass"<<std::endl;
  
  // PMT Cathode (the sensitive detector)
  G4Box* cathode_box = new G4Box( os.str().c_str(), fSize, fSize, 0.25 * CLHEP::mm);
  reset(os);
  os << "H12700_cathode_" << fPMTId; 
  G4LogicalVolume * cathode_box_log = new G4LogicalVolume( cathode_box, fMaterials["cathode"], os.str().c_str() );
  cathode_box_log->SetVisAttributes( G4VisAttributes( G4Color::Red() ) );
  //std::cout<<"Building cathode"<<std::endl;

  fVols[ "cathode" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,1.25 * CLHEP::mm), cathode_box_log, os.str().c_str(), front_glass_log, false, 0 );
  std::cout<<"Built cathode"<<std::endl;

  fSurfaces.skin_surface( "cathode", fVols[ "cathode" ] );
  // set step limit in cathode
  G4UserLimits * cathode_steplimit = new G4UserLimits( 0.125  *CLHEP::mm );
  cathode_box_log->SetUserLimits( cathode_steplimit );
  //std::cout<<"Built H12700"<<std::endl;
    
}


