
#include "ellipsefitter.h"


EllipseFitter::EllipseFitter() {

  // prepare root minimizer
  minalg = ROOT::Math::Factory::CreateMinimizer("Minuit2", "");
  if (minalg==nullptr) {
    std::cerr<<"Error making ROOT::Math::Factory"<<std::endl;
    exit(0);
  }
  minalg->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
  minalg->SetMaxIterations(10000);  // for GSL
  minalg->SetTolerance(0.001);
  minalg->SetPrintLevel(0);

  // build functor to do the fit
  std::vector< xypoint > pts; // we will set these later
  fchi2 = new ringfitchi2( pts );
  fcn = new ROOT::Math::Functor(fchi2, &ringfitchi2::operator(), 5);
  minalg->SetFunction( *fcn );

  parnames = { "b  ","e  ","phi","xc ","yc " };
  step     = {0.01,0.01,0.01,0.01,0.01};
}

EllipseFitter::~EllipseFitter(){
  delete fchi2;
  delete fcn;
  delete minalg;
}


void EllipseFitter::fit_rings( const HoughResults& hrs, unsigned long long index ){
  static unsigned long long call_count=0;
  ++call_count;
  // For now we ignore index.  Eventually it should be used to read in constraints
  Init( call_count );
  EventIdx = index;
  NRings   = hrs.size();
  unsigned iring = 0;
  for ( const HoughResult& hr : hrs ){
    // five parameter fit --> need at least 5 hits!
    if ( hr.data.size() < 5 ) {
      N   [ iring ]   = hr.data.size();
      stat[ iring ]   = 1;
      isring[ iring ] = hr.type;
      continue; // go to next ring
    }
    // set the data to fit
    fchi2->set_points( hr.data );

    // reset the minimizer
    minalg->Clear();
    
    // initialize parameter guesses (b, e, phi, xc, yc)
    pars = { hr.rc, 0.0, 0.0, hr.xyc.x, hr.xyc. y };
    for ( int i=0; i<5; ++i ){
      minalg->SetVariable( i, parnames[i].c_str(), pars[i], step[i] );
    }
    // do the minimization
    minalg->Minimize();
    // get result
    const double *xs  = minalg->X();
    const double *xse = minalg->Errors();
    
    // save the fit result to TTree structure
    N   [ iring ] = hr.data.size();
    b   [ iring ] = xs[0];
    e   [ iring ] = xs[1];
    phi [ iring ] = xs[2];
    x   [ iring ] = xs[3];
    y   [ iring ] = xs[4];
    be  [ iring ] = xse[0];
    ee  [ iring ] = xse[1];
    phie[ iring ] = xse[2];
    xe  [ iring ] = xse[3];
    ye  [ iring ] = xse[4];
    chi2[ iring ] = minalg->MinValue();
    stat[ iring ] = minalg->Status();
    isring[ iring ] = hr.type;
    ++iring;
  }
    
}


void EllipseFitter::MakeTTreeBranches( TTree * t ){
  t->Branch( "Event"   , &Event   , "Event/l"          );
  t->Branch( "EventIdx", &EventIdx, "EventIdx/l"       );
  t->Branch( "NRings"  , &NRings  , "NRings/i"         );
  t->Branch( "N"       ,  N       , "N[NRings]/i"      );
  t->Branch( "b"       ,  b       , "b[NRings]/F"      );
  t->Branch( "e"       ,  e       , "e[NRings]/F"      );
  t->Branch( "phi"     ,  phi     , "phi[NRings]/F"    );
  t->Branch( "x"       ,  x       , "x[NRings]/F"      );
  t->Branch( "y"       ,  y       , "y[NRings]/F"      );
  t->Branch( "be"      ,  be      , "be[NRings]/F"     );
  t->Branch( "ee"      ,  ee      , "ee[NRings]/F"     );
  t->Branch( "phie"    ,  phie    , "phie[NRings]/F"   );
  t->Branch( "xe"      ,  xe      , "xe[NRings]/F"     );
  t->Branch( "ye"      ,  ye      , "ye[NRings]/F"     );
  t->Branch( "chi2"    ,  chi2    , "chi2[NRings]/F"   );
  t->Branch( "stat"    ,  stat    , "stat[NRings]/I"   );
  t->Branch( "isring"  ,  isring  , "isring[NRings]/I" );
}

void EllipseFitter::SetBranchAddresses( TTree * t ){
  t->SetBranchAddress( "Event"   , &Event    );
  t->SetBranchAddress( "EventIdx", &EventIdx );
  t->SetBranchAddress( "NRings"  , &NRings   );
  t->SetBranchAddress( "N"       ,  N        );
  t->SetBranchAddress( "b"       ,  b        );
  t->SetBranchAddress( "e"       ,  e        );
  t->SetBranchAddress( "phi"     ,  phi      );
  t->SetBranchAddress( "x"       ,  x        );
  t->SetBranchAddress( "y"       ,  y        );
  t->SetBranchAddress( "be"      ,  be       );
  t->SetBranchAddress( "ee"      ,  ee       );
  t->SetBranchAddress( "phie"    ,  phie     );
  t->SetBranchAddress( "xe"      ,  xe       );
  t->SetBranchAddress( "ye"      ,  ye       );
  t->SetBranchAddress( "chi2"    ,  chi2     );
  t->SetBranchAddress( "stat"    ,  stat     );
  t->SetBranchAddress( "isring"  ,  isring   );
}

void EllipseFitter::Init( unsigned long long evnum ){
  Event = evnum;
  EventIdx = 0;
  NRings = 0;
  for ( unsigned i =0 ; i< MAXRINGS; ++i ){
    N   [i]=0;
    b   [i]=-9999.;
    e   [i]=-9999.;
    phi [i]=-9999.;
    x   [i]=-9999.;
    y   [i]=-9999.;
    be  [i]=-9999.;
    ee  [i]=-9999.;
    phie[i]=-9999.;
    xe  [i]=-9999.;
    ye  [i]=-9999.;
    chi2[i]=-9999.;
    stat[i]=9;
    isring[i]=1;
  }
}
