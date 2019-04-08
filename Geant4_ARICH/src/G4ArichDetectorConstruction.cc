

//#include "G4ArichDetectorMessenger.hh"
#include "G4ArichDetectorConstruction.hh"


#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "MaterialsMap.hh"
#include "OpticalSurfaceDefinitions.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichDetectorConstruction::G4ArichDetectorConstruction()
 : G4VUserDetectorConstruction()
{
  // set defaults
  fExpHall_x = fExpHall_y = fExpHall_z = 20.0*CLHEP::m;
  //fDetMessenger = new G4ArichDetectorMessenger( this );
  hasmirror = true;
  mirrorlength = 40.0*CLHEP::mm;
  expHall_log=nullptr;
  expHall_phys=nullptr;

  ReadGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ArichDetectorConstruction::~G4ArichDetectorConstruction(){
  for ( H12700Geometry * fgeom : fPMTs ){
    if ( fgeom!=nullptr ) delete fgeom;
  }
}


void G4ArichDetectorConstruction::ConstructSDandField(){
  //
  //std::cout<<"fPMT->fSD = "<<fPMT->fSD<<std::endl;
  unsigned ipmt=0;
  for ( H12700Geometry * fgeom : fPMTs ){
    fgeom->attachSD( ipmt ); //PMT ipmt!
    ++ipmt;
  }
  G4SDManager::GetSDMpointer()->ListTree();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* G4ArichDetectorConstruction::Construct()
{
  std::cout<<"G4ArichDetectorConstruction::Construct()"<<std::endl;
  

  MaterialsMap & fMaterials = MaterialsMap::Get();
  OpticalSurfaceDefinitions & fSurfaces = OpticalSurfaceDefinitions::Get();
  
  std::cout<<"Got materials map"<<std::endl;
  // Air
  //
  G4Material* air = fMaterials[ "air"];

  // The experimental Hall
  // only build it once.
  if ( expHall_log == nullptr ){
    G4Box* expHall_box = new G4Box("World",fExpHall_x,fExpHall_y,fExpHall_z);
    
    expHall_log = new G4LogicalVolume(expHall_box,air,"World",0,0,0);
    
    expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);
    
  }

  // Black water (or air) tank
  //  G4Material* black_plastic = fMaterials[ "blacksheet" ];
  
  //
  // ------------- Volumes --------------

  // The Aerogel pieces
  G4Box* front_ag1 = new G4Box("front_ag1", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* front_ag2 = new G4Box("front_ag2", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* front_ag3 = new G4Box("front_ag3", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* front_ag4 = new G4Box("front_ag4", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* back_ag1 = new G4Box("back_ag1", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* back_ag2 = new G4Box("back_ag2", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* back_ag3 = new G4Box("back_ag3", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  G4Box* back_ag4 = new G4Box("back_ag4", 10.0 * CLHEP::cm, 10.0 * CLHEP::cm, 1.0 * CLHEP::cm);
  
  G4LogicalVolume* front_ag1_log = new G4LogicalVolume(front_ag1, fMaterials["aerogel103"],"front_ag1_log",0,0,0);
  G4LogicalVolume* front_ag2_log = new G4LogicalVolume(front_ag2, fMaterials["aerogel103"],"front_ag2_log",0,0,0);
  G4LogicalVolume* front_ag3_log = new G4LogicalVolume(front_ag3, fMaterials["aerogel103"],"front_ag3_log",0,0,0);
  G4LogicalVolume* front_ag4_log = new G4LogicalVolume(front_ag4, fMaterials["aerogel103"],"front_ag4_log",0,0,0);
 
  G4LogicalVolume* back_ag1_log = new G4LogicalVolume(back_ag1, fMaterials["aerogel104"],"back_ag1_log",0,0,0);
  G4LogicalVolume* back_ag2_log = new G4LogicalVolume(back_ag2, fMaterials["aerogel104"],"back_ag2_log",0,0,0);
  G4LogicalVolume* back_ag3_log = new G4LogicalVolume(back_ag3, fMaterials["aerogel104"],"back_ag3_log",0,0,0);
  G4LogicalVolume* back_ag4_log = new G4LogicalVolume(back_ag4, fMaterials["aerogel104"],"back_ag4_log",0,0,0);

  front_ag[0] = new G4PVPlacement(0,G4ThreeVector( 10.025*CLHEP::cm, 10.025*CLHEP::cm,1.*CLHEP::cm), front_ag1_log,"front_ag1", expHall_log,false,0);
  front_ag[1] = new G4PVPlacement(0,G4ThreeVector(-10.025*CLHEP::cm, 10.025*CLHEP::cm,1.*CLHEP::cm), front_ag2_log,"front_ag2", expHall_log,false,0);
  front_ag[2] = new G4PVPlacement(0,G4ThreeVector( 10.025*CLHEP::cm,-10.025*CLHEP::cm,1.*CLHEP::cm), front_ag3_log,"front_ag3", expHall_log,false,0);
  front_ag[3] = new G4PVPlacement(0,G4ThreeVector(-10.025*CLHEP::cm,-10.025*CLHEP::cm,1.*CLHEP::cm), front_ag4_log,"front_ag4", expHall_log,false,0);

  front_ag1_log->SetVisAttributes( G4VisAttributes( G4Color::Cyan() ) );
  front_ag2_log->SetVisAttributes( G4VisAttributes( G4Color::Cyan() ) );
  front_ag3_log->SetVisAttributes( G4VisAttributes( G4Color::Cyan() ) );
  front_ag4_log->SetVisAttributes( G4VisAttributes( G4Color::Cyan() ) );

  back_ag[0] = new G4PVPlacement(0,G4ThreeVector( 10.025*CLHEP::cm, 10.025*CLHEP::cm,3.05*CLHEP::cm), back_ag1_log,"back_ag1", expHall_log,false,0);
  back_ag[1] = new G4PVPlacement(0,G4ThreeVector(-10.025*CLHEP::cm, 10.025*CLHEP::cm,3.05*CLHEP::cm), back_ag2_log,"back_ag2", expHall_log,false,0);
  back_ag[2] = new G4PVPlacement(0,G4ThreeVector( 10.025*CLHEP::cm,-10.025*CLHEP::cm,3.05*CLHEP::cm), back_ag3_log,"back_ag3", expHall_log,false,0);
  back_ag[3] = new G4PVPlacement(0,G4ThreeVector(-10.025*CLHEP::cm,-10.025*CLHEP::cm,3.05*CLHEP::cm), back_ag4_log,"back_ag4", expHall_log,false,0);

  back_ag1_log->SetVisAttributes( G4VisAttributes( G4Color::Blue() ) );
  back_ag2_log->SetVisAttributes( G4VisAttributes( G4Color::Blue() ) );
  back_ag3_log->SetVisAttributes( G4VisAttributes( G4Color::Blue() ) );
  back_ag4_log->SetVisAttributes( G4VisAttributes( G4Color::Blue() ) );

  // Make mirrors
  if ( hasmirror ) { //mirrorlength
    G4Box* mirror_ag1 = new G4Box("mirror_ag1", 20.1 * CLHEP::cm,  1.0 * CLHEP::mm, mirrorlength/2 );   
    G4Box* mirror_ag2 = new G4Box("mirror_ag2", 20.1 * CLHEP::cm,  1.0 * CLHEP::mm, mirrorlength/2 );   
    G4Box* mirror_ag3 = new G4Box("mirror_ag3",  1.0 * CLHEP::mm, 20.1 * CLHEP::cm, mirrorlength/2 );   
    G4Box* mirror_ag4 = new G4Box("mirror_ag4",  1.0 * CLHEP::mm, 20.1 * CLHEP::cm, mirrorlength/2 );   
    G4LogicalVolume* mirror_ag1_log = new G4LogicalVolume(mirror_ag1, fMaterials["Al"],"mirror_ag1_log",0,0,0);
    G4LogicalVolume* mirror_ag2_log = new G4LogicalVolume(mirror_ag2, fMaterials["Al"],"mirror_ag1_log",0,0,0);
    G4LogicalVolume* mirror_ag3_log = new G4LogicalVolume(mirror_ag3, fMaterials["Al"],"mirror_ag1_log",0,0,0);
    G4LogicalVolume* mirror_ag4_log = new G4LogicalVolume(mirror_ag4, fMaterials["Al"],"mirror_ag1_log",0,0,0);

    mirror_ag1_log->SetVisAttributes( G4VisAttributes( G4Color::Gray() ) );
    mirror_ag2_log->SetVisAttributes( G4VisAttributes( G4Color::Gray() ) );
    mirror_ag3_log->SetVisAttributes( G4VisAttributes( G4Color::Gray() ) );
    mirror_ag4_log->SetVisAttributes( G4VisAttributes( G4Color::Gray() ) );
    mirror_ag[0] = new G4PVPlacement(0,G4ThreeVector(  0.0*CLHEP::cm, 20.2*CLHEP::cm, mirrorlength/2 ), mirror_ag1_log,"mirror_ag1", expHall_log,false,0);
    mirror_ag[1] = new G4PVPlacement(0,G4ThreeVector(  0.0*CLHEP::cm,-20.2*CLHEP::cm, mirrorlength/2 ), mirror_ag2_log,"mirror_ag2", expHall_log,false,0);
    mirror_ag[2] = new G4PVPlacement(0,G4ThreeVector( 20.2*CLHEP::cm,  0.0*CLHEP::cm, mirrorlength/2 ), mirror_ag3_log,"mirror_ag3", expHall_log,false,0);
    mirror_ag[3] = new G4PVPlacement(0,G4ThreeVector(-20.2*CLHEP::cm,  0.0*CLHEP::cm, mirrorlength/2 ), mirror_ag4_log,"mirror_ag4", expHall_log,false,0);


    for (unsigned i=0; i<4; ++i ) fSurfaces.skin_surface( "reflector", mirror_ag[i] );
  }

    
  // put PMTs somewhere in geometry
  const double PMTwid     = 52.0 * CLHEP::mm;
  const double PMTsep     = 1.0 * CLHEP::mm;
  const double PMToffsetz = 20.0 * CLHEP::cm; 
  //const int ncol[5] = { 3, 5, 7, 5, 3 }; // number of rows in column
  //int iglobal = 0;
  //for ( int irow = 0; irow < 5; ++irow ){
  //  for ( int icol = 0; icol< ncol[irow]; ++icol ){
  //    if ( irow == 2 && icol == 3 ) continue;
  //    double xpos =  ( -( ncol[irow]/2 )  + icol ) * ( PMTwid + PMTsep );
  //    double ypos =  (2-irow)*(PMTwid+PMTSep);
  //    fPMTs[iglobal] = new H12700Geometry( expHall_log,
  //					   G4ThreeVector( xpos, ypos, PMToffsetz ),
  //					   G4ThreeVector( 0., 0., -1. ) );
  //    ++iglobal;
  //  }
  //}
  // Only place one big panel, and worry about geometry later
  std::cout<<"About to build an H12700"<<std::endl;

  fPMTs.push_back( new H12700Geometry( expHall_log,
				       G4ThreeVector( 0, 0, PMToffsetz ),
				       G4ThreeVector( 0., 0., -1. ), 
				       1.0 *CLHEP::m ) );
  std::cout<<"Built an H12700"<<std::endl;

  return expHall_phys;
}

void apply_unit( double & val, const std::string& unit ){
  if ( unit == "nm" ){
    val *= CLHEP::nm;
  } else if ( unit == "um" ){
    val *= CLHEP::um;
  } else if ( unit == "mm" ){
    val *= CLHEP::mm;
  } else if ( unit == "cm" ){
    val *= CLHEP::cm;
  } else if ( unit == "m" ){
    val *= CLHEP::m;
  } else {
    std::cout<<"apply_unit "<<unit<<", unknown unit, not applying."<<std::endl;
  }
}

void G4ArichDetectorConstruction::ReadGeometry(){

  std::cout<<"G4ArichDetectorConstruction::ReadGeometry() starting "<<std::endl;
  
  std::ifstream infile( "rich-geom.txt" );

  if ( !infile ) {
    std::cout<<"Could not open rich-geom.txt, using default geometry"<<std::endl;
    return;
  }

  std::string line;
  while ( std::getline( infile, line ) ){
    std::cout<<"Processing line: "<<line<<std::endl;
    std::istringstream is( line );
    std::string tag;

    is >> tag;
    if ( tag == "#" ){
      continue;
    } else if ( tag == "hasmirror" ) {
      std::string mybool;
      is >> mybool;
      if ( mybool == "true" ) hasmirror = true;
      else if ( mybool == "false" ) hasmirror = false;
      else if ( mybool == "0" ) hasmirror = false;
      else hasmirror = true;
      std::cout<<"G4ArichDetectorConstruction::ReadGeometry hasmirror = "<<hasmirror<<std::endl;
      
    } else if ( tag == "mirrorlength" ){
      std::string unit;
      is >> mirrorlength >> unit;
      std::cout<<"mirrorlength ="<<mirrorlength<<" unit="<<unit<<std::endl;
      apply_unit( mirrorlength, unit );
      std::cout<<"G4ArichDetectorConstruction::ReadGeometry mirrorlength = "<<mirrorlength/CLHEP::mm<<" mm"<<std::endl;

    } 
  }
  std::cout<<"G4ArichDetectorConstruction::ReadGeometry() ending "<<std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
