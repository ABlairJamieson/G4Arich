
#include "OpticalSurfaceDefinitions.hh"
#include "G4MaterialPropertiesTable.hh"

std::vector< G4LogicalBorderSurface * > OpticalSurfaceDefinitions::fBorders;
std::vector< G4LogicalSkinSurface * > OpticalSurfaceDefinitions::fSkins;
std::map< std::string, G4OpticalSurface * > OpticalSurfaceDefinitions::fSurfaces;
OpticalSurfaceDefinitions * OpticalSurfaceDefinitions::fOpSurfaceDefs = nullptr;

OpticalSurfaceDefinitions& OpticalSurfaceDefinitions::Get(){
  if ( fOpSurfaceDefs == nullptr ) OpticalSurfaceDefinitions();
  return * fOpSurfaceDefs;
}

G4OpticalSurface * OpticalSurfaceDefinitions::operator[]( const std::string& key ) const {
  if ( fSurfaces[ key ] == nullptr ){
    std::cerr<<"OpticalSurfaceDefinitions::operator[] key="<<key<<" not found"<<std::endl;
    throw Invalid();
  }
  return fSurfaces[ key ];
}

 
void OpticalSurfaceDefinitions::skin_surface( const std::string& surface_name,
					      G4VPhysicalVolume* volume ){
  G4OpticalSurface * surf = fSurfaces[ surface_name ];
  if ( surf == nullptr ) {
    std::cerr<<"OpticalSurfaceDefinitions::skin_surface name="<<surface_name<<" not found"<<std::endl;
    throw Invalid();
  }
  G4LogicalVolume * lvol = volume->GetLogicalVolume();
  fSkins.push_back( new G4LogicalSkinSurface( surface_name, lvol, surf ) );
}

 
void OpticalSurfaceDefinitions::border_surface( const std::string& surface_name,
						G4VPhysicalVolume* volume1,
						G4VPhysicalVolume* volume2  ){
  G4OpticalSurface * surf = fSurfaces[ surface_name ];
  if ( surf == nullptr ) {
    std::cerr<<"OpticalSurfaceDefinitions::border_surface name="<<surface_name<<" not found"<<std::endl;
    throw Invalid();
  }
  fBorders.push_back( new G4LogicalBorderSurface( surface_name, volume1, volume2, surf ) );
}

OpticalSurfaceDefinitions::OpticalSurfaceDefinitions(){
  if ( fOpSurfaceDefs == nullptr ) {
    BuildSurfaces();
    fOpSurfaceDefs  = this;
  }
}


