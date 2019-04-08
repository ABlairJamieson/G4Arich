
#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"


// initialize static member variables
bool HistoManager::instanceFlag = false;
HistoManager * HistoManager::hman = NULL;

/// Return a pointer to the singleton instance
HistoManager * HistoManager::GetManager(){
  if (!instanceFlag){
    hman = new HistoManager();
    instanceFlag = true;
    return hman;
  } else {
    return hman;
  }
}

/// Build all of the histograms and trees 
/// for a given run number
void HistoManager::BookHistograms( int arunnum ){
  
  // Open output root file
  char outname[128];
  sprintf( outname, "g4arich_%06d.root", arunnum ) ;
  fout = new TFile( outname, "recreate" );

  ptf_tree = new TTree("ptf_tree","ptf_tree");
  ptf = new PTFTTree;

  Int_t split = 1;
  Int_t bsize = 16000;

  ptf->MakeTTreeBranches( ptf_tree );

  truth_tree = new TTree("truth_tree","truth_tree");
  truth = new TruthTTree;

  truth->MakeTTreeBranches( truth_tree );
  
  return;
}

/// Write histograms and trees to file, by calling TFile::Write
void HistoManager::WriteHistograms(){
  fout->Write();
  //fout->Close();
}
