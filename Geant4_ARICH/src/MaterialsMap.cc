#include "MaterialsMap.hh"
#include "G4NistManager.hh"
#include <TGraph.h>

MaterialsMap * MaterialsMap::fMaterialsMap = nullptr;
std::map< std::string, G4Material * > MaterialsMap::fMaterials;

MaterialsMap& MaterialsMap::Get() {
  std::cout<<"MaterialsMap::Get()"<<std::endl;
  if ( fMaterialsMap==nullptr ) MaterialsMap();
  return *fMaterialsMap;
}

G4Material* MaterialsMap::operator[]( const std::string& key ) const{
  if ( fMaterials[key] == nullptr ) throw Invalid();
  return fMaterials[ key ];
}

void MaterialsMap::get_material_names( std::vector< std::string > & matnames ) const{
  for ( const auto & curpair : fMaterials ){
    matnames.push_back( curpair.first );
  }
}

MaterialsMap::MaterialsMap(){
  std::cout<<"MaterialsMap constructor"<<std::endl;
  if ( fMaterialsMap == nullptr ){
    BuildMaterials();
    fMaterialsMap = this;
    std::cout << *fMaterialsMap;
  }
}

void MaterialsMap::ReadMaterials(){
  
  std::cout<<"MaterialsMap::ReadMaterials() starting "<<std::endl;
  water_RAYFF = 0.625;
  water_ABWFF = 1.0;
  
  std::ifstream infile( "ptf-materials.txt" );

  if ( !infile ) {
    std::cout<<"Could not open ptf-materials.txt, using default parameters"<<std::endl;
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
    } else if ( tag == "water_RAYFF" ) {
      is >> water_RAYFF;
      std::cout<<"MaterialsMap::ReadMaterials water_RAYFF = "<<water_RAYFF<<std::endl;
    } else if ( tag == "water_ABWFF" ) {
      is >> water_ABWFF;
      std::cout<<"MaterialsMap::ReadMaterials water_ABWFF = "<<water_ABWFF<<std::endl;
    } else if ( tag == "cathode_ABWFF" ) {
      is >> cathode_ABWFF;
      std::cout<<"MaterialsMap::ReadMaterials cathode_ABWFF = "<<cathode_ABWFF<<std::endl;
    } else if ( tag == "glass_ABWFF" ) {
      is >> glass_ABWFF;
      std::cout<<"MaterialsMap::ReadMaterials glass_ABWFF = "<<glass_ABWFF<<std::endl;
    } 
  }

}


