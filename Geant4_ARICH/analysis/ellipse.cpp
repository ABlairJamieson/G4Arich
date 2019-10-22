
#include "ellipse.h"
#include <vector>

using std::ostream;

ellipse_st::ellipse_st( double bb, double ee, double phiphi, xypoint cc ) :
  b(bb), e(ee), phi(phiphi), c(cc) { 
  epenalty = 0.0;
  if ( e>1.0 ) { 
    //std::cerr<<"Error eccentricity e="<<e<<" too large, capping at e=1"<<std::endl;
    epenalty = (e-1.0)*(e-1.0)*100;
    e=1.0;
  }
  if ( e<0.0 ) { 
    //std::cerr<<"Error eccentricity e="<<e<<" negative, capping at e=0"<<std::endl;
    epenalty = e*e*100;
    e=0.0;
  }
  if ( bb<0.0 ) bb = fabs( bb );
}

// parametric function radius of ellipse 
// at theta == angle from long axis in radians
double ellipse_st::r_of_theta( double theta ) const {
  double checkfirst = 1 - e*e*cos(theta)*cos(theta);
  if ( checkfirst < 0. ) return 0.;
  return b / sqrt( checkfirst );
}

// get x, y coordinates on ellipse at
// theta == angle from long axis in radians
xypoint ellipse_st::xy( double theta ) const{
  double r = r_of_theta( theta );
  xypoint p = c;
  p.x += r*cos(theta) * cos( phi ) - r*sin(theta) * sin( phi );
  p.y += r*cos(theta) * sin( phi ) + r*sin(theta) * cos( phi );
  return p;
}

// get x, y coordinates on ellipse that is centered and not rotated
xypoint ellipse_st::xy_norot( double theta ) const {
  double r = r_of_theta( theta );
  return xypoint( r*cos(theta), r*sin(theta) );
}


// check if point is inside ellipse (return true if it is)
bool ellipse_st::is_inside( xypoint p ) const {
  // rotate point to coord system of centered ellips
  double xp =  (p.x - c.x) * cos(phi) + (p.y - c.y) * sin(phi); 
  double yp = -1 * (p.x - c.x) * sin(phi) + (p.y - c.y) * cos(phi);
  
  double e0 = b / sqrt( 1 - e * e );   //long axis of ellipse
  double e1 = b; // short axis of ellipse
  
  if ( xp*xp/e0/e0 + yp*yp/e1/e1 <= 1 ) return true;
  return false;
  
}


// find smallest distance from arbitrary point (px, py) to
// a point on the ellipse.
double ellipse_st::dmin( xypoint p ) const{
  return sqrt( dmin2( p ) );
}

// find smallest distance-squared from arbitrary point (px, py) to
// a point on the ellipse.
double ellipse_st::dmin2( const xypoint& p, xypoint& xymin ) const {
  // work in coordinates of ellipse
  // Point becomes Y = (y0,y1)
  double px =  (p.x - c.x) * cos( phi ) + (p.y - c.y) * sin( phi ); 
  double py = -1.0 * (p.x - c.x) * sin( phi ) + (p.y - c.y) * cos( phi );

  std::vector< double >  ang;
  // pick initial angle range based on px,py
  if ( px > 0  && py > 0) { // first quadrant
    ang.push_back( 0. );
    ang.push_back( pi/2 ); 
  } else if ( px < 0 && py > 0 ) { // second quadrant
    ang.push_back( pi/2 );
    ang.push_back( pi ); 
  } else if ( px < 0 && py < 0 ) { // third quadrant
    ang.push_back( pi );
    ang.push_back( 3*pi/2 ); 
  } else {
    ang.push_back( 3*pi/2 );
    ang.push_back( 2*pi ); 
  }

  double curdmin = 999e99;
  double angmin=ang[0];
  double dang = (ang[1]-ang[0])/10;
  for ( double theta=ang[0]; theta<=ang[1]; theta+=dang ){
    xypoint curxy = xy_norot( theta );
    double cur_d2 = (curxy.x-px)*(curxy.x-px)  +(curxy.y-py)*(curxy.y-py) ;
    if ( cur_d2 < curdmin ) {
      curdmin = cur_d2;
      angmin = theta;
    }
  }
  for ( double theta=angmin-dang; theta<=angmin+dang; theta+=dang/10 ){
    xypoint curxy = xy_norot( theta );
    double cur_d2 = (curxy.x-px)*(curxy.x-px)  +(curxy.y-py)*(curxy.y-py) ;
    if ( cur_d2 < curdmin ) {
      curdmin = cur_d2;
      angmin = theta;
    }
  }
  for ( double theta=angmin-dang/10; theta<=angmin+dang/10; theta+=dang/100 ){
    xypoint curxy = xy_norot( theta );
    double cur_d2 = (curxy.x-px)*(curxy.x-px)  +(curxy.y-py)*(curxy.y-py) ;
    if ( cur_d2 < curdmin ) {
      curdmin = cur_d2;
      angmin = theta;
    }
  }
  
  xymin = xy( angmin );
      
  return curdmin;
}


