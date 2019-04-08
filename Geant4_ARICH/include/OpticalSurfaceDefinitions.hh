#ifndef _OpticalSurfaceDefinitions_hh_
#define _OpticalSurfaceDefinitions_hh_

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

/// OpticalSurfaceDefinitions
/// Singleton class to set up optical surfaces
/// B. Jamieson, April 2018
class OpticalSurfaceDefinitions {
public:
  class Invalid{ };// throw exception when undefined surface requested
  
  /// Get() single instance of class
  static OpticalSurfaceDefinitions& Get();
  
  /// Operator to get a G4OpticalSurface of given name
  G4OpticalSurface * operator[]( const std::string& key ) const;
  
  /// Setup a skin optical surface using an optical surface of a given name
  void skin_surface( const std::string& surface_name,
		     G4VPhysicalVolume* volume );
  
  /// Setup a border optical surface
  void border_surface( const std::string& surface_name,
		       G4VPhysicalVolume* volume1,
		       G4VPhysicalVolume* volume2 );
  
protected:
  OpticalSurfaceDefinitions();
  
private:
  void BuildSurfaces();
  void ReadOptSurfaces();

  static std::vector< G4LogicalBorderSurface * >     fBorders;
  static std::vector< G4LogicalSkinSurface *>        fSkins;
  static std::map< std::string, G4OpticalSurface * > fSurfaces;  
  static OpticalSurfaceDefinitions * fOpSurfaceDefs;

  // values read from file "ptf-optsurfaces.txt" (in ReadOptSurfaces)
  double blacksheet_BSRFF; //< reflectivity scaling factor
  double blacksheet_CSS;   //< specular scatter fraction 
  double blacksheet_CSL;   //< specular lobe fraction
  double blacksheet_sigmaalpha; //< radians (facet normals in UNIFIED model)
  double blacksheet_CBS;   //< backscatter fraction

  double cathode_CSS;
  double cathode_CSL;
  double cathode_CBS;
  double cathode_sigmaalpha;

  double pmtglass_CSS;
  double pmtglass_CSL;
  double pmtglass_CBS;
  double pmtglass_sigmaalpha;

  double acrylic_CSS;
  double acrylic_CSL;
  double acrylic_CBS;
  double acrylic_sigmaalpha;

  double water_CSS;
  double water_CSL;
  double water_CBS;
  double water_sigmaalpha;
  
  double reflector_reflectivity;

  double pmtsteel_reflectivity;
  
};

#endif

