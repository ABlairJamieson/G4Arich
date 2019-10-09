#ifndef _ringfitchi2_h_
#define _ringfitchi2_h_

#include "ellipse.h"
#include <vector>

/// ringfitchi2 functor calculates chi2 from vector of points to 
/// parameters of ellipse passed as arguments.  See operator() for 
/// the parameter order.
/// Author:  Blair Jamieson, Andrew Sikora (Mar. 2019)

// for real chi2, need to divide by a sigma on each location:
// use magic number: pixel size divide by sqrt(12)
const double one_over_sigma2 = (12.0/36.0); // for  1/ ( (6mm)^2 / 12 )


// ring fit chi2 functor
struct ringfitchi2 {  
  // xypoints assumed to be in mm
  ringfitchi2( std::vector< xypoint > p ) : xy( p ){ }

  // function to calculate the chi2
  // pars[0] = b (short radius) (mm)
  // pars[1] = eccentricity
  // pars[2] = phi (rotation of short radius from x axis in radians)
  // pars[3] = center x (mm)
  // pars[4] = center y (mm)
  double operator()( const double * pars); 

private:
  std::vector< xypoint > xy;
};




#endif