void MaterialsMap::BuildMaterials(){
  std::cout<<"MaterialsMap::BuildMaterials"<<std::endl;

  ReadMaterials();
  
  // pmtglass, pmtsteel, cathode, vacuum, acrylic
  G4NistManager* nist = G4NistManager::Instance();

  fMaterials[ "vacuum" ] = nist->FindOrBuildMaterial( "G4_Galactic" );
  G4MaterialPropertiesTable *vacMPT = new G4MaterialPropertiesTable();
  const G4int NUMENTRIES_vac = 2;
  G4double ENERGY_vac[NUMENTRIES_vac] = {1.*CLHEP::eV , 7.*CLHEP::eV};
  G4double RINDEX_vac[NUMENTRIES_vac] = {1.0, 1.0};
  vacMPT->AddProperty("RINDEX",   ENERGY_vac, RINDEX_vac,          NUMENTRIES_vac);
  fMaterials[ "vacuum" ]->SetMaterialPropertiesTable( vacMPT );

  
  fMaterials[ "acrylic" ] = nist->FindOrBuildMaterial( "G4_PLEXIGLASS" );
  G4MaterialPropertiesTable *acrMPT = new G4MaterialPropertiesTable();
  const G4int NUMENTRIES_acryl = 306;
  G4double ENERGY_skAcrylic[NUMENTRIES_acryl] =
     { 2.066*CLHEP::eV, 2.070*CLHEP::eV, 2.073*CLHEP::eV, 2.077*CLHEP::eV, 2.080*CLHEP::eV, 2.084*CLHEP::eV, 2.087*CLHEP::eV,
       2.091*CLHEP::eV, 2.094*CLHEP::eV, 2.098*CLHEP::eV, 2.101*CLHEP::eV, 2.105*CLHEP::eV, 2.109*CLHEP::eV, 2.112*CLHEP::eV,
       2.116*CLHEP::eV, 2.119*CLHEP::eV, 2.123*CLHEP::eV, 2.127*CLHEP::eV, 2.130*CLHEP::eV, 2.134*CLHEP::eV, 2.138*CLHEP::eV,
       2.141*CLHEP::eV, 2.145*CLHEP::eV, 2.149*CLHEP::eV, 2.153*CLHEP::eV, 2.156*CLHEP::eV, 2.160*CLHEP::eV, 2.164*CLHEP::eV,
       2.168*CLHEP::eV, 2.171*CLHEP::eV, 2.175*CLHEP::eV, 2.179*CLHEP::eV, 2.183*CLHEP::eV, 2.187*CLHEP::eV, 2.191*CLHEP::eV,
       2.194*CLHEP::eV, 2.198*CLHEP::eV, 2.202*CLHEP::eV, 2.206*CLHEP::eV, 2.210*CLHEP::eV, 2.214*CLHEP::eV, 2.218*CLHEP::eV,
       2.222*CLHEP::eV, 2.226*CLHEP::eV, 2.230*CLHEP::eV, 2.234*CLHEP::eV, 2.238*CLHEP::eV, 2.242*CLHEP::eV, 2.246*CLHEP::eV,
       2.250*CLHEP::eV, 2.254*CLHEP::eV, 2.258*CLHEP::eV, 2.262*CLHEP::eV, 2.267*CLHEP::eV, 2.271*CLHEP::eV, 2.275*CLHEP::eV,
       2.279*CLHEP::eV, 2.283*CLHEP::eV, 2.288*CLHEP::eV, 2.292*CLHEP::eV, 2.296*CLHEP::eV, 2.300*CLHEP::eV, 2.305*CLHEP::eV,
       2.309*CLHEP::eV, 2.313*CLHEP::eV, 2.317*CLHEP::eV, 2.322*CLHEP::eV, 2.326*CLHEP::eV, 2.331*CLHEP::eV, 2.335*CLHEP::eV,
       2.339*CLHEP::eV, 2.344*CLHEP::eV, 2.348*CLHEP::eV, 2.353*CLHEP::eV, 2.357*CLHEP::eV, 2.362*CLHEP::eV, 2.366*CLHEP::eV,
       2.371*CLHEP::eV, 2.375*CLHEP::eV, 2.380*CLHEP::eV, 2.384*CLHEP::eV, 2.389*CLHEP::eV, 2.394*CLHEP::eV, 2.398*CLHEP::eV,
       2.403*CLHEP::eV, 2.407*CLHEP::eV, 2.412*CLHEP::eV, 2.417*CLHEP::eV, 2.422*CLHEP::eV, 2.426*CLHEP::eV, 2.431*CLHEP::eV,
       2.436*CLHEP::eV, 2.441*CLHEP::eV, 2.445*CLHEP::eV, 2.450*CLHEP::eV, 2.455*CLHEP::eV, 2.460*CLHEP::eV, 2.465*CLHEP::eV,
       2.470*CLHEP::eV, 2.475*CLHEP::eV, 2.480*CLHEP::eV, 2.485*CLHEP::eV, 2.490*CLHEP::eV, 2.495*CLHEP::eV, 2.500*CLHEP::eV,
       2.505*CLHEP::eV, 2.510*CLHEP::eV, 2.515*CLHEP::eV, 2.520*CLHEP::eV, 2.525*CLHEP::eV, 2.530*CLHEP::eV, 2.535*CLHEP::eV,
       2.541*CLHEP::eV, 2.546*CLHEP::eV, 2.551*CLHEP::eV, 2.556*CLHEP::eV, 2.562*CLHEP::eV, 2.567*CLHEP::eV, 2.572*CLHEP::eV,
       2.578*CLHEP::eV, 2.583*CLHEP::eV, 2.588*CLHEP::eV, 2.594*CLHEP::eV, 2.599*CLHEP::eV, 2.605*CLHEP::eV, 2.610*CLHEP::eV,
       2.616*CLHEP::eV, 2.621*CLHEP::eV, 2.627*CLHEP::eV, 2.632*CLHEP::eV, 2.638*CLHEP::eV, 2.644*CLHEP::eV, 2.649*CLHEP::eV,
       2.655*CLHEP::eV, 2.661*CLHEP::eV, 2.666*CLHEP::eV, 2.672*CLHEP::eV, 2.678*CLHEP::eV, 2.684*CLHEP::eV, 2.689*CLHEP::eV,
       2.695*CLHEP::eV, 2.701*CLHEP::eV, 2.707*CLHEP::eV, 2.713*CLHEP::eV, 2.719*CLHEP::eV, 2.725*CLHEP::eV, 2.731*CLHEP::eV,
       2.737*CLHEP::eV, 2.743*CLHEP::eV, 2.749*CLHEP::eV, 2.755*CLHEP::eV, 2.761*CLHEP::eV, 2.768*CLHEP::eV, 2.774*CLHEP::eV,
       2.780*CLHEP::eV, 2.786*CLHEP::eV, 2.792*CLHEP::eV, 2.799*CLHEP::eV, 2.805*CLHEP::eV, 2.811*CLHEP::eV, 2.818*CLHEP::eV,
       2.824*CLHEP::eV, 2.831*CLHEP::eV, 2.837*CLHEP::eV, 2.844*CLHEP::eV, 2.850*CLHEP::eV, 2.857*CLHEP::eV, 2.863*CLHEP::eV,
       2.870*CLHEP::eV, 2.877*CLHEP::eV, 2.883*CLHEP::eV, 2.890*CLHEP::eV, 2.897*CLHEP::eV, 2.904*CLHEP::eV, 2.910*CLHEP::eV,
       2.917*CLHEP::eV, 2.924*CLHEP::eV, 2.931*CLHEP::eV, 2.938*CLHEP::eV, 2.945*CLHEP::eV, 2.952*CLHEP::eV, 2.959*CLHEP::eV,
       2.966*CLHEP::eV, 2.973*CLHEP::eV, 2.980*CLHEP::eV, 2.988*CLHEP::eV, 2.995*CLHEP::eV, 3.002*CLHEP::eV, 3.009*CLHEP::eV,
       3.017*CLHEP::eV, 3.024*CLHEP::eV, 3.031*CLHEP::eV, 3.039*CLHEP::eV, 3.046*CLHEP::eV, 3.054*CLHEP::eV, 3.061*CLHEP::eV,
       3.069*CLHEP::eV, 3.077*CLHEP::eV, 3.084*CLHEP::eV, 3.092*CLHEP::eV, 3.100*CLHEP::eV, 3.107*CLHEP::eV, 3.115*CLHEP::eV,
       3.123*CLHEP::eV, 3.131*CLHEP::eV, 3.139*CLHEP::eV, 3.147*CLHEP::eV, 3.155*CLHEP::eV, 3.163*CLHEP::eV, 3.171*CLHEP::eV,
       3.179*CLHEP::eV, 3.187*CLHEP::eV, 3.195*CLHEP::eV, 3.204*CLHEP::eV, 3.212*CLHEP::eV, 3.220*CLHEP::eV, 3.229*CLHEP::eV,
       3.237*CLHEP::eV, 3.246*CLHEP::eV, 3.254*CLHEP::eV, 3.263*CLHEP::eV, 3.271*CLHEP::eV, 3.280*CLHEP::eV, 3.289*CLHEP::eV,
       3.297*CLHEP::eV, 3.306*CLHEP::eV, 3.315*CLHEP::eV, 3.324*CLHEP::eV, 3.333*CLHEP::eV, 3.342*CLHEP::eV, 3.351*CLHEP::eV, 
       3.360*CLHEP::eV, 3.369*CLHEP::eV, 3.378*CLHEP::eV, 3.388*CLHEP::eV, 3.397*CLHEP::eV, 3.406*CLHEP::eV, 3.416*CLHEP::eV, 
       3.425*CLHEP::eV, 3.434*CLHEP::eV, 3.444*CLHEP::eV, 3.454*CLHEP::eV, 3.463*CLHEP::eV, 3.473*CLHEP::eV, 3.483*CLHEP::eV, 
       3.493*CLHEP::eV, 3.502*CLHEP::eV, 3.512*CLHEP::eV, 3.522*CLHEP::eV, 3.532*CLHEP::eV, 3.542*CLHEP::eV, 3.553*CLHEP::eV, 
       3.563*CLHEP::eV, 3.573*CLHEP::eV, 3.583*CLHEP::eV, 3.594*CLHEP::eV, 3.604*CLHEP::eV, 3.615*CLHEP::eV, 3.625*CLHEP::eV, 
       3.636*CLHEP::eV, 3.647*CLHEP::eV, 3.657*CLHEP::eV, 3.668*CLHEP::eV, 3.679*CLHEP::eV, 3.690*CLHEP::eV, 3.701*CLHEP::eV, 
       3.712*CLHEP::eV, 3.723*CLHEP::eV, 3.734*CLHEP::eV, 3.746*CLHEP::eV, 3.757*CLHEP::eV, 3.769*CLHEP::eV, 3.780*CLHEP::eV, 
       3.792*CLHEP::eV, 3.803*CLHEP::eV, 3.815*CLHEP::eV, 3.827*CLHEP::eV, 3.839*CLHEP::eV, 3.850*CLHEP::eV, 3.862*CLHEP::eV, 
       3.875*CLHEP::eV, 3.887*CLHEP::eV, 3.899*CLHEP::eV, 3.911*CLHEP::eV, 3.924*CLHEP::eV, 3.936*CLHEP::eV, 3.949*CLHEP::eV, 
       3.961*CLHEP::eV, 3.974*CLHEP::eV, 3.987*CLHEP::eV, 3.999*CLHEP::eV, 4.012*CLHEP::eV, 4.025*CLHEP::eV, 4.039*CLHEP::eV, 
       4.052*CLHEP::eV, 4.065*CLHEP::eV, 4.078*CLHEP::eV, 4.092*CLHEP::eV, 4.105*CLHEP::eV, 4.119*CLHEP::eV, 4.133*CLHEP::eV, 
       4.147*CLHEP::eV, 4.161*CLHEP::eV, 4.175*CLHEP::eV, 4.189*CLHEP::eV, 4.203*CLHEP::eV };
  // TF: from http://refractiveindex.info/?shelf=organic&book=poly%28methyl_methacrylate%29&page=Szczurowski
  G4double RINDEX_skAcrylic[NUMENTRIES_acryl] =
    {1.4901, 1.4901, 1.4902, 1.4902, 1.4902, 1.4903, 1.4903, 1.4904, 1.4904, 1.4904, 
     1.4905, 1.4905, 1.4906, 1.4906, 1.4907, 1.4907, 1.4908, 1.4908, 1.4908, 1.4909, 
     1.4909, 1.4910, 1.4910, 1.4911, 1.4911, 1.4912, 1.4912, 1.4913, 1.4913, 1.4913, 
     1.4914, 1.4914, 1.4915, 1.4915, 1.4916, 1.4916, 1.4917, 1.4917, 1.4918, 1.4918, 
     1.4919, 1.4919, 1.4920, 1.4920, 1.4921, 1.4921, 1.4922, 1.4922, 1.4923, 1.4923, 
     1.4924, 1.4924, 1.4925, 1.4926, 1.4926, 1.4927, 1.4927, 1.4928, 1.4928, 1.4929, 
     1.4929, 1.4930, 1.4931, 1.4931, 1.4932, 1.4932, 1.4933, 1.4933, 1.4934, 1.4935, 
     1.4935, 1.4936, 1.4936, 1.4937, 1.4938, 1.4938, 1.4939, 1.4940, 1.4940, 1.4941, 
     1.4941, 1.4942, 1.4943, 1.4943, 1.4944, 1.4945, 1.4945, 1.4946, 1.4947, 1.4947, 
     1.4948, 1.4949, 1.4949, 1.4950, 1.4951, 1.4951, 1.4952, 1.4953, 1.4954, 1.4954, 
     1.4955, 1.4956, 1.4956, 1.4957, 1.4958, 1.4959, 1.4959, 1.4960, 1.4961, 1.4962, 
     1.4962, 1.4963, 1.4964, 1.4965, 1.4965, 1.4966, 1.4967, 1.4968, 1.4969, 1.4970, 
     1.4970, 1.4971, 1.4972, 1.4973, 1.4974, 1.4974, 1.4975, 1.4976, 1.4977, 1.4978, 
     1.4979, 1.4980, 1.4980, 1.4981, 1.4982, 1.4983, 1.4984, 1.4985, 1.4986, 1.4987, 
     1.4988, 1.4989, 1.4990, 1.4991, 1.4992, 1.4993, 1.4994, 1.4995, 1.4996, 1.4997, 
     1.4997, 1.4998, 1.5000, 1.5001, 1.5002, 1.5003, 1.5004, 1.5005, 1.5006, 1.5007, 
     1.5008, 1.5009, 1.5010, 1.5011, 1.5012, 1.5013, 1.5015, 1.5016, 1.5017, 1.5018, 
     1.5019, 1.5020, 1.5022, 1.5023, 1.5024, 1.5025, 1.5026, 1.5028, 1.5029, 1.5030, 
     1.5031, 1.5033, 1.5034, 1.5035, 1.5036, 1.5038, 1.5039, 1.5040, 1.5042, 1.5043, 
     1.5044, 1.5046, 1.5047, 1.5049, 1.5050, 1.5051, 1.5053, 1.5054, 1.5056, 1.5057, 
     1.5059, 1.5060, 1.5062, 1.5063, 1.5065, 1.5066, 1.5068, 1.5069, 1.5071, 1.5073, 
     1.5074, 1.5076, 1.5077, 1.5079, 1.5081, 1.5082, 1.5084, 1.5086, 1.5088, 1.5089, 
     1.5091, 1.5093, 1.5095, 1.5096, 1.5098, 1.5100, 1.5102, 1.5104, 1.5106, 1.5108, 
     1.5109, 1.5111, 1.5113, 1.5115, 1.5117, 1.5119, 1.5121, 1.5123, 1.5125, 1.5127, 
     1.5130, 1.5132, 1.5134, 1.5136, 1.5138, 1.5141, 1.5143, 1.5145, 1.5147, 1.5149, 
     1.5152, 1.5154, 1.5157, 1.5159, 1.5161, 1.5164, 1.5166, 1.5169, 1.5171, 1.5174, 
     1.5176, 1.5179, 1.5181, 1.5184, 1.5187, 1.5190, 1.5192, 1.5195, 1.5198, 1.5201, 
     1.5203, 1.5206, 1.5209, 1.5212, 1.5215, 1.5218, 1.5221, 1.5224, 1.5227, 1.5230, 
     1.5234, 1.5237, 1.5240, 1.5243, 1.5247, 1.5250, 1.5254, 1.5257, 1.5260, 1.5264, 
     1.5267, 1.5271, 1.5275, 1.5279, 1.5282, 1.5286, 1.5290, 1.5294, 1.5297, 1.5302, 
     1.5306, 1.5310, 1.5314, 1.5318, 1.5322, 1.5326};
  // Table from acrsg.F is abs coeff, so converted to abs length in m and inverted as
  // it was original a function of wavelength and now photon energy in increased energies.
  G4double ABSORPTION_skAcrylic[NUMENTRIES_acryl] =
    {3472.22*CLHEP::m, 3355.70*CLHEP::m, 3300.33*CLHEP::m, 3412.97*CLHEP::m, 3496.50*CLHEP::m, 3649.64*CLHEP::m, 3861.00*CLHEP::m, 3921.57*CLHEP::m, 3846.15*CLHEP::m, 3703.70*CLHEP::m, 
     3448.28*CLHEP::m, 3048.78*CLHEP::m, 2932.55*CLHEP::m, 2770.08*CLHEP::m, 2808.99*CLHEP::m, 2906.98*CLHEP::m, 2898.55*CLHEP::m, 2906.98*CLHEP::m, 3003.00*CLHEP::m, 3164.56*CLHEP::m, 
     3184.71*CLHEP::m, 3246.75*CLHEP::m, 3086.42*CLHEP::m, 2994.01*CLHEP::m, 2994.01*CLHEP::m, 2967.36*CLHEP::m, 3030.30*CLHEP::m, 3039.51*CLHEP::m, 3105.59*CLHEP::m, 2958.58*CLHEP::m, 
     3012.05*CLHEP::m, 3039.51*CLHEP::m, 2967.36*CLHEP::m, 2932.55*CLHEP::m, 2923.98*CLHEP::m, 2890.17*CLHEP::m, 2857.14*CLHEP::m, 2865.33*CLHEP::m, 2915.45*CLHEP::m, 2898.55*CLHEP::m, 
     2881.84*CLHEP::m, 2754.82*CLHEP::m, 2702.70*CLHEP::m, 2754.82*CLHEP::m, 2890.17*CLHEP::m, 2840.91*CLHEP::m, 2659.57*CLHEP::m, 2604.17*CLHEP::m, 2624.67*CLHEP::m, 2857.14*CLHEP::m, 
     2849.00*CLHEP::m, 2849.00*CLHEP::m, 2610.97*CLHEP::m, 2597.40*CLHEP::m, 2688.17*CLHEP::m, 2590.67*CLHEP::m, 2439.02*CLHEP::m, 2380.95*CLHEP::m, 2398.08*CLHEP::m, 2320.19*CLHEP::m, 
     2369.67*CLHEP::m, 2293.58*CLHEP::m, 2267.57*CLHEP::m, 2262.44*CLHEP::m, 2320.19*CLHEP::m, 2309.47*CLHEP::m, 2237.14*CLHEP::m, 2247.19*CLHEP::m, 2257.34*CLHEP::m, 2336.45*CLHEP::m, 
     2347.42*CLHEP::m, 2421.31*CLHEP::m, 2439.02*CLHEP::m, 2392.34*CLHEP::m, 2392.34*CLHEP::m, 2500.00*CLHEP::m, 2500.00*CLHEP::m, 2512.56*CLHEP::m, 2481.39*CLHEP::m, 2444.99*CLHEP::m, 
     2155.17*CLHEP::m, 2053.39*CLHEP::m, 2008.03*CLHEP::m, 2040.82*CLHEP::m, 1988.07*CLHEP::m, 2004.01*CLHEP::m, 1976.28*CLHEP::m, 1834.86*CLHEP::m, 1798.56*CLHEP::m, 1869.16*CLHEP::m, 
     1808.32*CLHEP::m, 1754.39*CLHEP::m, 1805.05*CLHEP::m, 1841.62*CLHEP::m, 1872.66*CLHEP::m, 1964.64*CLHEP::m, 1976.28*CLHEP::m, 1893.94*CLHEP::m, 1890.36*CLHEP::m, 1908.40*CLHEP::m, 
     1897.53*CLHEP::m, 1855.29*CLHEP::m, 1886.79*CLHEP::m, 1904.76*CLHEP::m, 1841.62*CLHEP::m, 1855.29*CLHEP::m, 1834.86*CLHEP::m, 1805.05*CLHEP::m, 1733.10*CLHEP::m, 1779.36*CLHEP::m, 
     1754.39*CLHEP::m, 1788.91*CLHEP::m, 1831.50*CLHEP::m, 1834.86*CLHEP::m, 1869.16*CLHEP::m, 1912.05*CLHEP::m, 2004.01*CLHEP::m, 1968.50*CLHEP::m, 1915.71*CLHEP::m, 1818.18*CLHEP::m, 
     1795.33*CLHEP::m, 1766.78*CLHEP::m, 1808.32*CLHEP::m, 1766.78*CLHEP::m, 1754.39*CLHEP::m, 1751.31*CLHEP::m, 1776.20*CLHEP::m, 1838.24*CLHEP::m, 1872.66*CLHEP::m, 1919.39*CLHEP::m, 
     1901.14*CLHEP::m, 1988.07*CLHEP::m, 1926.78*CLHEP::m, 1893.94*CLHEP::m, 1901.14*CLHEP::m, 1851.85*CLHEP::m, 1818.18*CLHEP::m, 1769.91*CLHEP::m, 1727.12*CLHEP::m, 1724.14*CLHEP::m, 
     1709.40*CLHEP::m, 1689.19*CLHEP::m, 1692.05*CLHEP::m, 1680.67*CLHEP::m, 1697.79*CLHEP::m, 1727.12*CLHEP::m, 1763.67*CLHEP::m, 1697.79*CLHEP::m, 1692.05*CLHEP::m, 1666.67*CLHEP::m, 
     1600.00*CLHEP::m, 1636.66*CLHEP::m, 1615.51*CLHEP::m, 1610.31*CLHEP::m, 1538.46*CLHEP::m, 1501.50*CLHEP::m, 1519.76*CLHEP::m, 1492.54*CLHEP::m, 1515.15*CLHEP::m, 1457.73*CLHEP::m, 
     1479.29*CLHEP::m, 1461.99*CLHEP::m, 1470.59*CLHEP::m, 1470.59*CLHEP::m, 1453.49*CLHEP::m, 1512.86*CLHEP::m, 1522.07*CLHEP::m, 1589.83*CLHEP::m, 1577.29*CLHEP::m, 1597.44*CLHEP::m, 
     1600.00*CLHEP::m, 1672.24*CLHEP::m, 1663.89*CLHEP::m, 1647.45*CLHEP::m, 1661.13*CLHEP::m, 1607.72*CLHEP::m, 1543.21*CLHEP::m, 1488.10*CLHEP::m, 1459.85*CLHEP::m, 1373.63*CLHEP::m, 
     1326.26*CLHEP::m, 1373.63*CLHEP::m, 1410.44*CLHEP::m, 1406.47*CLHEP::m, 1396.65*CLHEP::m, 1308.90*CLHEP::m, 1328.02*CLHEP::m, 1386.96*CLHEP::m, 1347.71*CLHEP::m, 1297.02*CLHEP::m, 
     1262.63*CLHEP::m, 1234.57*CLHEP::m, 1283.70*CLHEP::m, 1319.26*CLHEP::m, 1310.62*CLHEP::m, 1319.26*CLHEP::m, 1402.52*CLHEP::m, 1459.85*CLHEP::m, 1464.13*CLHEP::m, 1472.75*CLHEP::m, 
     1420.45*CLHEP::m, 1432.66*CLHEP::m, 1400.56*CLHEP::m, 1362.40*CLHEP::m, 1358.70*CLHEP::m, 1358.70*CLHEP::m, 1379.31*CLHEP::m, 1360.54*CLHEP::m, 1362.40*CLHEP::m, 1355.01*CLHEP::m, 
     1342.28*CLHEP::m, 1367.99*CLHEP::m, 1293.66*CLHEP::m, 1221.00*CLHEP::m, 1197.60*CLHEP::m, 1165.50*CLHEP::m, 1119.82*CLHEP::m, 1107.42*CLHEP::m, 1108.65*CLHEP::m, 1040.58*CLHEP::m, 
     1033.06*CLHEP::m, 1000.00*CLHEP::m, 952.38*CLHEP::m, 925.93*CLHEP::m, 909.09*CLHEP::m, 869.57*CLHEP::m, 800.00*CLHEP::m, 781.25*CLHEP::m, 769.23*CLHEP::m, 729.93*CLHEP::m, 735.29*CLHEP::m, 
     729.93*CLHEP::m, 709.22*CLHEP::m, 684.93*CLHEP::m, 657.89*CLHEP::m, 645.16*CLHEP::m, 625.00*CLHEP::m, 581.40*CLHEP::m, 568.18*CLHEP::m, 558.66*CLHEP::m, 543.48*CLHEP::m, 540.54*CLHEP::m, 523.56*CLHEP::m, 
     505.05*CLHEP::m, 490.20*CLHEP::m, 485.44*CLHEP::m, 462.96*CLHEP::m, 450.45*CLHEP::m, 442.48*CLHEP::m, 429.18*CLHEP::m, 420.17*CLHEP::m, 411.52*CLHEP::m, 411.52*CLHEP::m, 393.70*CLHEP::m, 393.70*CLHEP::m, 
     386.10*CLHEP::m, 381.68*CLHEP::m, 355.87*CLHEP::m, 343.64*CLHEP::m, 331.13*CLHEP::m, 343.64*CLHEP::m, 338.98*CLHEP::m, 335.57*CLHEP::m, 326.80*CLHEP::m, 313.48*CLHEP::m, 303.95*CLHEP::m, 294.12*CLHEP::m, 
     292.40*CLHEP::m, 280.90*CLHEP::m, 273.97*CLHEP::m, 270.27*CLHEP::m, 265.25*CLHEP::m, 261.78*CLHEP::m, 256.41*CLHEP::m, 250.00*CLHEP::m, 242.13*CLHEP::m, 237.53*CLHEP::m, 233.10*CLHEP::m, 228.83*CLHEP::m, 
     226.76*CLHEP::m, 223.21*CLHEP::m, 219.30*CLHEP::m, 215.05*CLHEP::m, 211.86*CLHEP::m, 208.77*CLHEP::m, 206.61*CLHEP::m, 201.21*CLHEP::m, 196.85*CLHEP::m, 193.42*CLHEP::m, 189.75*CLHEP::m, 186.22*CLHEP::m, 
     182.15*CLHEP::m, 175.75*CLHEP::m, 168.92*CLHEP::m, 162.60*CLHEP::m, 154.80*CLHEP::m, 144.51*CLHEP::m, 133.51*CLHEP::m, 120.19*CLHEP::m, 105.93*CLHEP::m, 90.91*CLHEP::m, 75.76*CLHEP::m, 61.35*CLHEP::m, 
     48.54*CLHEP::m, 37.88*CLHEP::m, 29.41*CLHEP::m};
  acrMPT->AddProperty("RINDEX", ENERGY_skAcrylic, RINDEX_skAcrylic, NUMENTRIES_acryl );
  acrMPT->AddProperty("ABSLENGTH", ENERGY_skAcrylic, ABSORPTION_skAcrylic, NUMENTRIES_acryl );
  fMaterials[ "acrylic" ]->SetMaterialPropertiesTable( acrMPT );

  
  G4double a= 12.01*CLHEP::g/CLHEP::mole;
  G4Element* elC = new G4Element("Carbon","C", 6,a);
  a = 1.01*CLHEP::g/ CLHEP::mole;
  G4Element* elH = new G4Element("Hydrogen","H", 1,a);

  
 //---Black sheet
  G4double density = 0.95*CLHEP::g/CLHEP::cm3;
  G4Material* Blacksheet
    = new G4Material("Blacksheet",density,2);
  Blacksheet->AddElement(elC, 1);
  Blacksheet->AddElement(elH, 2);
  fMaterials[ "blacksheet" ] = Blacksheet;

  std::cout<<"Finished blacksheet defn"<<std::endl;
  
  //---Tyvek - jl145
  density = 0.38*CLHEP::g/CLHEP::cm3;  //cf. DuPont product handbook
  G4Material* Tyvek = new G4Material("Tyvek",density,2);
  Tyvek->AddElement(elC, 1);  //polyethylene
  Tyvek->AddElement(elH, 2);
  fMaterials[ "tyvek" ] = Tyvek;
  
  std::cout<<"Finished tyvek defn"<<std::endl;

  
  
  fMaterials[ "pmtsteel" ] = nist->FindOrBuildMaterial( "G4_STAINLESS-STEEL" );
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();   
  const G4int nEntries = 2;
  G4double photonEnergy[nEntries] = {1.*CLHEP::eV , 7.*CLHEP::eV};
  G4double abslength_Steel[nEntries] = {.001*CLHEP::mm , .001*CLHEP::mm};
  mpt->AddProperty("ABSLENGTH", photonEnergy, abslength_Steel, nEntries);
  fMaterials[ "pmtsteel" ]->SetMaterialPropertiesTable(mpt);
 std::cout<<"Finished pmtsteel defn"<<std::endl;

  
  fMaterials[ "Al" ] = nist->FindOrBuildMaterial( "G4_Al" );
  G4MaterialPropertiesTable *mptal = new G4MaterialPropertiesTable();   
  mptal->AddProperty("ABSLENGTH", photonEnergy, abslength_Steel, nEntries);
  fMaterials[ "Al" ]->SetMaterialPropertiesTable(mptal);
 std::cout<<"Finished Al defn"<<std::endl;
  
  fMaterials[ "water" ] = nist->FindOrBuildMaterial( "G4_WATER" );
  
   //From SKDETSIM water absorption
   const G4int NUMENTRIES_water=60;

   G4double ENERGY_water[NUMENTRIES_water] =
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

   // M Fechner : new ; define the water refraction index using refsg.F 
   //from skdetsim using the whole range.   
    G4double RINDEX1[NUMENTRIES_water] = 
     {1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014, 
      1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
      1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
      1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
      1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
      1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
      1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
      1.38362, 1.39074, 1.39956, 1.41075, 1.42535};
   

    //G4double ABWFF = 1.0;

    // Get from the tuning parameters
    //ABWFF = WCSimTuningParams->GetAbwff();

    //T. Akiri: Values from Skdetsim 
    G4double ABSORPTION_water[NUMENTRIES_water] =
      {
        16.1419*CLHEP::cm*water_ABWFF,  18.278*CLHEP::cm*water_ABWFF, 21.0657*CLHEP::cm*water_ABWFF, 24.8568*CLHEP::cm*water_ABWFF, 30.3117*CLHEP::cm*water_ABWFF, 
	38.8341*CLHEP::cm*water_ABWFF, 54.0231*CLHEP::cm*water_ABWFF, 81.2306*CLHEP::cm*water_ABWFF, 120.909*CLHEP::cm*water_ABWFF, 160.238*CLHEP::cm*water_ABWFF, 
	193.771*CLHEP::cm*water_ABWFF, 215.017*CLHEP::cm*water_ABWFF, 227.747*CLHEP::cm*water_ABWFF,  243.85*CLHEP::cm*water_ABWFF, 294.036*CLHEP::cm*water_ABWFF, 
	321.647*CLHEP::cm*water_ABWFF,  342.81*CLHEP::cm*water_ABWFF, 362.827*CLHEP::cm*water_ABWFF, 378.041*CLHEP::cm*water_ABWFF, 449.378*CLHEP::cm*water_ABWFF,
        739.434*CLHEP::cm*water_ABWFF, 1114.23*CLHEP::cm*water_ABWFF, 1435.56*CLHEP::cm*water_ABWFF, 1611.06*CLHEP::cm*water_ABWFF, 1764.18*CLHEP::cm*water_ABWFF, 
	2100.95*CLHEP::cm*water_ABWFF,  2292.9*CLHEP::cm*water_ABWFF, 2431.33*CLHEP::cm*water_ABWFF,  3053.6*CLHEP::cm*water_ABWFF, 4838.23*CLHEP::cm*water_ABWFF, 
	6539.65*CLHEP::cm*water_ABWFF, 7682.63*CLHEP::cm*water_ABWFF, 9137.28*CLHEP::cm*water_ABWFF, 12220.9*CLHEP::cm*water_ABWFF, 15270.7*CLHEP::cm*water_ABWFF, 
	19051.5*CLHEP::cm*water_ABWFF, 23671.3*CLHEP::cm*water_ABWFF, 29191.1*CLHEP::cm*water_ABWFF, 35567.9*CLHEP::cm*water_ABWFF,   42583*CLHEP::cm*water_ABWFF,
        49779.6*CLHEP::cm*water_ABWFF, 56465.3*CLHEP::cm*water_ABWFF,   61830*CLHEP::cm*water_ABWFF, 65174.6*CLHEP::cm*water_ABWFF, 66143.7*CLHEP::cm*water_ABWFF,   
	  64820*CLHEP::cm*water_ABWFF,   61635*CLHEP::cm*water_ABWFF, 57176.2*CLHEP::cm*water_ABWFF, 52012.1*CLHEP::cm*water_ABWFF, 46595.7*CLHEP::cm*water_ABWFF, 
	41242.1*CLHEP::cm*water_ABWFF, 36146.3*CLHEP::cm*water_ABWFF, 31415.4*CLHEP::cm*water_ABWFF, 27097.8*CLHEP::cm*water_ABWFF, 23205.7*CLHEP::cm*water_ABWFF, 
	19730.3*CLHEP::cm*water_ABWFF, 16651.6*CLHEP::cm*water_ABWFF, 13943.6*CLHEP::cm*water_ABWFF, 11578.1*CLHEP::cm*water_ABWFF, 9526.13*CLHEP::cm*water_ABWFF
      };

   // M Fechner: Rayleigh scattering -- as of version 4.6.2 of GEANT,
   // one may use one's own Rayleigh scattering lengths (the buffer is no
   // longer overwritten for "water", see 4.6.2 release notes)

   // RAYFF = 1/ARAS, for those of you who know SKdetsim...
   // actually that's not quite right because the scattering models
   // are different; in G4 there is no scattering depolarization
   // std value at SK = 0.6. But Mie scattering is implemented
   // in SKdetsim and not in G4

   
  // april 2005 : reduced reflections, let's increase scattering...
  // sep 09: for the large detector like superK the old values are muc better
   //G4double RAYFF = 1.0/1.65;  //old
   //    G4double RAYFF = 1.0/1.5;  



   // Get from the tuning parameters
   //RAYFF = WCSimTuningParams->GetRayff();
   //    printf("RAYFF: %f\n",RAYFF);

   //T. Akiri: Values from Skdetsim 
   G4double RAYLEIGH_water[NUMENTRIES_water] = {
      386929*CLHEP::cm*water_RAYFF,  366249*CLHEP::cm*water_RAYFF,  346398*CLHEP::cm*water_RAYFF,  327355*CLHEP::cm*water_RAYFF,  309097*CLHEP::cm*water_RAYFF,  
      291603*CLHEP::cm*water_RAYFF,  274853*CLHEP::cm*water_RAYFF,  258825*CLHEP::cm*water_RAYFF,  243500*CLHEP::cm*water_RAYFF,  228856*CLHEP::cm*water_RAYFF,  
      214873*CLHEP::cm*water_RAYFF,  201533*CLHEP::cm*water_RAYFF,  188816*CLHEP::cm*water_RAYFF,  176702*CLHEP::cm*water_RAYFF,  165173*CLHEP::cm*water_RAYFF,
      154210*CLHEP::cm*water_RAYFF,  143795*CLHEP::cm*water_RAYFF,  133910*CLHEP::cm*water_RAYFF,  124537*CLHEP::cm*water_RAYFF,  115659*CLHEP::cm*water_RAYFF,  
      107258*CLHEP::cm*water_RAYFF, 99318.2*CLHEP::cm*water_RAYFF, 91822.2*CLHEP::cm*water_RAYFF,   84754*CLHEP::cm*water_RAYFF, 78097.3*CLHEP::cm*water_RAYFF, 
     71836.5*CLHEP::cm*water_RAYFF,   65956*CLHEP::cm*water_RAYFF, 60440.6*CLHEP::cm*water_RAYFF, 55275.4*CLHEP::cm*water_RAYFF, 50445.6*CLHEP::cm*water_RAYFF,
       45937*CLHEP::cm*water_RAYFF, 41735.2*CLHEP::cm*water_RAYFF, 37826.6*CLHEP::cm*water_RAYFF, 34197.6*CLHEP::cm*water_RAYFF, 30834.9*CLHEP::cm*water_RAYFF, 
     27725.4*CLHEP::cm*water_RAYFF, 24856.6*CLHEP::cm*water_RAYFF, 22215.9*CLHEP::cm*water_RAYFF, 19791.3*CLHEP::cm*water_RAYFF, 17570.9*CLHEP::cm*water_RAYFF,   
       15543*CLHEP::cm*water_RAYFF, 13696.6*CLHEP::cm*water_RAYFF, 12020.5*CLHEP::cm*water_RAYFF, 10504.1*CLHEP::cm*water_RAYFF, 9137.15*CLHEP::cm*water_RAYFF,
     7909.45*CLHEP::cm*water_RAYFF,  6811.3*CLHEP::cm*water_RAYFF, 5833.25*CLHEP::cm*water_RAYFF,  4966.2*CLHEP::cm*water_RAYFF, 4201.36*CLHEP::cm*water_RAYFF, 
     3530.28*CLHEP::cm*water_RAYFF, 2944.84*CLHEP::cm*water_RAYFF, 2437.28*CLHEP::cm*water_RAYFF, 2000.18*CLHEP::cm*water_RAYFF,  1626.5*CLHEP::cm*water_RAYFF, 
     1309.55*CLHEP::cm*water_RAYFF, 1043.03*CLHEP::cm*water_RAYFF, 821.016*CLHEP::cm*water_RAYFF,  637.97*CLHEP::cm*water_RAYFF, 488.754*CLHEP::cm*water_RAYFF
   };


   // Get from the tuning parameters
   G4double MIEFF = 0.0; // mie scattering turned off

   //Values extracted from Skdetsim
   G4double MIE_water[NUMENTRIES_water] = {
     7790020*CLHEP::cm*MIEFF, 7403010*CLHEP::cm*MIEFF, 7030610*CLHEP::cm*MIEFF, 6672440*CLHEP::cm*MIEFF, 6328120*CLHEP::cm*MIEFF, 
     5997320*CLHEP::cm*MIEFF, 5679650*CLHEP::cm*MIEFF, 5374770*CLHEP::cm*MIEFF, 5082340*CLHEP::cm*MIEFF, 4802000*CLHEP::cm*MIEFF, 
     4533420*CLHEP::cm*MIEFF, 4276280*CLHEP::cm*MIEFF, 4030220*CLHEP::cm*MIEFF, 3794950*CLHEP::cm*MIEFF, 3570120*CLHEP::cm*MIEFF,
     3355440*CLHEP::cm*MIEFF, 3150590*CLHEP::cm*MIEFF, 2955270*CLHEP::cm*MIEFF, 2769170*CLHEP::cm*MIEFF, 2592000*CLHEP::cm*MIEFF, 
     2423470*CLHEP::cm*MIEFF, 2263300*CLHEP::cm*MIEFF, 2111200*CLHEP::cm*MIEFF, 1966900*CLHEP::cm*MIEFF, 1830120*CLHEP::cm*MIEFF, 
     1700610*CLHEP::cm*MIEFF, 1578100*CLHEP::cm*MIEFF, 1462320*CLHEP::cm*MIEFF, 1353040*CLHEP::cm*MIEFF, 1250000*CLHEP::cm*MIEFF,
     1152960*CLHEP::cm*MIEFF, 1061680*CLHEP::cm*MIEFF,  975936*CLHEP::cm*MIEFF,  895491*CLHEP::cm*MIEFF,  820125*CLHEP::cm*MIEFF, 
      749619*CLHEP::cm*MIEFF,  683760*CLHEP::cm*MIEFF,  622339*CLHEP::cm*MIEFF,  565152*CLHEP::cm*MIEFF,  512000*CLHEP::cm*MIEFF, 
      462688*CLHEP::cm*MIEFF,  417027*CLHEP::cm*MIEFF,  374832*CLHEP::cm*MIEFF,  335923*CLHEP::cm*MIEFF,  300125*CLHEP::cm*MIEFF,
      267267*CLHEP::cm*MIEFF,  237184*CLHEP::cm*MIEFF,  209715*CLHEP::cm*MIEFF,  184704*CLHEP::cm*MIEFF,  162000*CLHEP::cm*MIEFF, 
      141456*CLHEP::cm*MIEFF,  122931*CLHEP::cm*MIEFF,  106288*CLHEP::cm*MIEFF, 91395.2*CLHEP::cm*MIEFF,   78125*CLHEP::cm*MIEFF, 
     66355.2*CLHEP::cm*MIEFF, 55968.2*CLHEP::cm*MIEFF, 46851.2*CLHEP::cm*MIEFF, 38896.2*CLHEP::cm*MIEFF,   32000*CLHEP::cm*MIEFF
   };

   G4double MIE_water_const[3]={0.4,0.,1};// gforward, gbackward, forward backward ratio

   G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
   myMPT1->AddProperty("RINDEX",   ENERGY_water, RINDEX1,          NUMENTRIES_water);
   myMPT1->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
   myMPT1->AddProperty("RAYLEIGH", ENERGY_water, RAYLEIGH_water,   NUMENTRIES_water);
   //myMPT1->AddProperty("MIEHG",    ENERGY_water, MIE_water,        NUMENTRIES_water);
   //myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_water_const[0]);
   //myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_water_const[1]);
   //myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_water_const[2]);
   fMaterials[ "water" ]->SetMaterialPropertiesTable(myMPT1);
  
   fMaterials[ "H" ] = nist->FindOrBuildMaterial( "G4_H" );
   fMaterials[ "air" ] = nist->FindOrBuildMaterial( "G4_AIR" );
   // tried to pick reasonable numbers... numbers in WCSim were suspect
   G4double RINDEX_air[NUMENTRIES_water] = 
     { 1.000275, 1.000275, 1.000275, 1.000275,
       1.000275, 1.000275, 1.000275, 1.000275,
       1.000276, 1.000276, 1.000276, 1.000276,
       1.000276, 1.000276, 1.000276, 1.000276,
       1.000277, 1.000277, 1.000277, 1.000277,
       1.000277, 1.000277, 1.000277, 1.000277,
       1.000278, 1.000278, 1.000278, 1.000278,
       1.000279, 1.000279, 1.000279, 1.000279,
       1.000280, 1.000280, 1.000280, 1.000280,
       1.000281, 1.000281, 1.000281, 1.000281,
       1.000282, 1.000283, 1.000284, 1.000285,
       1.000286, 1.000287, 1.000288, 1.000290,
       1.000291, 1.000292, 1.000293, 1.000295,
       1.000296, 1.000299, 1.000301, 1.000304,
       1.000308, 1.000312, 1.000316, 1.000320};
   G4double ABSLEN_air[NUMENTRIES_water] =
     { 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m};
   G4double RAYLEIGH_air[NUMENTRIES_water] =
     { 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 
       10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m, 10000.0*CLHEP::m};
   G4double MIE_air[NUMENTRIES_water] =
     { 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 
       1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m, 1000.0*CLHEP::m};
   G4double MIE_air_const[3]={0.99,0.99,0.8};// gforward, gbackward, forward backward ratio
   G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
   myMPT2->AddProperty("RINDEX",    ENERGY_water, RINDEX_air,          NUMENTRIES_water);
   myMPT2->AddProperty("ABSLENGTH",  ENERGY_water, ABSLEN_air,         NUMENTRIES_water);
   myMPT2->AddProperty("RAYLEIGH",  ENERGY_water, RAYLEIGH_air,        NUMENTRIES_water);
   //myMPT2->AddProperty("MIEHG",     ENERGY_water, MIE_air,             NUMENTRIES_water);
   //myMPT2->AddConstProperty("MIEHG_FORWARD",MIE_air_const[0]);
   //myMPT2->AddConstProperty("MIEHG_BACKWARD",MIE_air_const[1]);
   //myMPT2->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_air_const[2]);
   fMaterials[ "air" ]->SetMaterialPropertiesTable(myMPT2);
   
   a = 118.71 * CLHEP::g/CLHEP::mole;
   G4Element* elSb = new G4Element("Tin","Sn",50, a);
   a = 39.0983 * CLHEP::g/CLHEP::mole;
   G4Element* elK = new G4Element("Potassium","K",19, a);
   a = 132.90545 * CLHEP::g/CLHEP::mole;
   G4Element* elCs = new G4Element("Caesium","Cs",55, a);
   density = ( 1.93 + 7.31 + 0.862 ) / 3.0 * CLHEP::g / CLHEP::cm3;
   G4Material* SbKCs = new G4Material("SbKCs", density, 3 );
   SbKCs->AddElement( elSb, 1 );
   SbKCs->AddElement( elK,  1 );
   SbKCs->AddElement( elCs, 1 );
   fMaterials[ "cathode" ] = SbKCs;
   const G4int NUMENTRIES_cathode =  21;
   G4double ENERGY_cathode[NUMENTRIES_cathode];
   G4double RINDEX_cathode[NUMENTRIES_cathode];
   G4double ABSLENGTH_cathode[NUMENTRIES_cathode];
   G4double EFFICIENCY_cathode[NUMENTRIES_cathode];
   G4double lambda_cathode[NUMENTRIES_cathode] = {
     380,      395 ,     410 ,     425 ,     440 ,
     455 ,     470 ,     485 ,     500 ,     515 ,
     530 ,     545 ,     560 ,     575 ,     590 ,
     605 ,     620 ,     635 ,     650 ,     665 ,     680 };
   G4double n_cathode[NUMENTRIES_cathode] = {
     1.92,     2.18,     2.38,     2.61,     2.70,
     2.87,     3.00,     3.00,     3.00,     3.09,
     3.26,     3.20,     3.12,     3.06,     3.01,
     2.98,     2.96,     2.95,     2.95,     2.95,     2.96 };
   G4double k_cathode[NUMENTRIES_cathode] = {
     1.69, 1.69, 1.71, 1.53, 1.50,
     1.44, 1.34, 1.11, 1.06, 1.05,
     0.86, 0.63, 0.53, 0.46, 0.42,
     0.38, 0.37, 0.35, 0.34, 0.34, 0.33 };
   // convert k to absorption length a:
   // a = lambda / ( 4*pi*k )
   //double scale_abs_length = 6.0;//1000.0/30.0;
   for ( unsigned i=0; i<NUMENTRIES_cathode; ++i ){
     ENERGY_cathode[NUMENTRIES_cathode-i-1] = 1239.8 / lambda_cathode[i] * CLHEP::eV;
     RINDEX_cathode[NUMENTRIES_cathode-i-1] = n_cathode[i];
     ABSLENGTH_cathode[NUMENTRIES_cathode-i-1] = lambda_cathode[i] / ( 4.0*CLHEP::pi*k_cathode[i] ) * cathode_ABWFF * CLHEP::nm;
     EFFICIENCY_cathode[i] = 1.0;
     std::cout<<"Cathode photon E="<<ENERGY_cathode[NUMENTRIES_cathode-i-1]/CLHEP::eV<<" eV "<<" Abs-length="<<ABSLENGTH_cathode[NUMENTRIES_cathode-i-1]/CLHEP::nm<<" nm"<<std::endl;
   }   
   G4MaterialPropertiesTable *myMPTcath = new G4MaterialPropertiesTable();
   myMPTcath->AddProperty("RINDEX",   ENERGY_cathode, RINDEX_cathode,     NUMENTRIES_cathode);
   myMPTcath->AddProperty("ABSLENGTH",ENERGY_cathode, ABSLENGTH_cathode,  NUMENTRIES_cathode);
   myMPTcath->AddProperty("EFFICIENCY",ENERGY_cathode, EFFICIENCY_cathode,  NUMENTRIES_cathode);
   fMaterials[ "cathode" ]->SetMaterialPropertiesTable(myMPTcath);
   
   //---Glass
   a = 28.09*CLHEP::g/CLHEP::mole;
   G4Element* elSi = new G4Element("Silicon", "Si", 14., a); 
   a = 16.00*CLHEP::g/CLHEP::mole;
   G4Element* elO = new G4Element("Oxygen","O", 8,a);
   a = 26.98*CLHEP::g/CLHEP::mole;
   G4Element* elAl = new G4Element("Aluminum", "Al", 13, a);  
   density = 2.20*CLHEP::g/CLHEP::cm3;
   G4Material* SiO2 = new G4Material("SiO2",density,2);
   SiO2->AddElement(elSi, 1);
   SiO2->AddElement(elO , 2);
   a = 10.81*CLHEP::g/CLHEP::mole;
   G4Element* elB = new G4Element("Boron", "B", 5, a);  
   density = 2.46*CLHEP::g/CLHEP::cm3;
   G4Material* B2O3 = new G4Material("B2O3",density,2);
   B2O3->AddElement(elB, 2);
   B2O3->AddElement(elO, 3);
   a = 22.99*CLHEP::g/CLHEP::mole;
   G4Element* elNa = new G4Element("Sodium", "Na", 11, a);  
   density = 2.27*CLHEP::g/CLHEP::cm3;
   G4Material* Na2O = new G4Material("Na2O",density,2);
   Na2O->AddElement(elNa, 2);
   Na2O->AddElement(elO, 1);
   density = 4.00*CLHEP::g/CLHEP::cm3;
   G4Material* Al2O3 = new G4Material("Al2O3",density,2);
   Al2O3->AddElement(elAl, 2);
   Al2O3->AddElement(elO, 3);
   density = 2.23*CLHEP::g/CLHEP::cm3;
   G4Material* Glass  = new G4Material("Glass",density,4);
   Glass->AddMaterial(SiO2, 80.6*CLHEP::perCent);
   Glass->AddMaterial(B2O3, 13.0*CLHEP::perCent);
   Glass->AddMaterial(Na2O, 4.0*CLHEP::perCent);
   Glass->AddMaterial(Al2O3, 2.4*CLHEP::perCent);
   fMaterials[ "pmtglass" ] = Glass;
   // Based on analytical formula from http://refractiveindex.info/?shelf=glass&book=HIKARI-BK&page=J-BK7A for Borosilicate glass 
   G4double RINDEX_glass[NUMENTRIES_water] =
     { 1.5110, 1.5112, 1.5114, 1.5116, 1.5118, 
       1.5121, 1.5123, 1.5126, 1.5128, 1.5131, 
       1.5133, 1.5136, 1.5139, 1.5142, 1.5145, 
       1.5148, 1.5152, 1.5155, 1.5159, 1.5163, 
       1.5167, 1.5171, 1.5176, 1.5180, 1.5185, 
       1.5190, 1.5196, 1.5202, 1.5208, 1.5214, 
       1.5221, 1.5228, 1.5236, 1.5244, 1.5253, 
       1.5263, 1.5273, 1.5284, 1.5296, 1.5309, 
       1.5323, 1.5338, 1.5354, 1.5372, 1.5392, 
       1.5414, 1.5438, 1.5465, 1.5495, 1.5528, 
       1.5566, 1.5608, 1.5657, 1.5713, 1.5777, 
       1.5853, 1.5941, 1.6047, 1.6173, 1.6328 }; 
   
   G4double ABSORPTION_glass[NUMENTRIES_water]= 
     { 1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,
       1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF,1.0e3*CLHEP::cm*glass_ABWFF, 1.0e3*CLHEP::cm*glass_ABWFF };

   G4MaterialPropertiesTable *myMPT5 = new G4MaterialPropertiesTable();
   myMPT5->AddProperty("RINDEX",   ENERGY_water, RINDEX_glass,     NUMENTRIES_water);
   myMPT5->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_glass, NUMENTRIES_water);
   fMaterials[ "pmtglass" ]->SetMaterialPropertiesTable(myMPT5);

   // add an frp (fibre reinforced plastic) that is used in PMT cover
   // define it as a 50/50 mix of glass fiber with plastic
   fMaterials[ "glass" ] = nist->FindOrBuildMaterial( "G4_GLASS_PLATE" );
   fMaterials[ "polycarbonate" ] = nist->FindOrBuildMaterial( "G4_POLYCARBONATE" );
   density = 1.3 * CLHEP::g / CLHEP::cm3;
   fMaterials[ "frp" ] = new G4Material( "frp", density, 2 );
   fMaterials[ "frp" ]->AddMaterial( fMaterials[ "glass" ], 50.0 * CLHEP::perCent );
   fMaterials[ "frp" ]->AddMaterial( fMaterials[ "polycarbonate" ], 50.0 * CLHEP::perCent );


   // aerogel transmission data from Tabata-san, Chiba University
   // read in from data file as TGraph
   // data is in form transmittance (%) vs wavelength (nm) through 20 mm
   TGraph * A1030tg = new TGraph("data/leps6-1a-10297.dx");  
   TGraph * A1035tg = new TGraph("data/leps2-1a-10352.dx");
   TGraph * A1040tg = new TGraph("data/hds2-3b-10401.dx");
   TGraph * A1045tg = new TGraph("data/btr4-1a-10452.dx");
   TGraph * A1050tg = new TGraph("data/leps1-1b-10505.dx");

   G4Material* H2O =nist->FindOrBuildMaterial( "G4_WATER" );
   const int nAerogelEntries = 200; 
   // define optical properties from 1.9 to 6.2 eV
   double Aerogel_E[nAerogelEntries];
     
   /// Add aerogels   
   // density is rho = 4*(n-1) g/cm^3 [ref: M.Tabata, et. al. "Development of Silica Aerogel with Any Density," IEEE 2005 NSSCR.
   // absorption length is via rayleigh scattering using data from Tabata-san read into TGraph
    // index of refraction is given with a0 = 0.05639, lambda0 = 83.22 nm [ ref. T. Bellunato, et. al. "Refractive index dispersion law of silica aerogel," Phys. Rev. C 52 (2007) 759.
   //  n = sqrt( 1 + a0 * lambda^2 / ( lambda^2 - lambda0^2 ) )
 

   //******************************************************************************************************************************
   // double A103_Abs[nAerogelEntries]; // absorption length
   double A103_Ray[nAerogelEntries]; // rayleigh scattering length
   double A103_n[nAerogelEntries];   // index of refraction
   for ( int i=0; i< nAerogelEntries; ++i ){
     Aerogel_E[ i ] = ( 1.9 + i * (6.2-1.9) / nAerogelEntries )  * CLHEP::eV;
     double lambda = 1239.8 / Aerogel_E[i] * CLHEP::nm;
     double trans20mm = A1030tg->Eval( lambda / CLHEP::nm ) / 100.0 ; // divide by 100 to convert % to fraction
     // get Rayleigh scattering length from transmission through 20 mm
     if ( trans20mm >= 1.0 ) { //100% transmission - set long scatter length
       A103_Ray[i] = 100.0 * CLHEP::km;
     } else if ( trans20mm <= 0.0 ) { // 0% tramission - set tiny scatter length
       A103_Ray[i] = 1.0 * CLHEP::nm;
     } else {
       A103_Ray[i] = -20.0 * CLHEP::mm / std::log( trans20mm );
     }
     A103_n[i]   = sqrt( 1.0 + 0.05639*pow(lambda/CLHEP::nm,2) / ( pow(lambda/CLHEP::nm,2) - pow(83.22,2) ) );
     std::cout<<"aerogel103 E["<<i<<"] = "<<Aerogel_E[i]<<"  n="<<A103_n[i]<<std::endl;
   }
   density = 4*(1.03-1.0) * CLHEP::g/ CLHEP::cm3;
   int ncomp;
   double fracMass;
   G4Material* aerogel103 = new G4Material ("aerogel103", density, ncomp=3);
   aerogel103->AddElement (elC, fracMass = 0.1*CLHEP::perCent);
   aerogel103->AddMaterial (SiO2, fracMass = 62.5*CLHEP::perCent);
   aerogel103->AddMaterial (H2O, fracMass = 37.4*CLHEP::perCent);
   fMaterials[ "aerogel103" ] = aerogel103;
   G4MaterialPropertiesTable *mptA103 = new G4MaterialPropertiesTable();
   mptA103->AddProperty("RINDEX",   Aerogel_E, A103_n,   nAerogelEntries );
   //mptA103->AddProperty("ABSLENGTH",Aerogel_E, A103_Abs, nAerogelEntries );
   mptA103->AddProperty("RAYLEIGH", Aerogel_E, A103_Ray, nAerogelEntries );
   fMaterials[ "aerogel103" ]->SetMaterialPropertiesTable( mptA103 );

   //******************************************************************************************************************************
   // double A1035_Abs[nAerogelEntries]; // absorption length
   double A1035_Ray[nAerogelEntries]; // rayleigh scattering length
   double A1035_n[nAerogelEntries];   // index of refraction
   for ( int i=0; i< nAerogelEntries; ++i ){
     Aerogel_E[ i ] = ( 1.9 + i * (6.2-1.9) / nAerogelEntries )  * CLHEP::eV;
     double lambda = 1239.8 / Aerogel_E[i] * CLHEP::nm;
     double trans20mm = A1035tg->Eval( lambda / CLHEP::nm ) / 100.0 ; // divide by 100 to convert % to fraction
     // get Rayleigh scattering length from transmission through 20 mm
     if ( trans20mm >= 1.0 ) { //100% transmission - set long scatter length
       A1035_Ray[i] = 100.0 * CLHEP::km;
     } else if ( trans20mm <= 0.0 ) { // 0% tramission - set tiny scatter length
       A1035_Ray[i] = 1.0 * CLHEP::nm;
     } else {
       A1035_Ray[i] = -20.0 * CLHEP::mm / std::log( trans20mm );
     }
     A1035_n[i]   = sqrt( 1.0 + 0.05639*pow(lambda/CLHEP::nm,2) / ( pow(lambda/CLHEP::nm,2) - pow(83.22,2) ) ) * 1.035/1.030;
     std::cout<<"aerogel1035 E["<<i<<"] = "<<Aerogel_E[i]<<"  n="<<A1035_n[i]<<std::endl;
   }
   density = 4*(1.035-1.0) * CLHEP::g/ CLHEP::cm3;
   G4Material* aerogel1035 = new G4Material ("aerogel1035", density, ncomp=3);
   aerogel1035->AddElement (elC, fracMass = 0.1*CLHEP::perCent);
   aerogel1035->AddMaterial (SiO2, fracMass = 62.5*CLHEP::perCent);
   aerogel1035->AddMaterial (H2O, fracMass = 37.4*CLHEP::perCent);
   fMaterials[ "aerogel1035" ] = aerogel1035;
   G4MaterialPropertiesTable *mptA1035 = new G4MaterialPropertiesTable();
   mptA1035->AddProperty("RINDEX",   Aerogel_E, A1035_n,   nAerogelEntries );
   //mptA1035->AddProperty("ABSLENGTH",Aerogel_E, A1035_Abs, nAerogelEntries );
   mptA1035->AddProperty("RAYLEIGH", Aerogel_E, A1035_Ray, nAerogelEntries );
   fMaterials[ "aerogel1035" ]->SetMaterialPropertiesTable( mptA1035 );

   //******************************************************************************************************************************
   // double A104_Abs[nAerogelEntries]; // absorption length
   double A104_Ray[nAerogelEntries]; // rayleigh scattering length
   double A104_n[nAerogelEntries];   // index of refraction
   for ( int i=0; i< nAerogelEntries; ++i ){
     Aerogel_E[ i ] = ( 1.9 + i * (6.2-1.9) / nAerogelEntries )  * CLHEP::eV;
     double lambda = 1239.8 / Aerogel_E[i] * CLHEP::nm;
     double trans20mm = A1040tg->Eval( lambda / CLHEP::nm ) / 100.0 ; // divide by 100 to convert % to fraction
     // get Rayleigh scattering length from transmission through 20 mm
     if ( trans20mm >= 1.0 ) { //100% transmission - set long scatter length
       A104_Ray[i] = 100.0 * CLHEP::km;
     } else if ( trans20mm <= 0.0 ) { // 0% tramission - set tiny scatter length
       A104_Ray[i] = 1.0 * CLHEP::nm;
     } else {
       A104_Ray[i] = -20.0 * CLHEP::mm / std::log( trans20mm );
     }
     A104_n[i]   = sqrt( 1.0 + 0.05639*pow(lambda/CLHEP::nm,2) / ( pow(lambda/CLHEP::nm,2) - pow(83.22,2) ) ) * 1.04/1.030;
     std::cout<<"aerogel104 E["<<i<<"] = "<<Aerogel_E[i]<<"  n="<<A104_n[i]<<std::endl;
   }
   density = 4*(1.035-1.0) * CLHEP::g/ CLHEP::cm3;
   G4Material* aerogel104 = new G4Material ("aerogel104", density, ncomp=3);
   aerogel104->AddElement (elC, fracMass = 0.1*CLHEP::perCent);
   aerogel104->AddMaterial (SiO2, fracMass = 62.5*CLHEP::perCent);
   aerogel104->AddMaterial (H2O, fracMass = 37.4*CLHEP::perCent);
   fMaterials[ "aerogel104" ] = aerogel104;
   G4MaterialPropertiesTable *mptA104 = new G4MaterialPropertiesTable();
   mptA104->AddProperty("RINDEX",   Aerogel_E, A104_n,   nAerogelEntries );
   //mptA104->AddProperty("ABSLENGTH",Aerogel_E, A104_Abs, nAerogelEntries );
   mptA104->AddProperty("RAYLEIGH", Aerogel_E, A104_Ray, nAerogelEntries );
   fMaterials[ "aerogel104" ]->SetMaterialPropertiesTable( mptA104 );

   //******************************************************************************************************************************
   // double A1045_Abs[nAerogelEntries]; // absorption length
   double A1045_Ray[nAerogelEntries]; // rayleigh scattering length
   double A1045_n[nAerogelEntries];   // index of refraction
   for ( int i=0; i< nAerogelEntries; ++i ){
     Aerogel_E[ i ] = ( 1.9 + i * (6.2-1.9) / nAerogelEntries )  * CLHEP::eV;
     double lambda = 1239.8 / Aerogel_E[i] * CLHEP::nm;
     double trans20mm = A1045tg->Eval( lambda / CLHEP::nm ) / 100.0 ; // divide by 100 to convert % to fraction
     // get Rayleigh scattering length from transmission through 20 mm
     if ( trans20mm >= 1.0 ) { //100% transmission - set long scatter length
       A1045_Ray[i] = 100.0 * CLHEP::km;
     } else if ( trans20mm <= 0.0 ) { // 0% tramission - set tiny scatter length
       A1045_Ray[i] = 1.0 * CLHEP::nm;
     } else {
       A1045_Ray[i] = -20.0 * CLHEP::mm / std::log( trans20mm );
     }
     A1045_n[i]   = sqrt( 1.0 + 0.05639*pow(lambda/CLHEP::nm,2) / ( pow(lambda/CLHEP::nm,2) - pow(83.22,2) ) ) * 1.045/1.030;
     std::cout<<"aerogel1045 E["<<i<<"] = "<<Aerogel_E[i]<<"  n="<<A1045_n[i]<<std::endl;
   }
   density = 4*(1.035-1.0) * CLHEP::g/ CLHEP::cm3;
   G4Material* aerogel1045 = new G4Material ("aerogel1045", density, ncomp=3);
   aerogel1045->AddElement (elC, fracMass = 0.1*CLHEP::perCent);
   aerogel1045->AddMaterial (SiO2, fracMass = 62.5*CLHEP::perCent);
   aerogel1045->AddMaterial (H2O, fracMass = 37.4*CLHEP::perCent);
   fMaterials[ "aerogel1045" ] = aerogel1045;
   G4MaterialPropertiesTable *mptA1045 = new G4MaterialPropertiesTable();
   mptA1045->AddProperty("RINDEX",   Aerogel_E, A1045_n,   nAerogelEntries );
   //mptA1045->AddProperty("ABSLENGTH",Aerogel_E, A1045_Abs, nAerogelEntries );
   mptA1045->AddProperty("RAYLEIGH", Aerogel_E, A1045_Ray, nAerogelEntries );
   fMaterials[ "aerogel1045" ]->SetMaterialPropertiesTable( mptA1045 );

   //******************************************************************************************************************************
   // double A105_Abs[nAerogelEntries]; // absorption length
   double A105_Ray[nAerogelEntries]; // rayleigh scattering length
   double A105_n[nAerogelEntries];   // index of refraction
   for ( int i=0; i< nAerogelEntries; ++i ){
     Aerogel_E[ i ] = ( 1.9 + i * (6.2-1.9) / nAerogelEntries )  * CLHEP::eV;
     double lambda = 1239.8 / Aerogel_E[i] * CLHEP::nm;
     double trans20mm = A1050tg->Eval( lambda / CLHEP::nm ) / 100.0 ; // divide by 100 to convert % to fraction
     // get Rayleigh scattering length from transmission through 20 mm
     if ( trans20mm >= 1.0 ) { //100% transmission - set long scatter length
       A105_Ray[i] = 100.0 * CLHEP::km;
     } else if ( trans20mm <= 0.0 ) { // 0% tramission - set tiny scatter length
       A105_Ray[i] = 1.0 * CLHEP::nm;
     } else {
       A105_Ray[i] = -20.0 * CLHEP::mm / std::log( trans20mm );
     }
     A105_n[i]   = sqrt( 1.0 + 0.05639*pow(lambda/CLHEP::nm,2) / ( pow(lambda/CLHEP::nm,2) - pow(83.22,2) ) ) * 1.050/1.030;
     std::cout<<"aerogel105 E["<<i<<"] = "<<Aerogel_E[i]<<"  n="<<A105_n[i]<<std::endl;
   }
   density = 4*(1.035-1.0) * CLHEP::g/ CLHEP::cm3;
   G4Material* aerogel105 = new G4Material ("aerogel105", density, ncomp=3);
   aerogel105->AddElement (elC, fracMass = 0.1*CLHEP::perCent);
   aerogel105->AddMaterial (SiO2, fracMass = 62.5*CLHEP::perCent);
   aerogel105->AddMaterial (H2O, fracMass = 37.4*CLHEP::perCent);
   fMaterials[ "aerogel105" ] = aerogel105;
   G4MaterialPropertiesTable *mptA105 = new G4MaterialPropertiesTable();
   mptA105->AddProperty("RINDEX",   Aerogel_E, A105_n,   nAerogelEntries );
   //mptA105->AddProperty("ABSLENGTH",Aerogel_E, A105_Abs, nAerogelEntries );
   mptA105->AddProperty("RAYLEIGH", Aerogel_E, A105_Ray, nAerogelEntries );
   fMaterials[ "aerogel105" ]->SetMaterialPropertiesTable( mptA105 );

}

/// function to print info about materials available
std::ostream & operator<<( std::ostream& os, const MaterialsMap& mm ){

  std::vector< std::string > matnames;
  mm.get_material_names( matnames );
  for ( const auto & matname : matnames ){
    G4Material* mat = mm[ matname ];
    os << "-----" << matname << "------" << std::endl;
    os << mat;
    os << std::endl;
  }
  return os;
}

