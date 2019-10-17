
#include "ringfitchi2.h"


double ringfitchi2::operator()( const double * pars ){
  double chi2=0.0;
  ellipse_st e1( pars[0], pars[1], pars[2], xypoint( pars[3], pars[4] ) ); 
  for ( const xypoint p : xy ){
    //slow method
    double dmin2 = e1.dmin2( p );

    //faster method
    //double dmin2 = e1.fast_dmin2(p);

    //faster try #2

    //double dmin2 = e1.DistancePointEllipse2(p);

    chi2 += dmin2 * one_over_sigma2;
  }
  // add a penalty for bad eccentricity
  chi2 += e1.get_epenalty();

  return chi2;
}


