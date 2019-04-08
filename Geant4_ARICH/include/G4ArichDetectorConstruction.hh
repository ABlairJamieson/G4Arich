//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef G4ArichDetectorConstruction_h
#define G4ArichDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "H12700Geometry.hh"

//class G4ArichDetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ArichDetectorConstruction : public G4VUserDetectorConstruction {
public:
  G4ArichDetectorConstruction();
  virtual ~G4ArichDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  void ConstructSDandField();
    

  void set_hasmirror( bool ahasmirror ){
    std::cout<<"G4ArichDetectorConstruction::set_hasmirror="<<ahasmirror<<std::endl;
    hasmirror = ahasmirror;}
  void set_mirrorlength( double amirrorlength ){
    std::cout<<"G4ArichDetectorConstruction::set_mirrorlength="<<amirrorlength/CLHEP::mm<<" mm"<<std::endl;
    mirrorlength = amirrorlength; }
  
private:
  void ReadGeometry();

  //G4ArichDetectorMessenger* fDetMessenger;
  
  G4double fExpHall_x;
  G4double fExpHall_y;
  G4double fExpHall_z;

  // parameters for mirror around aerogel:
  bool     hasmirror; // default true
  double   mirrorlength;  // default 40mm

  // keep track of the world
  G4LogicalVolume * expHall_log;
  G4VPhysicalVolume * expHall_phys;
  G4VPhysicalVolume *  waterTank_phys;

  G4VPhysicalVolume * front_ag[4];
  G4VPhysicalVolume * back_ag[4];
  G4VPhysicalVolume * mirror_ag[4];

  // array of multi-anode PMTs
  std::vector< H12700Geometry * > fPMTs;
    

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*G4ArichDetectorConstruction_h*/