// find smallest distance-squared from arbitrary point (px, py) to
// a point on the ellipse.
double ellipse_st::dmin2( const xypoint& p ) const {
  // work in coordinates of ellipse
  // Point becomes Y = (y0,y1)
  double px =  (p.x - c.x) * cos( phi ) + (p.y - c.y) * sin( phi ); 
  double py = -1.0 * (p.x - c.x) * sin( phi ) + (p.y - c.y) * cos( phi );

  std::vector< double >  ang;
  // pick initial angle range based on px,py
  if ( px > 0  && py > 0) { // first quadrant
    ang.push_back( 0. );
    ang.push_back( pi/2 ); 
  } else if ( px < 0 && py > 0 ) { // second quadrant
    ang.push_back( pi/2 );
    ang.push_back( pi ); 
  } else if ( px < 0 && py < 0 ) { // third quadrant
    ang.push_back( pi );
    ang.push_back( 3*pi/2 ); 
  } else {
    ang.push_back( 3*pi/2 );
    ang.push_back( 2*pi ); 
  }

  double curdmin = 999e99;
  double angmin=ang[0];
  double dang = (ang[1]-ang[0])/10;
  for ( double theta=ang[0]; theta<=ang[1]; theta+=dang ){
    xypoint curxy = xy_norot( theta );
    double cur_d2 = (curxy.x-px)*(curxy.x-px)  +(curxy.y-py)*(curxy.y-py) ;
    if ( cur_d2 < curdmin ) {
      curdmin = cur_d2;
      angmin = theta;
    }
  }
  for ( double theta=angmin-dang; theta<=angmin+dang; theta+=dang/10 ){
    xypoint curxy = xy_norot( theta );
    double cur_d2 = (curxy.x-px)*(curxy.x-px)  +(curxy.y-py)*(curxy.y-py) ;
    if ( cur_d2 < curdmin ) {
      curdmin = cur_d2;
      angmin = theta;
    }
  }
  for ( double theta=angmin-dang/10; theta<=angmin+dang/10; theta+=dang/100 ){
    xypoint curxy = xy_norot( theta );
    double cur_d2 = (curxy.x-px)*(curxy.x-px)  +(curxy.y-py)*(curxy.y-py) ;
    if ( cur_d2 < curdmin ) {
      curdmin = cur_d2;
      angmin = theta;
    }
  }
  
  return curdmin;
}



