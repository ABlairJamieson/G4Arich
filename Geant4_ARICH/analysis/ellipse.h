#ifndef _ellipse_h_
#define _ellipse_h_

#include <iostream>
#include <cmath>
#include "xypoint.h"

using std::ostream;
using std::acos;

const double pi = acos(-1.);


/// ellipse_st: define an ellipse by:
///   bb == shorter axis length
///   ee == eccentricity
///   phiphi == rotation of ellipse about x axis
///   cc == xypoint of center of ellipse
///
/// Author: Blair Jamieson (Summer 2019) 
struct ellipse_st {

  ellipse_st( double bb, double ee, double phiphi, xypoint cc );

  // parametric function radius of ellipse 
  // at theta == angle from long axis in radians
  double r_of_theta( double theta ) const;

  // get x, y coordinates on ellipse at
  // theta == angle from long axis in radians
  xypoint xy( double theta ) const;

  // get x, y coordinates on ellipse at
  // theta == angle from x-axis
  // without rotation of ellipse by phi or offset by center
  xypoint xy_norot( double theta ) const;

  // find smallest distance from arbitrary point (px, py) to
  // a point on the ellipse.
  double dmin( xypoint p ) const;

  // check if point is inside ellipse (return true if it is)
  bool is_inside( xypoint p ) const;

  // return penalty term in a chi2 for having a bad eccentricity parameter
  double get_epenalty() const { return epenalty; }

  //return semimajor axis of ellipse
  double get_b() const {return b;}

  // find smallest distance-squared from arbitrary point (px, py) to
  // a point on the ellipse. Also return closest point.
  double dmin2( const xypoint& p, xypoint& xymin ) const;

  // find smallest distance-squared from arbitrary point (px, py) to
  // a point on the ellipse. Faster implementation.
  double dmin2( const xypoint& p ) const;

  // find closest point on ellipse that is smallest distance from 
  // arbitrary point (px, py) to a point on the ellipse.
  xypoint closest_point( const xypoint &p ) const;

  // Below method doesn't always work:  Needs debugging, or maybe it
  // would never work
  // faster method of finding smallest distnce-squared from point p 
  // to a point on the ellipse. From:
  // https://www.geometrictools.com/Documentation/DistancePointEllipseEllipsoid.pdf
  double DistancePointEllipse2(xypoint p, xypoint& closestxy, int& kind ) const;
  
  friend ostream& operator<<( ostream& os, const ellipse_st &  e );

private:
  
  // function used by DistancePointEllipse
  double GetRoot(double r0, double z0, double z1, double g) const;
  
  xypoint c; // center
  double  b; // short axis length
  double  e; // eccentricity 0=circle, 1=parallel lines
  double  phi; // rotation of ellipse from x axis
  double  epenalty; // penalty term for bad eccentricity
};




#endif
