/// TruthTTree.hh
/// Class defining variable to put into an output tree for Truth information.
/// \author {Blair Jamieson, Andrew Sikora} \date{ Oct 2018 }
#ifndef TruthTTree_h
#define TruthTTree_h 1

const int MAXTRACK = 500; //< maximum number of Tracks

/// TruthTTree class to hold information per event
struct TruthTTree {
  int    EventNum;             //< Event number
  int    NTracks;              //< Number of tracks saved
  int    trackid[MAXTRACK];    //< geant track id
  int    PDG[MAXTRACK];        //< PDG code
  float  Px[MAXTRACK];        //< Momentum x component (GeV/c)
  float  Py[MAXTRACK];        //< Momentum y component (GeV/c)
  float  Pz[MAXTRACK];        //< Momentum z component (GeV/c)
  float  VtxX[MAXTRACK];     //< Initial x location (cm)
  float  VtxY[MAXTRACK];     //< Initial y location (cm)
  float  VtxZ[MAXTRACK];     //< Initial z location (cm)
  float  T[MAXTRACK];        //< Initial time (ns)

  const char * GetRootString(){
    if (rootstring != "" ) return rootstring.c_str();
    rootstring ="EventNum/I";
    rootstring+=":NTracks/I";
    rootstring+=":trackid[MAXTRACK]/I";
    rootstring+=":PDG[MAXTRACK]/I";
    rootstring+=":Px[MAXTRACK]/F";
    rootstring+=":Py[MAXTRACK]/F";
    rootstring+=":Pz[MAXTRACK]/F";
    rootstring+=":VtxX[MAXTRACK]/F";
    rootstring+=":VtxY[MAXTRACK]/F";
    rootstring+=":VtxZ[MAXTRACK]/F";
    rootstring+=":T[MAXTRACK]/F";
    return rootstring.c_str(); 
  }

  void MakeTTreeBranches(TTree * t){
    t->Branch( "EventNum", &EventNum, "EventNum/I" );
    t->Branch( "NTracks", &NTracks, "NTracks/I" );
    t->Branch( "trackid", trackid, "trackid[NTracks]/I" );
    t->Branch( "PDG", PDG, "PDG[NTracks]/I" );
    t->Branch( "Px", Px, "Px[NTracks]/F" );
    t->Branch( "Py", Py, "Py[NTracks]/F" );
    t->Branch( "Pz", Pz, "Pz[NTracks]/F" );
    t->Branch( "VtxX", VtxX, "VtxX[NTracks]/F" );
    t->Branch( "VtxY", VtxY, "VtxY[NTracks]/F" );
    t->Branch( "VtxZ", VtxZ, "VtxZ[NTracks]/F" );
    t->Branch( "T", T, "T[NTracks]/F" );

    return;
  }

  void SetBranchAddresses(TTree * t){
    t->SetBranchAddress( "EventNum", &EventNum );
    t->SetBranchAddress( "NTracks", &NTracks );
    t->SetBranchAddress( "trackid", trackid );
    t->SetBranchAddress( "PDG", PDG );
    t->SetBranchAddress( "Px", Px );
    t->SetBranchAddress( "Py", Py );
    t->SetBranchAddress( "Pz", Pz );
    t->SetBranchAddress( "VtxX", VtxX );
    t->SetBranchAddress( "VtxY", VtxY );
    t->SetBranchAddress( "VtxZ", VtxZ );
    t->SetBranchAddress( "T", T );

    return;
  }
  
  TruthTTree(){
    Init(0);
  }

  void Init( int evtnum = 0 ){
    EventNum=evtnum;
    NTracks=0;
    for (unsigned i=0; i<MAXTRACK; ++i){
      trackid[i]=-9999;
      PDG[i] = -9999;
      Px[i]=-9999.0;
      Py[i]=-9999.0;
      Pz[i]=-9999.0;
      VtxX[i]=-9999.0;      
      VtxY[i]=-9999.0;      
      VtxZ[i]=-9999.0;
      T[i]=-9999.0;
    }
  }

private:
  // hold copy of string needed for making TTree
  std::string rootstring;
};


#endif