// faster method of finding smallest-squared distnce from point p 
// to a point on the ellipse. From:
// https://www.geometrictools.com/Documentation/DistancePointEllipseEllipsoid.pdf
/*
double ellipse_st::fast_dmin2(xypoint p) const{
  double dmin2;
  bool xflip;
  bool yflip;
  //work in coordinates of ellipse
  xypoint pprime((p.x-c.x)*cos(phi)+(p.y-c.y)*sin(phi), 
		 -1*(p.x-c.x)*sin(phi)+(p.y-c.y)*cos(phi));

  //do calculation as if point is in the first quadrant of the ellipse
  if(pprime.x < 0){
    pprime.x *= -1;
    xflip = true;
  }
  if (pprime.y < 0){
    pprime.y *= -1;
    yflip = true;
  }
  
  double a = b/sqrt(1-e*e);                //long axis of ellipse
  double tbar = Findtbar(a, pprime);       //find roots of F
  double xmin = a*a*pprime.x/(tbar+a*a);   //use tbar to get coordinates
  double ymin = b*b*pprime.y/(tbar+b*b);   //of closest point in ellipse
  //flip back to initial quadrant
  if (xflip) 
    xmin *= -1;
  if (yflip) 
    ymin *= -1;
  
  //calculate minimum distance squared
  dmin2 = (pprime.x-xmin)*(pprime.x-xmin) + (pprime.y-ymin)*(pprime.y-ymin);
  
  return dmin2;

}
*/

// function for which roots are found that give point on ellipse
// with minimum distance to point p
/*
double ellipse_st::Findtbar(double a, xypoint pprime) const{
  
  double t0 = -1*b*b+b*pprime.y; //staring left most point of biseciton
  
  //starting left most point of bisection
  double t1 = -1*b*b+sqrt(a*a*pprime.x*pprime.x + b*b*pprime.y*pprime.y);
  double f = F(a, pprime, t0, t1);
  
  int count =0;

  //do the bisection method to find the roots of F.
  // 0.01 can be reduced for more accurate roots
  while ( sqrt(f*f) > 0.01 ){
    //count++;
    //if(count<20){ 
    //  std::cout<<"t1 = "<<t1<<"\tt0 = "<<t0<<std::endl;
    //  std::cout<<"F = "<<f<<std::endl;
    //}
    
    if(f > 0) 
      t0 =(t1+t0)/2;
    if(f < 0) 
      t1 =(t1+t0)/2;
    f = F(a, pprime, t0, t1);
  }
  double tbar = (t1+t0)/2;
  
  return tbar;
}

double ellipse_st::F(double a, xypoint pprime, double t0, double t1) const{
  double tmp = (t1+t0)/2;

  return (a*pprime.x/(tmp+a*a))*(a*pprime.x/(tmp+a*a)) 
    + (b*pprime.y/(tmp+b*b))*(b*pprime.y/(tmp+b*b)) -1;
}
*/

double RobustLength( double v0, double v1 ){
  if ( v0 > v1 ) {
    return v0 * sqrt( 1 + (v1/v0)*(v1/v0) ); 
    
  } else {
    return v1 * sqrt( 1 + (v0/v1)*(v0/v1) ); 
  }
}

const unsigned MaxIterations = 1000;

double ellipse_st::GetRoot(double r0, double z0, double z1, double g) const{
  double n0 = r0 * z0;
  double s0 = z1 - 1;
  double s1 = ( g<0 ? 0 : RobustLength( n0, z0 ) -1 );
  double s = 0;
  
  for ( unsigned i=0; i < MaxIterations; ++i ){
    s = (s0 + s1)/2;
    if( s == s0 || s == s1) break;
    
    double ratio0 = n0 / ( s + r0 );
    double ratio1 = z1 / ( s + 1 );
    g = ratio0 * ratio0 + ratio1 * ratio1 - 1;
    
    if( g > 0 ){
      s0 = s;
    } else if ( g < 0 ){
      s1 = s;
    } else {
      break;
    }
  }  
  return s;
}

