#ifndef _ellipsefitter_h_
#define _ellipsefitter_h_

#include "ellipse.h"
#include "hough.h"

#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>
#include <TTree.h>

#include "ringfitchi2.h"

const unsigned MAXRINGS = 20;

/// EllipseFitter class
/// -- Does chi-squared fit of ellipses to rings found using CircleHough
/// -- Stores fit results in TTree
/// Author: Blair Jamieson (Oct. 2019)
struct EllipseFitter {
 public:
  EllipseFitter();
  ~EllipseFitter();

  /// does ring fit to all rings identified and stored in HoughResults
  /// if index is not -1 (maximum unsigned) then read in input file that
  /// can be used to put constraints on the ring
  void fit_rings( const HoughResults& hrs, unsigned long long index );

  /// Make branches for TTree to write results in
  /// You must call this before calling fit_rings
  /// that way you create the TTree branches that result goes to
  void MakeTTreeBranches(TTree * t);

  /// Make branches for TTree to read results in
  void SetBranchAddresses(TTree * t);

  /// Initialize TTree variables
  void Init( unsigned long long evnum=0 );
  
  // keep data publicly accessible
  unsigned long long Event;        //< Event number of sim
  unsigned long long EventIdx;     //< Event number in input to sim
  unsigned           NRings;       //< Number of rings in event
  unsigned           N   [MAXRINGS]; //< number of pixels in ellipse
  float              b   [MAXRINGS]; //< shorter axis length "radius"
  float              e   [MAXRINGS]; //< eccentricity 0 < e < 1
  float              phi [MAXRINGS]; //< rotation of ellipse about x-axis
  float              x   [MAXRINGS]; //< center of ellipse in x
  float              y   [MAXRINGS]; //< center of ellipse in y
  float              be  [MAXRINGS]; //< error from fit shorter axis length "radius"
  float              ee  [MAXRINGS]; //< error from fit eccentricity 0 < e < 1
  float              phie[MAXRINGS]; //< error from fit rotation of ellipse about x-axis
  float              xe  [MAXRINGS]; //< error from fit center of ellipse in x
  float              ye  [MAXRINGS]; //< error from fit center of ellipse in y
  float              chi2[MAXRINGS]; //< chi2 from fit
  int                stat[MAXRINGS]; //< Fit status 0==good, other==failed
  int                isring[MAXRINGS]; //< 0==Hough thought it was a ring,  other==not a ring 


private:
  ringfitchi2           * fchi2;   // chi2 calculation
  ROOT::Math::Functor   * fcn;     // holder for chi2 function used in minimization
  ROOT::Math::Minimizer * minalg;  // root minimizer (TMinuit2)

  std::vector< std::string> parnames;
  std::vector< double >     step;
  std::vector< double >     pars; 
};



#endif // _ellipsefitter_h_
