
#include "R3600Geometry.hh"
#include "MaterialsMap.hh"
#include "OpticalSurfaceDefinitions.hh"

#include "G4UnionSolid.hh"
#include "G4Ellipsoid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UserLimits.hh"

#include "CathodeSD.hh"
#include "G4SDManager.hh"


R3600Geometry::R3600Geometry( G4LogicalVolume * parentvol,
				const G4ThreeVector & loc,
				const G4ThreeVector & dir,
				bool add_acrylic,
				double acryl_thick,
				double glass_thick,
				double cath_thick ) :
  fParent(parentvol), fPMTId(0), fLoc(loc), fDir(dir.unit()),  fWithAcrylic(add_acrylic),
  acrylic_thickness( acryl_thick ), glass_thickness( glass_thick ),
  cathode_thickness( cath_thick ), fSD(nullptr)
{
  static int pmtcount=0;
  ++pmtcount; fPMTId = pmtcount;
  Init();
}

R3600Geometry::~R3600Geometry(){

  if ( fRm != nullptr ) delete fRm;
  for ( auto & curpair : fVols ){
    G4VPhysicalVolume * curphysvol = curpair.second;
    delete curphysvol;
  }
  //if ( fSD != nullptr ) delete fSD;
}

  /*
void R3600Geometry::get_solid( G4VSolid & vol, G4RotationMatrix& rm, G4ThreeVector& loc ) {
  std::ostringstream os;
  int volcount=0;

  // check if solid volume was built already?
  if ( fSumSolid == nullptr ){
    //std::pair< std::string,G4VPhysicalVolume * > refpair = *(fVols.begin());
    //std::string refname = refpair.first;
    //G4VPhysicalVolume* refvol = refpair.second;
    G4VPhysicalVolume* refvol = fVols.begin()->second;
    G4VSolid* sumvol = refvol->GetLogicalVolume()->GetSolid();
    G4ThreeVector refloc = refvol->GetTranslation() - fLoc;
    for ( auto & curpair : fVols ){
      if ( refvol == curpair.second ) continue;
      G4VPhysicalVolume * curphysvol = curpair.second;
      G4VSolid* curvol = curphysvol->GetLogicalVolume()->GetSolid();
      G4ThreeVector curloc = curphysvol->GetTranslation() - refloc;
      reset(os);
      os << "R3600Sum_"<< fPMTId <<"_"<<++volcount;
      G4UnionSolid * cursumvol = new G4UnionSolid( os.str().c_str(), sumvol, curvol, 0, curloc );
      sumvol = cursumvol;
    }
    fSumSolid = sumvol;
  }

  vol = *fSumSolid;
  rm = *fRm;
  loc = fLoc;
  return;
}
  */

  
void R3600Geometry::attachSD( int pmtID ){
  // Add sensitive detector to Cathode
  G4VPhysicalVolume * cath_phys = fVols[ "cathode" ];
  G4LogicalVolume * cath_log = cath_phys->GetLogicalVolume();
  //if ( fSD != nullptr ){
  //  std::cout<<"Sensitive detector already exists, attach to "<<cath_phys->GetName()<<std::endl;
  //  cath_log->SetSensitiveDetector( fSD );
  //
  //} else {
  std::cout<<"Building sensitive detector with name "<<cath_phys->GetName()<<std::endl;
  fSD = new CathodeSD( pmtID, cath_phys->GetName() );
  cath_log->SetSensitiveDetector( fSD );

  //}
  G4SDManager::GetSDMpointer()->ListTree();
}
				   
				   