double ellipse_st::DistancePointEllipse2(xypoint p, xypoint& closestxy, int& kind ) const{
  // work in coordinates of ellipse
  // Point becomes Y = (y0,y1)
  double y0 =  (p.x - c.x) * cos(phi) + (p.y - c.y) * sin( phi ); 
  double y1 = -1.0 * (p.x - c.x) * sin( phi ) + (p.y - c.y) * cos(phi);

  // calculation seems to fail when near y-axis
  // and outside ellipse
  bool rotate90=false;
  if ( ! is_inside( p ) ){
    if ( y0 == 0 || fabs( y1 / y0 ) > 1.0 ) { // rotate 90 deg clockwise
      rotate90=true;
      double tmp0 = y0;
      y0 = y1;
      y1 = -tmp0;
    }
  }

  //do calculation as if point is in the first quadrant of the ellipse
  bool y0flip = false, y1flip = false;
  if ( y0 < 0 ) {
    y0 *= -1;
    y0flip = true;
  }
  if ( y1 < 0 ){
    y1 *= -1;
    y1flip = true;
  }
  
  double x0 = 0., x1 = 0.;
  double dmin2 = 0.;
  double e0 = b / sqrt( 1.0 - e * e );   //long axis of ellipse
  double e1 = b; // short axis of ellipse
  if ( rotate90 ) {
    double tmp0 = e0;
    e0=e1;
    e1=tmp0;
  }
  kind=0;
  if( y1 > 0 ) {
    if ( y0 > 0 ) {
      double z0 = y0 / e0; //e0>e1 -> a>b
      double z1 = y1 / e1;
      double g = z0 * z0 + z1 * z1 - 1.0;
      if( g != 0 ){
	double r0 = ( e0 / e1 ) * ( e0 / e1 );
	double sbar = GetRoot( r0, z0, z1, g );
	x0 = r0 * y0 / ( sbar + r0 );
	x1 = y1 / ( sbar + 1.0 );
	dmin2 = ( x0 - y0 ) * ( x0 - y0 ) +  ( x1 - y1 ) * ( x1 - y1 );
	kind =1;
      } else { // right on ellipse
	x0 = y0;
	x1 = y1;	
	dmin2 = 0;
	kind = 2;
      }
    } else { // y0 == 0
      x0 = 0;
      x1 = e1;
      dmin2 = (y1 - e1) * (y1 - e1);
      kind=3;
    }
  } else { // y1 == 0
    double number0 = e0 * y0;
    double denom0 = e0 * e0 - e1 * e1;
    if( number0 < denom0 ){
      double xde0 = number0 / denom0;
      x0 = e0 * xde0;
      x1 = e1 * sqrt( 1 - xde0 * xde0 );
      dmin2 = (x0 - y0 ) * (x0 - y0 )  +  x1 * x1;
      kind=4;
    } else { // right on ellipse
      x0 = e0;
      x1 = 0;
      dmin2 = ( y0 - e0 ) * ( y0 - e0 );      
      kind=5;
    }
  }

  // move to correct quadrant
  if ( y0flip ) { x0 *= -1; kind += 10; }
  if ( y1flip ) { x1 *= -1; kind += 100; }

  if ( rotate90 ) {//unrotate 90
    double tmp0 = x0;
    x0 = -x1;
    x1 = tmp0;
  }
    
  // de-rotate
  double x0r = x0 * cos( phi ) - x1 * sin( phi );
  double x1r = x0 * sin( phi ) + x1 * cos( phi );
  // de-translate
  x0r += c.x;
  x1r += c.y;
  closestxy = xypoint( x0r, x1r );

  return dmin2;  
}


// find closest point on ellipse that is smallest distance from 
// arbitrary point (px, py) to a point on the ellipse.
xypoint ellipse_st::closest_point( const xypoint& p ) const{
  xypoint closest;
  dmin2( p, closest);
  return closest;
}


ostream& operator<<( ostream& os, const ellipse_st &  e ){

  os << "center (x,y) = (" << e.c.x <<", " << e.c.y << ") "
     << " short radius b = " << e.b
     << " eccentricity = " << e.e
     << " phi = " << e.phi
     <<std::endl;
  return os;
}