void OpticalSurfaceDefinitions::ReadOptSurfaces(){
  
  std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces() starting "<<std::endl;
  // default values:
  blacksheet_BSRFF = 1.0;
  blacksheet_CSL = 0.3;
  blacksheet_CSS = 0.2;
  blacksheet_CBS = 0.2;
  blacksheet_sigmaalpha = 0.15; //radians
  
  //# cathode reflection fractions and roughness
  cathode_CSS = 0.7;
  cathode_CSL = 0.2;
  cathode_CBS = 0.0;
  cathode_sigmaalpha = 0.314;

  //# pmt glass reflection fractions and roughness
  pmtglass_CSL = 0.98;
  pmtglass_CSS = 0.02;
  pmtglass_CBS = 0.00;
  pmtglass_sigmaalpha  = 0.03;

  //# acrylic reflection fractions and roughness
  acrylic_CSS = 0.7;
  acrylic_CSL = 0.2;
  acrylic_CBS = 0.02;
  acrylic_sigmaalpha = 0.314;

  //# water reflection fractions and roughness
  water_CSS = 0.01;
  water_CSL = 0.99;
  water_CBS = 0.00;
  water_sigmaalpha = 0.001;

  //# reflector
  reflector_reflectivity = 0.95;

  //# pmt steel reflectivity
  pmtsteel_reflectivity = 0.8;

  std::ifstream infile( "ptf-optsurfaces.txt" );

  if ( !infile ) {
    std::cout<<"Could not open ptf-optsurfaces.txt, using default parameters"<<std::endl;
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
    } else if ( tag == "blacksheet_BSRFF" ) {
      is >> blacksheet_BSRFF;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces blacksheet_BSRFF = "<<blacksheet_BSRFF<<std::endl;
    } else if ( tag == "blacksheet_CSL" ) {
      is >> blacksheet_CSL;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces blacksheet_CSL = "<<blacksheet_CSL<<std::endl;
    } else if ( tag == "blacksheet_CSS" ) {
      is >> blacksheet_CSS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces blacksheet_CSS = "<<blacksheet_CSS<<std::endl;
    } else if ( tag == "blacksheet_CBS" ) {
      is >> blacksheet_CBS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces blacksheet_CBS = "<<blacksheet_CBS<<std::endl;
    } else if ( tag == "blacksheet_sigmaalpha" ) {
      is >> blacksheet_sigmaalpha;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces blacksheet_sigmaalpha = "<<blacksheet_sigmaalpha<<" radians"<<std::endl;


    } else if ( tag == "cathode_CSL" ) {
      is >> cathode_CSL;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces cathode_CSL = "<<cathode_CSL<<std::endl;
    } else if ( tag == "cathode_CSS" ) {
      is >> cathode_CSS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces cathode_CSS = "<<cathode_CSS<<std::endl;
    } else if ( tag == "cathode_CBS" ) {
      is >> cathode_CBS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces cathode_CBS = "<<cathode_CBS<<std::endl;
    } else if ( tag == "cathode_sigmaalpha" ) {
      is >> cathode_sigmaalpha;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces cathode_sigmaalpha = "<<cathode_sigmaalpha<<" radians"<<std::endl;


    } else if ( tag == "pmtglass_CSL" ) {
      is >> pmtglass_CSL;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces pmtglass_CSL = "<<pmtglass_CSL<<std::endl;
    } else if ( tag == "pmtglass_CSS" ) {
      is >> pmtglass_CSS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces pmtglass_CSS = "<<pmtglass_CSS<<std::endl;
    } else if ( tag == "pmtglass_CBS" ) {
      is >> pmtglass_CBS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces pmtglass_CBS = "<<pmtglass_CBS<<std::endl;
    } else if ( tag == "pmtglass_sigmaalpha" ) {
      is >> pmtglass_sigmaalpha;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces pmtglass_sigmaalpha = "<<pmtglass_sigmaalpha<<" radians"<<std::endl;


    } else if ( tag == "acrylic_CSL" ) {
      is >> acrylic_CSL;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces acrylic_CSL = "<<acrylic_CSL<<std::endl;
    } else if ( tag == "acrylic_CSS" ) {
      is >> acrylic_CSS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces acrylic_CSS = "<<acrylic_CSS<<std::endl;
    } else if ( tag == "acrylic_CBS" ) {
      is >> acrylic_CBS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces acrylic_CBS = "<<acrylic_CBS<<std::endl;
    } else if ( tag == "acrylic_sigmaalpha" ) {
      is >> acrylic_sigmaalpha;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces acrylic_sigmaalpha = "<<acrylic_sigmaalpha<<" radians"<<std::endl;


    } else if ( tag == "water_CSL" ) {
      is >> water_CSL;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces water_CSL = "<<water_CSL<<std::endl;
    } else if ( tag == "water_CSS" ) {
      is >> water_CSS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces water_CSS = "<<water_CSS<<std::endl;
    } else if ( tag == "water_CBS" ) {
      is >> water_CBS;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces water_CBS = "<<water_CBS<<std::endl;
    } else if ( tag == "water_sigmaalpha" ) {
      is >> water_sigmaalpha;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces water_sigmaalpha = "<<water_sigmaalpha<<" radians"<<std::endl;

    } else if ( tag =="reflector_reflectivity" ){
      is >> reflector_reflectivity;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces reflector_reflectivity = "<<reflector_reflectivity<<std::endl;
 
    } else if ( tag =="pmtsteel_reflectivity" ){
      is >> pmtsteel_reflectivity;
      std::cout<<"OpticalSurfaceDefinitions::ReadOptSurfaces pmtsteel_reflectivity = "<<pmtsteel_reflectivity<<std::endl;
    } 

  }

}



