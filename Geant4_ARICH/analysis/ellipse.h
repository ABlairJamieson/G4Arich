#ifndef _ellipse_h_
#define _ellipse_h_

#include <iostream>
#include <cmath>

using std::ostream;
using std::acos;

const double pi = acos(-1.);

struct xypoint{
  double x;
  double y;
  xypoint( double xx, double yy ) : x(xx), y(yy) { }; 
  xypoint() :x(0), y(0) { };
};

struct ellipse_st {
  // define an ellipse by:
  //   bb == shorter axis length
  //   ee == eccentricity
  //   phiphi == rotation of ellipse about x axis
  //   cc == xypoint of center of ellipse
  ellipse_st( double bb, double ee, double phiphi, xypoint cc );

  // parametric function radius of ellipse 
  // at theta == angle from long axis in radians
  double r_of_theta( double theta ) const;

  // get x, y coordinates on ellipse at
  // theta == angle from long axis in radians
  xypoint xy( double theta ) const;

  // find smallest distance from arbitrary point (px, py) to
  // a point on the ellipse.
  double dmin( xypoint p ) const;

  // faster method of finding smallest distnce-squared from point p 
  // to a point on the ellipse. From:
  // https://www.geometrictools.com/Documentation/DistancePointEllipseEllipsoid.pdf
  double fast_dmin2 (xypoint p) const;

  //function needed for fast_dmin
  double Findtbar (double a, xypoint pprime) const;

  //function needed for fast_dmin
  double F (double a, xypoint pprime, double t0, double t1) const;


  double GetRoot(double r0, double z0, double z1, double g) const;

  double DistancePointEllipse2(xypoint p) const;


  // return penalty term in a chi2 for having a bad eccentricity parameter
  double get_epenalty() const { return epenalty; }

  //return semimajor axis of ellipse
  double get_b() const {return b;}

  // find smallest distance-squared from arbitrary point (px, py) to
  // a point on the ellipse.
  double dmin2( xypoint p ) const;

  // find closest point on ellipse that is smallest distance from 
  // arbitrary point (px, py) to a point on the ellipse.
  xypoint closest_point( xypoint p ) const;

  friend ostream& operator<<( ostream& os, const ellipse_st &  e );

private:
  xypoint c; // center
  double  b; // short axis length
  double  e; // eccentricity 0=circle, 1=parallel lines
  double  phi; // rotation of ellipse from x axis
  double  epenalty; // penalty term for bad eccentricity
};
  

#endif
