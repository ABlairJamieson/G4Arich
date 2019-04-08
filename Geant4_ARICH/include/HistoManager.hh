/// \author{ Blair Jamieson } \date{ 2011 }
/// Singleton to hold all of the histograms and make them
/// accessible at any stage of the processing
/// updates:  July 2018 -- modify for PTFTTree use
///           Oct  2018 -- modify for TruthTTree
#ifndef HistoManager_h
#define HistoManager_h 1

#include <TFile.h>
#include <TTree.h>
#include "PTFTTree.hh"
#include  "TruthTTree.hh"

/// Histogram manager class holds global copy
/// of all of the histograms.
class HistoManager {
private:
  static bool instanceFlag;
  static HistoManager * hman;
  HistoManager(){};     //< private constructor does nothing
  
public:
  static HistoManager * GetManager();
  ~HistoManager(){
    instanceFlag = false;
  }

  void BookHistograms( int arunnum );
  void WriteHistograms();
  
  TFile * fout; //< Output file

  /// Storage container for values being
  /// saved into the tree.
  PTFTTree * ptf;
  TruthTTree * truth;

  /// Pointer to the Tree
  TTree * ptf_tree; 
  TTree * truth_tree;
};

#endif