void OpticalSurfaceDefinitions::BuildSurfaces() {

  ReadOptSurfaces();
  
  const G4int NUM = 2;
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** water *************************************************************************
   fSurfaces[ "water" ] = new G4OpticalSurface("Water");
   fSurfaces[ "water" ]->SetType(dielectric_dielectric);
   fSurfaces[ "water" ]->SetModel(unified); 
   fSurfaces[ "water" ]->SetFinish(ground);
   fSurfaces[ "water" ]->SetSigmaAlpha( water_sigmaalpha );
   G4double ENERGY_water[NUM] = { 1.4*CLHEP::eV,6.2*CLHEP::eV};
   G4double CSL_water   [NUM] = { water_CSL, water_CSL };  //Specular Lobe fraction
   G4double CSS_water   [NUM] = { water_CSS, water_CSS };  //Specular Scatter fraction
   G4double CBS_water   [NUM] = { water_CBS, water_CBS};  //backscatter fraction
   G4MaterialPropertiesTable *mySTWa = new G4MaterialPropertiesTable();
   mySTWa->AddProperty("SPECULARLOBECONSTANT",  ENERGY_water, CSL_water,    NUM);
   mySTWa->AddProperty("SPECULARSPIKECONSTANT", ENERGY_water, CSS_water,    NUM);
   mySTWa->AddProperty("BACKSCATTERCONSTANT",   ENERGY_water, CBS_water,    NUM);
   fSurfaces[ "water" ]->SetMaterialPropertiesTable(mySTWa);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** air *************************************************************************
   fSurfaces[ "air" ] = new G4OpticalSurface("Air");
   fSurfaces[ "air" ]->SetType(dielectric_dielectric);
   fSurfaces[ "air" ]->SetModel(unified);
   fSurfaces[ "air" ]->SetFinish(polished);
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** blacksheet ***********************************************************************************
   const unsigned NUMENTRIES_blacksheet = 60;
   fSurfaces[ "blacksheet" ] = new G4OpticalSurface("BlacksheetSurface");
   fSurfaces[ "blacksheet" ]->SetType(dielectric_dielectric);
   fSurfaces[ "blacksheet" ]->SetModel(unified); 
   fSurfaces[ "blacksheet" ]->SetFinish(ground);
   fSurfaces[ "blacksheet" ]->SetSigmaAlpha( blacksheet_sigmaalpha );
   G4double ENERGY_blacksheet[NUMENTRIES_blacksheet] =
     { 1.56962*CLHEP::eV, 1.58974*CLHEP::eV, 1.61039*CLHEP::eV, 1.63157*CLHEP::eV, 
       1.65333*CLHEP::eV, 1.67567*CLHEP::eV, 1.69863*CLHEP::eV, 1.72222*CLHEP::eV, 
       1.74647*CLHEP::eV, 1.77142*CLHEP::eV, 1.7971*CLHEP::eV,  1.82352*CLHEP::eV, 
       1.85074*CLHEP::eV, 1.87878*CLHEP::eV, 1.90769*CLHEP::eV, 1.93749*CLHEP::eV, 
       1.96825*CLHEP::eV, 1.99999*CLHEP::eV, 2.03278*CLHEP::eV, 2.06666*CLHEP::eV,
       2.10169*CLHEP::eV, 2.13793*CLHEP::eV, 2.17543*CLHEP::eV, 2.21428*CLHEP::eV, 
       2.25454*CLHEP::eV, 2.29629*CLHEP::eV, 2.33962*CLHEP::eV, 2.38461*CLHEP::eV, 
       2.43137*CLHEP::eV, 2.47999*CLHEP::eV, 2.53061*CLHEP::eV, 2.58333*CLHEP::eV, 
       2.63829*CLHEP::eV, 2.69565*CLHEP::eV, 2.75555*CLHEP::eV, 2.81817*CLHEP::eV, 
       2.88371*CLHEP::eV, 2.95237*CLHEP::eV, 3.02438*CLHEP::eV, 3.09999*CLHEP::eV,
       3.17948*CLHEP::eV, 3.26315*CLHEP::eV, 3.35134*CLHEP::eV, 3.44444*CLHEP::eV, 
       3.54285*CLHEP::eV, 3.64705*CLHEP::eV, 3.75757*CLHEP::eV, 3.87499*CLHEP::eV, 
       3.99999*CLHEP::eV, 4.13332*CLHEP::eV, 4.27585*CLHEP::eV, 4.42856*CLHEP::eV, 
       4.59258*CLHEP::eV, 4.76922*CLHEP::eV, 4.95999*CLHEP::eV, 5.16665*CLHEP::eV, 
       5.39129*CLHEP::eV, 5.63635*CLHEP::eV, 5.90475*CLHEP::eV, 6.19998*CLHEP::eV };
   G4double CSL_blacksheet   [NUMENTRIES_blacksheet]; //Specular Lobe fraction
   for ( unsigned i=0; i < NUMENTRIES_blacksheet; ++i ) CSL_blacksheet[i]= blacksheet_CSL;
   G4double CSS_blacksheet   [NUMENTRIES_blacksheet]; //Specular Scatter fraction
   for ( unsigned i=0; i < NUMENTRIES_blacksheet; ++i ) CSS_blacksheet[i]= blacksheet_CSS;
   G4double CBS_blacksheet   [NUMENTRIES_blacksheet]; //backscatter fraction
   for ( unsigned i=0; i < NUMENTRIES_blacksheet; ++i ) CBS_blacksheet[i]= blacksheet_CBS;
   G4double REFLECTIVITY_blacksheet[NUMENTRIES_blacksheet] =
     { 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF,
       0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.057*blacksheet_BSRFF,
       0.059*blacksheet_BSRFF, 0.060*blacksheet_BSRFF, 0.059*blacksheet_BSRFF, 0.058*blacksheet_BSRFF,
       0.057*blacksheet_BSRFF, 0.055*blacksheet_BSRFF, 0.050*blacksheet_BSRFF, 0.045*blacksheet_BSRFF,
       0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF,
       0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF,
       0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF,
       0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF,
       0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF, 0.045*blacksheet_BSRFF };
   G4MaterialPropertiesTable *myST1 = new G4MaterialPropertiesTable();
   myST1->AddProperty("SPECULARLOBECONSTANT",  ENERGY_blacksheet, CSL_blacksheet,    NUMENTRIES_blacksheet);
   myST1->AddProperty("SPECULARSPIKECONSTANT", ENERGY_blacksheet, CSS_blacksheet,    NUMENTRIES_blacksheet);
   myST1->AddProperty("BACKSCATTERCONSTANT",   ENERGY_blacksheet, CBS_blacksheet,    NUMENTRIES_blacksheet);
   myST1->AddProperty("REFLECTIVITY",          ENERGY_blacksheet, REFLECTIVITY_blacksheet, NUMENTRIES_blacksheet);
   fSurfaces[ "blacksheet" ]->SetMaterialPropertiesTable(myST1);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** cathode *************************************************************************************
   const unsigned NUMENTRIES_cathode=2;
   G4double ENERGY_cathode[NUMENTRIES_cathode] = { 1.4 *CLHEP::eV, 6.2 * CLHEP::eV };
   G4double CSS_cathode[NUMENTRIES_cathode] = { cathode_CSS, cathode_CSS };
   G4double CSL_cathode[NUMENTRIES_cathode] = { cathode_CSL, cathode_CSL };
   G4double CBS_cathode[NUMENTRIES_cathode] = { cathode_CBS, cathode_CBS };
   G4MaterialPropertiesTable *myMPTcath = new G4MaterialPropertiesTable();
   myMPTcath->AddProperty("SPECULARLOBECONSTANT",  ENERGY_cathode, CSL_cathode, NUMENTRIES_cathode);
   myMPTcath->AddProperty("SPECULARSPIKECONSTANT", ENERGY_cathode, CSS_cathode, NUMENTRIES_cathode);
   myMPTcath->AddProperty("BACKSCATTERCONSTANT",   ENERGY_cathode, CBS_cathode, NUMENTRIES_cathode);
   fSurfaces[ "cathode" ] = new G4OpticalSurface("cathode" );
   fSurfaces[ "cathode" ]->SetType(dielectric_dielectric);
   fSurfaces[ "cathode" ]->SetModel(unified);  
   fSurfaces[ "cathode" ]->SetFinish(ground);
   fSurfaces[ "cathode" ]->SetSigmaAlpha( cathode_sigmaalpha );
   fSurfaces[ "cathode" ]->SetMaterialPropertiesTable(myMPTcath);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** fiber reinforced plastic  ********************************************************************
   fSurfaces[ "frp" ] = new G4OpticalSurface("PmtGlassSurface");
   fSurfaces[ "frp" ]->SetType(dielectric_dielectric);
   fSurfaces[ "frp" ]->SetModel(unified);  
   fSurfaces[ "frp" ]->SetFinish(groundfrontpainted);
   G4double ENERGY_frp[NUM]  = { 1.4*CLHEP::eV, 6.2*CLHEP::eV };
   G4double REFLECT_frp[NUM] = { 0.5, 0.5 };
   G4MaterialPropertiesTable *frpMPT = new G4MaterialPropertiesTable();
   frpMPT->AddProperty("REFLECTIVITY",          ENERGY_frp, REFLECT_frp, NUM);
   fSurfaces[ "frp" ]->SetMaterialPropertiesTable(frpMPT);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** pmtglass *************************************************************************************
   fSurfaces[ "pmtglass" ] = new G4OpticalSurface("PmtGlassSurface");
   fSurfaces[ "pmtglass" ]->SetType(dielectric_dielectric);
   fSurfaces[ "pmtglass" ]->SetModel(unified);  
   fSurfaces[ "pmtglass" ]->SetFinish(ground);
   fSurfaces[ "pmtglass" ]->SetSigmaAlpha( pmtglass_sigmaalpha );
   G4double ENERGY_glass[NUM] = { 1.4*CLHEP::eV, 6.2*CLHEP::eV };
   G4double CSL_glass   [NUM] = { pmtglass_CSL, pmtglass_CSL };  //Specular Lobe fraction
   G4double CSS_glass   [NUM] = { pmtglass_CSS, pmtglass_CSS };  //Specular Scatter fraction
   G4double CBS_glass   [NUM] = { pmtglass_CBS, pmtglass_CBS };  //backscatter fraction   
   G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
   myST2->AddProperty("SPECULARLOBECONSTANT",  ENERGY_glass, CSL_glass, NUM);
   myST2->AddProperty("SPECULARSPIKECONSTANT", ENERGY_glass, CSS_glass, NUM);
   myST2->AddProperty("BACKSCATTERCONSTANT",   ENERGY_glass, CBS_glass, NUM);
   fSurfaces[ "pmtglass" ]->SetMaterialPropertiesTable(myST2);

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** white tyvek *************************************************************************
   fSurfaces[ "tyvek" ] = new G4OpticalSurface("TyvekSurface");
   fSurfaces[ "tyvek" ]->SetType( dielectric_LUTDAVIS );
   fSurfaces[ "tyvek" ]->SetModel( DAVIS );
   fSurfaces[ "tyvek" ]->SetFinish( polishedteflonair );

   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** reflector *************************************************************************
   fSurfaces[ "reflector" ] = new G4OpticalSurface("reflector");
   fSurfaces[ "reflector" ]->SetType( dielectric_metal );
   fSurfaces[ "reflector" ]->SetModel( unified );
   fSurfaces[ "reflector" ]->SetFinish( polished );
   G4double ENE_refl[NUM] = {1.0*CLHEP::eV, 6.0*CLHEP::eV};
   G4double REF_refl[NUM] = {0.95, 0.95};
   G4MaterialPropertiesTable *mySTrefl = new G4MaterialPropertiesTable();
   mySTrefl->AddProperty("REFLECTIVITY", ENE_refl, REF_refl, NUM);
   fSurfaces[ "reflector" ]->SetMaterialPropertiesTable( mySTrefl );

   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** pmtsteel *************************************************************************
   fSurfaces[ "pmtsteel" ] = new G4OpticalSurface("pmtsteel");
   fSurfaces[ "pmtsteel" ]->SetType(dielectric_metal);
   fSurfaces[ "pmtsteel" ]->SetModel(unified); 
   fSurfaces[ "pmtsteel" ]->SetFinish(polished);
   G4double ENE_pmtsteel[NUM] = {1.0*CLHEP::eV, 6.0*CLHEP::eV};
   G4double REF_pmtsteel[NUM] = {pmtsteel_reflectivity, pmtsteel_reflectivity};
   G4MaterialPropertiesTable *mySTeel = new G4MaterialPropertiesTable();
   mySTeel->AddProperty("REFLECTIVITY", ENE_pmtsteel, REF_pmtsteel, NUM);
   fSurfaces[ "pmtsteel" ]->SetMaterialPropertiesTable( mySTeel );
   
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   // **** acrylic *************************************************************************
   fSurfaces[ "acrylic" ] = new G4OpticalSurface("AcrylicSurface");
   fSurfaces[ "acrylic" ]->SetType(dielectric_dielectric);
   fSurfaces[ "acrylic" ]->SetModel(unified);  
   fSurfaces[ "acrylic" ]->SetFinish(ground);
   fSurfaces[ "acrylic" ]->SetSigmaAlpha( acrylic_sigmaalpha );
   G4double ENERGY_acrylic[NUM] = { 1.0*CLHEP::eV, 6.0*CLHEP::eV };
   G4double CSL_acrylic   [NUM] = { acrylic_CSL, acrylic_CSL };  //Specular Lobe fraction
   G4double CSS_acrylic   [NUM] = { acrylic_CSS, acrylic_CSS };  //Specular Scatter fraction
   G4double CBS_acrylic   [NUM] = { acrylic_CBS, acrylic_CBS };  //backscatter fraction
   G4MaterialPropertiesTable *AcrPropTable = new G4MaterialPropertiesTable();
   AcrPropTable->AddProperty("SPECULARLOBECONSTANT",  ENERGY_acrylic,   CSL_acrylic, NUM);
   AcrPropTable->AddProperty("SPECULARSPIKECONSTANT", ENERGY_acrylic,   CSS_acrylic, NUM);
   AcrPropTable->AddProperty("BACKSCATTERCONSTANT",   ENERGY_acrylic,   CBS_acrylic, NUM);
   fSurfaces[ "acrylic" ]->SetMaterialPropertiesTable(AcrPropTable);
   
   
}




