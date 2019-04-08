
//
// Description: Modified from OpNovice example
//              to simulate optics of aerogel ring imaging cherenkov deector
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --
// Author: Blair Jamieson, Andrew Sikora  Sept. 2018 - Apr. 2019

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "G4ArichPhysicsList.hh"
#include "G4ArichDetectorConstruction.hh"
#include "G4ArichActionInitialization.hh"
#include "PTFDigitizeEvent.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " G4Arich [-m macro ] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }
  std::cout<<"Here 1"<<std::endl;
  
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  G4long myseed = 345354;
  for ( G4int i=1; i<argc; i=i+2 ) {
     if      ( G4String(argv[i]) == "-m" ) macro   = argv[i+1];
     else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
     else if ( G4String(argv[i]) == "-r" ) myseed  = atoi(argv[i+1]);
#ifdef G4MULTITHREADED
     else if ( G4String(argv[i]) == "-t" ) {
                    nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

    std::cout<<"Here 2"<<std::endl;

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
  //#ifdef G4MULTITHREADED
  //  G4MTRunManager * runManager = new G4MTRunManager;
  //  if ( nThreads > 0 ) runManager->SetNumberOfThreads(nThreads);
  //#else
  G4RunManager * runManager = new G4RunManager;
  //#endif
    std::cout<<"Here 3"<<std::endl;


  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

    std::cout<<"Here 3b"<<std::endl;
  // Physics list has to be before ruser action!
  runManager-> SetUserInitialization(new G4ArichPhysicsList());
    std::cout<<"Here 3c"<<std::endl;
  
  // read the macro file first!
  // User action initialization
  runManager->SetUserInitialization(new G4ArichActionInitialization());
    std::cout<<"Here 3d"<<std::endl;

  // Set mandatory initialization classes
  //

   // Detector construction
  G4ArichDetectorConstruction * fdet = new G4ArichDetectorConstruction(); 
  runManager-> SetUserInitialization( fdet );
    std::cout<<"Here 3d"<<std::endl;
  // Event action
  runManager->SetUserAction( new PTFDigitizeEvent() );
    std::cout<<"Here 4"<<std::endl;

  std::cout<<"Here 5"<<std::endl;

    std::cout<<"Here 6"<<std::endl;

  // Initialize G4 kernel
  //
  runManager->Initialize();

    std::cout<<"Here 7"<<std::endl;

  
#ifdef G4VIS_USE
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer(); 
  UImanager->ApplyCommand( "/process/activate Cerenkov" );   

  if ( macro.size() ) {
     // Batch mode
     G4String command = "/control/execute ";
     UImanager->ApplyCommand(command+macro);
  }
  else // Define UI session for interactive mode
  {
#ifdef G4UI_USE
     G4UIExecutive * ui = new G4UIExecutive(argc,argv,session);
#ifdef G4VIS_USE
     UImanager->ApplyCommand("/control/execute vis.mac");
#else
     UImanager->ApplyCommand(" ");
#endif
     if (ui->IsGUI())
        UImanager->ApplyCommand("/control/execute gui.mac");

     //fdet->ConstructSD();
     ui->SessionStart();
     delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
