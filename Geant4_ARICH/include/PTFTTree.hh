/// PTFTTree.hh
/// Class defining variable to put into an output tree for ARICH simulation.
/// \author {Blair Jamieson} \date{ July 2018 }
/// Modifications:
///   Oct. 2018  Andrew Sikora   Add starting position of photon and primary particle initial position, direction, energy
#ifndef PTFTTree_h
#define PTFTTree_h 1

const int MAXPMT = 100000; //< maximum number of PMTs
const int MAXQ = 1000000; //< maximum number of photons stored in TTree

/// PTFTTree class to hold information per event
struct PTFTTree {
  int   EventNum;             //< Event number
  int   NPMT;                 //< Number of PMTs digitized
  int   pmtid[MAXPMT];        //<[NPMT] PMT number 
  float dig_T[MAXPMT];        //<[NPMT] Digitizer Time (ns)
  int   dig_Q[MAXPMT];        //<[NPMT] Digitizer Number of photons
  int   NPhotons;             //< truth Number of photons 
  int   true_pmtid[MAXQ];     //<[NPhotons] pmtid that this photon hit
  int   true_used[MAXQ];      //<[NPhotons] whether this photon was detected
  float true_x[MAXQ];         //<[NPhotons] truth end x (mm)
  float true_y[MAXQ];         //<[NPhotons] truth end y (mm)
  float true_z[MAXQ];         //<[NPhotons] truth end z (mm)
  float true_dx[MAXQ];        //<[NPhotons] truth end dx (mm)
  float true_dy[MAXQ];        //<[NPhotons] truth end dy (mm)
  float true_dz[MAXQ];        //<[NPhotons] truth end dz (mm)
  float true_t[MAXQ];         //<[NPhotons] truth time of photon (ns)
  float true_e[MAXQ];         //<[NPhotons] truth photon energy (eV)
  float true_ini_z[MAXQ];     //<[NPhotons] truth initial z position
  int   root_eventid;         //< if generated from root input, event id of root event
  ULong64_t root_idx; //< if generated from root input, index into files
  

  const char * GetRootString(){
    if (rootstring != "" ) return rootstring.c_str();
    rootstring ="EventNum/I";
    rootstring+=":NPMT/I";
    rootstring+=":pmtid[NPMT]/I";
    rootstring+=":dig_T[NPMT]/F";
    rootstring+=":dig_Q[NPMT]/I";
    rootstring+=":NPhotons/I";
    rootstring+=":true_pmtid[NPhotons]/I";
    rootstring+=":true_used[NPhotons]/I";
    rootstring+=":true_x[NPhotons]/F";
    rootstring+=":true_y[NPhotons]/F";
    rootstring+=":true_z[NPhotons]/F";
    rootstring+=":true_dx[NPhotons]/F";
    rootstring+=":true_dy[NPhotons]/F";
    rootstring+=":true_dz[NPhotons]/F";
    rootstring+=":true_t[NPhotons]/F";
    rootstring+=":true_e[NPhotons]/F";
    rootstring+=":true_ini_z[NPhotons]/F";
    rootstring+=":root_eventid/I";
    rootstring+=":root_idx/l";

    return rootstring.c_str(); 
  }

  void MakeTTreeBranches(TTree * t){
    t->Branch( "EventNum", &EventNum, "EventNum/I" );
    t->Branch( "NPMT", &NPMT, "NPMT/I" );
    t->Branch( "pmtid", pmtid, "pmtid[NPMT]/I" );
    t->Branch( "dig_T", dig_T, "dig_T[NPMT]/F" );
    t->Branch( "dig_Q", dig_Q, "dig_Q[NPMT]/I" );
    t->Branch( "NPhotons", &NPhotons, "NPhotons/I" );
    t->Branch( "true_pmtid", true_pmtid, "true_pmtid[NPhotons]/I" );
    t->Branch( "true_used", true_used, "true_used[NPhotons]/I" );
    t->Branch( "true_x", true_x, "true_x[NPhotons]/F" );
    t->Branch( "true_y", true_y, "true_y[NPhotons]/F" );
    t->Branch( "true_z", true_z, "true_z[NPhotons]/F" );
    t->Branch( "true_dx", true_dx, "true_dx[NPhotons]/F" );
    t->Branch( "true_dy", true_dy, "true_dy[NPhotons]/F" );
    t->Branch( "true_dz", true_dz, "true_dz[NPhotons]/F" );
    t->Branch( "true_t", true_t, "true_t[NPhotons]/F" );
    t->Branch( "true_e", true_e, "true_e[NPhotons]/F" );
    t->Branch( "true_ini_z", true_ini_z, "true_ini_z[NPhotons]/F" );
    t->Branch( "root_eventid", &root_eventid, "root_eventid/I" );
    t->Branch( "root_idx", &root_idx, "root_idx/l" );
    return;
  }


  void SetBranchAddresses(TTree * t){
    t->SetBranchAddress( "EventNum", &EventNum );
    t->SetBranchAddress( "NPMT", &NPMT );
    t->SetBranchAddress( "pmtid", pmtid );
    t->SetBranchAddress( "dig_T", dig_T );
    t->SetBranchAddress( "dig_Q", dig_Q );
    t->SetBranchAddress( "NPhotons", &NPhotons );
    t->SetBranchAddress( "true_pmtid", true_pmtid );
    t->SetBranchAddress( "true_used", true_used );
    t->SetBranchAddress( "true_x", true_x );
    t->SetBranchAddress( "true_y", true_y );
    t->SetBranchAddress( "true_z", true_z  );
    t->SetBranchAddress( "true_dx", true_dx );
    t->SetBranchAddress( "true_dy", true_dy);
    t->SetBranchAddress( "true_dz", true_dz);
    t->SetBranchAddress( "true_t", true_t );
    t->SetBranchAddress( "true_e", true_e );
    t->SetBranchAddress( "true_ini_z", true_ini_z );
    t->SetBranchAddress( "root_eventid", &root_eventid );
    t->SetBranchAddress( "root_idx", &root_idx );
    return;
  }
  
  PTFTTree(){
    Init(0);
  }

  void Init( int evtnum = 0 ){
    EventNum=evtnum;
    NPMT=0;
    for (unsigned i=0; i<MAXPMT; ++i){
      pmtid[i]=-9999;
      dig_T[i]=-9999.0;
      dig_Q[i]=-9999;
    }
    NPhotons=0;
    for (unsigned i=0; i<MAXQ; ++i){
      true_pmtid[i]=-9999;
      true_used[i]=0;
      true_x[i]=-9999.0;
      true_y[i]=-9999.0;
      true_z[i]=-9999.0;
      true_dx[i]=-9999.0;
      true_dy[i]=-9999.0;
      true_dz[i]=-9999.0;
      true_t[i]=-9999.0;
      true_e[i]=-9999.0;
      true_ini_z[i]=-9999.0;
    }
    root_eventid = 0;
    root_idx = 0;
  }

private:
  // hold copy of string needed for making TTree
  std::string rootstring;
};


#endif