void R3600Geometry::Init( ){
  std::ostringstream os;
 
  MaterialsMap & fMaterials = MaterialsMap::Get();
  OpticalSurfaceDefinitions & fSurfaces = OpticalSurfaceDefinitions::Get();
 
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

  std::cout<<"R3600Geometry::Init glass_thickness="<<glass_thickness/CLHEP::mm<<" mm"
	   <<" cathode_thickness="<<cathode_thickness/CLHEP::nm<<" nm"
	   <<" has acrylic="<< WithAcrylic()<<std::endl;
  
  // constants to put into messenger class eventually:
  const double reflect_thickness = 1000.0 * CLHEP::nm;
  const double glass_stepsize    = glass_thickness / 5.0;
  const double cathode_stepsize  = cathode_thickness / 5.0;
  
  // PMT Glass Logical Volume.
  
  // build top glass
  os << "R3600EGTop_" << fPMTId;
  G4Ellipsoid* egtop = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm, 254.0*CLHEP::mm, 210.0*CLHEP::mm, 0.0*CLHEP::mm, 210.0*CLHEP::mm );
  reset(os);
  os << "R3600EGBotop_" << fPMTId;
  G4Ellipsoid* egbotop = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm, 254.0*CLHEP::mm, 180.0*CLHEP::mm, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  // Union of top and bottom glass
  os << "R3600EGTopBotop_"<< fPMTId;
  G4UnionSolid * egtopbotop = new G4UnionSolid( os.str().c_str(), egtop, egbotop, 0, G4ThreeVector(0.,0.,0.) ); // no offset
  reset(os);
  // Build side of pmt glass
  os <<"R3600TGSide_"<<fPMTId;
  G4Tubs* tgside = new G4Tubs( os.str().c_str(), 0.0, 145.5*CLHEP::mm, 78.5*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // side glass is at -231.5 mm from center of PMT
  // Union of top, bottom of top, and side glass
  reset(os);
  os << "R3600EGThreeParts_"<< fPMTId;
  G4UnionSolid * egthree = new G4UnionSolid( os.str().c_str(), egtopbotop, tgside, 0, G4ThreeVector(0.,0.,-231.5*CLHEP::mm) ); // 
  reset(os);
  os << "R3600EGBot_" << fPMTId;
  G4Ellipsoid* egbot = new G4Ellipsoid( os.str().c_str(), 141.5*CLHEP::mm, 141.5*CLHEP::mm, 90.0*CLHEP::mm, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  // bottom glass is at -310.0 mm from center of PMT
  // Union of top, bottom of top, side, and bottom glass
  reset(os);
  os << "R3600EGFourParts_"<< fPMTId;
  G4UnionSolid * egfour = new G4UnionSolid( os.str().c_str(), egthree, egbot, 0, G4ThreeVector(0.,0.,-310.0*CLHEP::mm) ); //
  // Pins part of glass
  reset(os);
  os <<"R3600TGPins_"<<fPMTId;
  G4Tubs* tgpins = new G4Tubs( os.str().c_str(), 0.0, 49.5*CLHEP::mm, 23.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // pins are offset by -410.9 mm from center of PMT
  // Union of all PMT glas
  reset(os);
  os << "R3600EGlass_"<< fPMTId;
  G4UnionSolid * eglass = new G4UnionSolid( os.str().c_str(), egfour, tgpins, 0, G4ThreeVector(0.,0.,-410.0*CLHEP::mm) ); //
  // the PMT glass logical volume
  G4LogicalVolume * eglass_log = new G4LogicalVolume( eglass, fMaterials["pmtglass"], os.str().c_str() );
  // set step limit in glass
  //G4UserLimits * glass_steplimit = new G4UserLimits( glass_stepsize );
  // eglass_log->SetUserLimits( glass_steplimit );
  
  // Place the glass
  // include the -210mm offset to put the top of the front glass at 0, instead of center of PMT dome
  G4ThreeVector offset = fLoc + G4ThreeVector( 0., 0., -210.0*CLHEP::mm );

  fVols[ "pmtglass" ] = new G4PVPlacement( fRm, offset, eglass_log, os.str().c_str(), fParent, false, 0 );
  fSurfaces.skin_surface( "pmtglass", fVols[ "pmtglass" ] );
  
  // PMT Vaccum Logical Volume.
  // It centers on PMT Glass, so placed "pmtvaccum" gets put inside PMT
  os << "R3600EVTop_" << fPMTId;
  G4Ellipsoid* evtop = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm - glass_thickness, 254.0*CLHEP::mm-glass_thickness, 210.0*CLHEP::mm-glass_thickness, 0.0*CLHEP::mm, 210.0*CLHEP::mm - glass_thickness );
  reset(os);
  os << "R3600EVBotop_" << fPMTId;
  G4Ellipsoid* evbotop = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm  - glass_thickness, 254.0*CLHEP::mm - glass_thickness, 180.0*CLHEP::mm - glass_thickness, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  // Union of top and bottom of top vacuum
  os << "R3600EVTopBotop_"<< fPMTId;
  G4UnionSolid * evtopbotop = new G4UnionSolid( os.str().c_str(), evtop, evbotop, 0, G4ThreeVector(0.,0.,0.) ); // no offset
  reset(os);
  // Build side of pmt vacuum
  os <<"R3600TVSide_"<<fPMTId;
  G4Tubs* tvside = new G4Tubs( os.str().c_str(), 0.0, 145.5*CLHEP::mm - glass_thickness, 78.5*CLHEP::mm, 0., 2.0*CLHEP::pi );
  // side vacuum is at -231.5 mm from center of PMT
  // Union of top, bottom of top, and side glass
  reset(os);
  os << "R3600EVThreeParts_"<< fPMTId;
  G4UnionSolid * evthree = new G4UnionSolid( os.str().c_str(), evtopbotop, tvside, 0, G4ThreeVector(0.,0.,-231.5*CLHEP::mm) ); // 
  reset(os);
  os << "R3600EVBot_" << fPMTId;
  G4Ellipsoid* evbot = new G4Ellipsoid( os.str().c_str(), 141.5*CLHEP::mm - glass_thickness, 141.5*CLHEP::mm - glass_thickness, 90.0*CLHEP::mm - glass_thickness, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  // bottom glass is at -310.0 mm from center of PMT
  // Union of top, bottom of top, side, and bottom glass
  reset(os);
  os << "R3600EVFourParts_"<< fPMTId;
  G4UnionSolid * evfour = new G4UnionSolid( os.str().c_str(), evthree, evbot, 0, G4ThreeVector(0.,0.,-310.0*CLHEP::mm) ); //
  // Pins part of PMT vacuum
  reset(os);
  os <<"R3600TVPins_"<<fPMTId;
  G4Tubs* tvpins = new G4Tubs( os.str().c_str(), 0.0, 49.5*CLHEP::mm - glass_thickness, 23.0*CLHEP::mm - glass_thickness/2.0, 0., 2.0*CLHEP::pi );
  // pins are offset by -410.9+2 mm from center of PMT
  // Union of all PMT glas
  reset(os);
  os << "R3600EVacuum_"<< fPMTId;
  G4UnionSolid * evacuum = new G4UnionSolid( os.str().c_str(), evfour, tvpins, 0, G4ThreeVector(0.,0.,-408.0*CLHEP::mm) ); //
  /// the PMT glass logical volume
  G4LogicalVolume * evacuum_log = new G4LogicalVolume( evacuum, fMaterials["vacuum"], os.str().c_str() );

  // Place the vacuum inside the glass
  fVols[ "pmtvacuum" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.), evacuum_log, os.str().c_str(), eglass_log, false, 0 );

  // and now the cathode. Place it in the vaccuum
  os << "R3600ECath1_" << fPMTId;
  G4Ellipsoid* ecath1 = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm - glass_thickness, 254.0*CLHEP::mm - glass_thickness, 210.0*CLHEP::mm - glass_thickness, 88.0*CLHEP::mm, 210.0*CLHEP::mm - glass_thickness );
  reset(os);
  os << "R3600ECath2_" << fPMTId; // make cathode 1 um thick (way thicker than reality ~30-60 nm?)
  G4Ellipsoid* ecath2 = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm - glass_thickness - cathode_thickness, 254.0*CLHEP::mm - glass_thickness - cathode_thickness, 210.0*CLHEP::mm - glass_thickness - cathode_thickness, 88.0*CLHEP::mm, 210.0*CLHEP::mm - glass_thickness - cathode_thickness );
  // subtract from center of ellipse to make a void
  reset(os);
  os << "R3600ECath_"<<fPMTId;
  G4SubtractionSolid* ecath = new G4SubtractionSolid( os.str().c_str(), ecath1, ecath2 );
  // cathode logical volume, sets material of cathode
  G4LogicalVolume * ecath_log = new G4LogicalVolume( ecath, fMaterials["cathode"], os.str().c_str() );
  ecath_log->SetVisAttributes( G4VisAttributes( G4Color::Yellow() ) );
  reset(os);
  os <<"R3600PCath_"<<fPMTId;
  fVols[ "cathode" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.), ecath_log, os.str().c_str(), evacuum_log, false, 0 );
  fSurfaces.skin_surface( "cathode", fVols[ "cathode" ] );
  // set step limit in cathode
  G4UserLimits * cathode_steplimit = new G4UserLimits( cathode_stepsize );
  ecath_log->SetUserLimits( cathode_steplimit );

  //attachSD();
  //disableSD(false);
  
  // reflective coating inside PMT
  reset(os);
  os << "R3600ERBotop1_" << fPMTId;
  G4Ellipsoid* erbotop1 = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm - glass_thickness , 254.0*CLHEP::mm - glass_thickness, 180.0*CLHEP::mm - glass_thickness, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  os << "R3600ERBotop2_" << fPMTId;
  G4Ellipsoid* erbotop2 = new G4Ellipsoid( os.str().c_str(), 254.0*CLHEP::mm - glass_thickness - reflect_thickness, 254.0*CLHEP::mm - glass_thickness - reflect_thickness, 180.0*CLHEP::mm - glass_thickness - reflect_thickness, -153.0*CLHEP::mm, 0.0*CLHEP::mm );
  reset(os);
  os << "R3600ERBotop_"<< fPMTId;
  G4SubtractionSolid * ertopbot = new G4SubtractionSolid( os.str().c_str(), erbotop1, erbotop2 );
  reset(os);
  // Build side of pmt reflective coat
  os <<"R3600TRSide_"<<fPMTId;
  G4Tubs* trside = new G4Tubs( os.str().c_str(), 145.5*CLHEP::mm - glass_thickness - reflect_thickness, 145.5*CLHEP::mm - glass_thickness, 78.5*CLHEP::mm/2, 0., 2.0*CLHEP::pi );
  reset(os);
  os << "R3600ERTwoParts_"<< fPMTId;
  G4UnionSolid * ertwo = new G4UnionSolid( os.str().c_str(), ertopbot, trside, 0, G4ThreeVector(0.,0.,-231.5*CLHEP::mm+78.5*CLHEP::mm/2) ); // 
  //reset(os);
  //os << "R3600ERBot1_" << fPMTId;
  //G4Ellipsoid* erbot1 = new G4Ellipsoid( os.str().c_str(), 141.5*CLHEP::mm - glass_thickness, 141.5*CLHEP::mm - glass_thickness, 90.0*CLHEP::mm - glass_thickness, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  //reset(os);
  //os << "R3600ERBot2_" << fPMTId;
  //G4Ellipsoid* erbot2 = new G4Ellipsoid( os.str().c_str(), 141.5*CLHEP::mm - glass_thickness - reflect_thickness, 141.5*CLHEP::mm - glass_thickness - reflect_thickness, 90.0*CLHEP::mm - glass_thickness - reflect_thickness, -90.0*CLHEP::mm*std::cos( 30*CLHEP::deg ), 0.0 );
  //reset(os);
  //os << "R3600ERBot_"<< fPMTId;
  //G4SubtractionSolid * erbot = new G4SubtractionSolid( os.str().c_str(), erbot1, erbot2 );
  // bottom glass is at -310.0 mm from center of PMT
  // Union of top, bottom of top, side, and bottom glass
  //reset(os);
  //os << "R3600ERThreeParts_"<< fPMTId;
  //G4UnionSolid * erthree = new G4UnionSolid( os.str().c_str(), ertwo, erbot, 0, G4ThreeVector(0.,0.,-310.0*CLHEP::mm) ); //

  // reflective coating logical volume, sets material
  G4LogicalVolume * ereflect_log = new G4LogicalVolume( ertwo, fMaterials["Al"], os.str().c_str() );
  ereflect_log->SetVisAttributes( G4VisAttributes( G4Color::Blue() ) );

  reset(os);
  os <<"R3600Reflect_"<<fPMTId;
  fVols[ "reflective" ] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,0.0), ereflect_log, os.str().c_str(), evacuum_log, false, 0 );
  fSurfaces.skin_surface( "reflector", fVols[ "reflective" ] );
  
  
  // and now the Metal dynodes
  reset(os);
  os <<"R3600FieldRim_"<<fPMTId;
  G4Tubs* tfrim = new G4Tubs( os.str().c_str(), 100.0*CLHEP::mm, 102.5*CLHEP::mm, 43.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os <<"R3600DynodeRim_"<<fPMTId;
  G4Tubs* tdrim = new G4Tubs( os.str().c_str(), 85.0*CLHEP::mm, 100.0*CLHEP::mm, 43.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os<<"R3600Union1_"<<fPMTId;
  G4UnionSolid * tdynunion1 = new G4UnionSolid(os.str().c_str(), tfrim, tdrim, 0, G4ThreeVector( 0.,0., -15.0*CLHEP::mm ) );
  reset(os);
  os<<"R3600Dynode_"<<fPMTId;
  G4Tubs* tdyn = new G4Tubs( os.str().c_str(), 0.0*CLHEP::mm, 85.0*CLHEP::mm, 43.0*CLHEP::mm, 0., 2.0*CLHEP::pi );
  reset(os);
  os<<"R3600Union2_"<<fPMTId;
  G4UnionSolid * tdynunion2 = new G4UnionSolid(os.str().c_str(), tdynunion1, tdyn, 0, G4ThreeVector( 0.,0., -52.0*CLHEP::mm ) );
  G4LogicalVolume* tdynunion2_log = new G4LogicalVolume( tdynunion2, fMaterials["pmtsteel"], os.str().c_str() );
  tdynunion2_log->SetVisAttributes( G4VisAttributes( G4Color::Red() ) );
  reset(os);
  os << "R3600PDynode_"<<fPMTId;
  //offset = fLoc + G4ThreeVector(0.,0.,-517.0*CLHEP::mm-112.0*CLHEP::mm );
  fVols["dynode"] = new G4PVPlacement( 0, G4ThreeVector(0.,0.,-112.0*CLHEP::mm), tdynunion2_log, os.str().c_str(), evacuum_log, false, 0 );
  fSurfaces.skin_surface( "pmtsteel", fVols[ "dynode" ] );

  if ( WithAcrylic() ) BuildAcrylic( );
}

void R3600Geometry::BuildAcrylic() {
  std::ostringstream os;
  MaterialsMap & fMaterials = MaterialsMap::Get();
  OpticalSurfaceDefinitions & fSurfaces = OpticalSurfaceDefinitions::Get();

  std::cout<<"R3600Geometry::BuildAcrylic thickness = "<<acrylic_thickness<<" mm"<<std::endl;
  
  // here we add more placed volumes for the acrylic and FRP
  // build top acrylic cover
  os << "AcrylEGTop1_" << fPMTId;
  G4Ellipsoid* egtop1 = new G4Ellipsoid( os.str().c_str(), 310.0*CLHEP::mm + acrylic_thickness, 310.0*CLHEP::mm + acrylic_thickness, 310.0*CLHEP::mm + acrylic_thickness, 145.0*CLHEP::mm, 310.0*CLHEP::mm + acrylic_thickness );
  reset(os);
  os << "AcrylEGTop2_" << fPMTId;
  G4Ellipsoid* egtop2 = new G4Ellipsoid( os.str().c_str(), 310.0*CLHEP::mm, 310.0*CLHEP::mm, 310.0*CLHEP::mm, 145.0*CLHEP::mm, 310.0*CLHEP::mm );
  reset(os);
  os << "AcrylEGTop_"<<fPMTId;
  G4SubtractionSolid* egtop = new G4SubtractionSolid( os.str().c_str(), egtop1, egtop2 );
  // hole in acrylic top
  reset(os);
  os << "AcrylHole_"<<fPMTId;
  G4Tubs * acrylhole = new G4Tubs( os.str().c_str(), 0.0, 4.5*CLHEP::mm, acrylic_thickness/2.0, 0.0, 2.0*CLHEP::pi );
  reset(os);
  os << "AcrylTopWHole_"<<fPMTId;
  G4SubtractionSolid* acryltopWhole = new G4SubtractionSolid( os.str().c_str(), egtop, acrylhole, 0, G4ThreeVector( 0., 0., 310.0*CLHEP::mm + acrylic_thickness/2.0 ) );
  
  // rim on acryilc
  reset(os);
  os << "AcrylRim_" <<fPMTId;
  G4Tubs* tubrim = new G4Tubs( os.str().c_str(), 281.5*CLHEP::mm, 322.0*CLHEP::mm, acrylic_thickness/2.0, 0., 2.0*CLHEP::pi );
  reset(os);
  os << "AcrylTopWRim_" <<fPMTId;
  G4UnionSolid* acrWrim = new G4UnionSolid( os.str().c_str(), acryltopWhole, tubrim, 0, G4ThreeVector(0.,0.,145.0*CLHEP::mm + acrylic_thickness/2.0 ) );
  
  
  G4LogicalVolume * eacryl_log = new G4LogicalVolume( acrWrim, fMaterials["acrylic"], os.str().c_str() );
  eacryl_log->SetVisAttributes( G4VisAttributes( G4Color::Gray() ) );
  
  // Place the acrylic
  // include the -300mm offset to put the top of the front glass at 0, instead of center of PMT dome
  G4ThreeVector offset = fLoc + G4ThreeVector( 0., 0., -300.0*CLHEP::mm );

  fVols[ "acrylic_dome" ] = new G4PVPlacement( fRm, offset, eacryl_log, os.str().c_str(), fParent, false, 0 );
  fSurfaces.skin_surface( "acrylic", fVols[ "acrylic_dome" ] );



  // build the FRP enclosure
  const double FRPThickness = 4.0 * CLHEP::mm;
  reset(os);
  os << "FRPRim_" << fPMTId;
  G4Tubs * frprim = new G4Tubs( os.str().c_str(), 275.0 * CLHEP::mm, 322.0 * CLHEP::mm, acrylic_thickness/2.0, 0., 2.0 * CLHEP::pi );
  reset(os);
  os << "FRPTTop_"<< fPMTId;
  G4Tubs * frpttop = new G4Tubs( os.str().c_str(), 275.0 * CLHEP::mm - FRPThickness, 275.0 * CLHEP::mm, 100.0 * CLHEP::mm/2.0, 0., 2.0 * CLHEP::pi );
  reset(os);
  os << "FRPSum1_"<< fPMTId;
  G4UnionSolid * frpsum1 = new G4UnionSolid( os.str().c_str(), frprim, frpttop, 0, G4ThreeVector(0.,0., -100.0 * CLHEP::mm / 2.0) );
  reset(os);
  os << "FRPETop1_"<< fPMTId;
  G4Ellipsoid * frpetop1 = new G4Ellipsoid( os.str().c_str(), 275.0 * CLHEP::mm, 275.0 * CLHEP::mm, 171.0 * CLHEP::mm, -125.0 * CLHEP::mm, 0.0 );
  reset(os);
  os << "FRPETop2_"<< fPMTId;
  G4Ellipsoid * frpetop2 = new G4Ellipsoid( os.str().c_str(), 275.0 * CLHEP::mm - FRPThickness, 275.0 * CLHEP::mm - FRPThickness, 171.0 * CLHEP::mm - FRPThickness, -125.0 * CLHEP::mm, 0.0 );
  reset(os);
  os << "FRPETop_"<< fPMTId;
  G4SubtractionSolid* frpetop = new G4SubtractionSolid( os.str().c_str(), frpetop1, frpetop2 );
  reset(os);
  os << "FRPSum2_"<< fPMTId;
  G4UnionSolid * frpsum2 = new G4UnionSolid( os.str().c_str(), frpsum1, frpetop, 0, G4ThreeVector(0.,0., -100.0 * CLHEP::mm ) );
  reset(os);
  os << "FRPSide_"<< fPMTId;
  //  G4Tubs* frpside = new G4Tubs( os.str().c_str(), 148.0 * CLHEP::mm - FRPThickness, 148.0 * CLHEP::mm, 116.0 * CLHEP::mm/2.0, 0.0, 2.0*CLHEP::pi );
  G4Tubs* frpside = new G4Tubs( os.str().c_str(), 160.0 * CLHEP::mm - FRPThickness, 160.0 * CLHEP::mm, 160.0 * CLHEP::mm/2.0, 0.0, 2.0*CLHEP::pi );
  reset(os);
  os << "FRPSum3_"<< fPMTId;
  //  G4UnionSolid * frpsum3 = new G4UnionSolid( os.str().c_str(), frpsum2, frpside, 0, G4ThreeVector(0.,0., -269.0*CLHEP::mm) );
  G4UnionSolid * frpsum3 = new G4UnionSolid( os.str().c_str(), frpsum2, frpside, 0, G4ThreeVector(0.,0., -281.0*CLHEP::mm) );
  reset(os);
  os << "FRPEBot1_"<< fPMTId;
  G4Ellipsoid * frpebot1 = new G4Ellipsoid( os.str().c_str(), 160.0 * CLHEP::mm,  160.0 * CLHEP::mm, 110.0 * CLHEP::mm, -84.0 * CLHEP::mm, 0.0 );
  reset(os);
  os << "FRPEBot2_"<< fPMTId;
  G4Ellipsoid * frpebot2 = new G4Ellipsoid( os.str().c_str(), 160.0 * CLHEP::mm - FRPThickness,  160.0 * CLHEP::mm - FRPThickness, 110.0 * CLHEP::mm - FRPThickness, -84.0 * CLHEP::mm, 0.0 );
  reset(os);
  os << "FRPEBot_"<< fPMTId;
  G4SubtractionSolid* frpebot = new G4SubtractionSolid( os.str().c_str(), frpebot1, frpebot2 );
  reset(os);
  os << "FRPSum4_"<< fPMTId;
  //G4UnionSolid * frpsum4 = new G4UnionSolid( os.str().c_str(), frpsum3, frpebot, 0, G4ThreeVector(0.,0., -327.0*CLHEP::mm) );
  G4UnionSolid * frpsum4 = new G4UnionSolid( os.str().c_str(), frpsum3, frpebot, 0, G4ThreeVector(0.,0., -361.0*CLHEP::mm) );
  reset(os);
  os << "FRTBot_"<< fPMTId;
  G4Tubs* frptbot = new G4Tubs( os.str().c_str(), 100.0 * CLHEP::mm - FRPThickness, 100.0 * CLHEP::mm, 89.5 * CLHEP::mm/2.0, 0.0, 2.0*CLHEP::pi );
  reset(os);
  os << "FRPSum5_"<< fPMTId;
  //G4UnionSolid * frpsum5 = new G4UnionSolid( os.str().c_str(), frpsum4, frptbot, 0, G4ThreeVector(0.,0., -470.5*CLHEP::mm) );
  G4UnionSolid * frpsum5 = new G4UnionSolid( os.str().c_str(), frpsum4, frptbot, 0, G4ThreeVector(0.,0., -494.5*CLHEP::mm) );

  G4LogicalVolume * frp_log = new G4LogicalVolume( frpsum5, fMaterials["frp"], os.str().c_str() );
  //G4LogicalVolume * frp_log = new G4LogicalVolume( frpsum3, fMaterials["frp"], os.str().c_str() );
  frp_log->SetVisAttributes( G4VisAttributes( G4Color::Brown() ) );
  
  

  offset = fLoc + G4ThreeVector( 0., 0., -165.0*CLHEP::mm );
  fVols[ "frp_cover" ] = new G4PVPlacement( fRm, offset, frp_log, os.str().c_str(), fParent, false, 0 );
  fSurfaces.skin_surface( "frp", fVols[ "frp_cover" ] );
    
  
}

