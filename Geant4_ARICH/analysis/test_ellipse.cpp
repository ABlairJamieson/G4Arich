
#include "ellipse.h"

void test_ellipse(){
  xypoint center( 100., -400. );
  double  e = 0.9;
  double  b = 100.0;
  double  phi  = pi/3;

  ellipse_st e1( b, e, phi, center );

  vector<double> xpoints;
  vector<double> ypoints;
  
  for (int i=0; i<360; ++i ){
    double theta = i*2*pi/360;
    xypoint p = e1.xy( theta ); 
    xpoints.push_back( p.x );
    ypoints.push_back( p.y );
  }


  TH2D   * hpane = new TH2D("hpane","hpane",100,-800.,800.,100,-800.,800.);

  TGraph * g = new TGraph( xpoints.size(), &xpoints[0], &ypoints[0] );
  g->SetLineColor(kRed);
  g->SetLineWidth(2);
  hpane->Draw();
  g->Draw("L");


  // test distance to various points:
  for ( unsigned i=0; i<1000; ++i ){
    double x = gRandom->Rndm()*600.0 - 200.0;
    double y = gRandom->Rndm()*700.0 - 700.0;
      xypoint xymin;
      double dmin2 = e1.dmin2(  xypoint(x,y), xymin );

      TLine * tl = new TLine( x, y, xymin.x, xymin.y );
      TMarker* tm = new TMarker( x, y, 7 );
      if ( e1.is_inside( xypoint(x,y) )  ) {
	tl->SetLineColor(kGreen);
	tm->SetMarkerColor( kGreen+2 );
      } else {
	tl->SetLineColor(kBlue);
	tm->SetMarkerColor( kBlue+2 );
      }
      tl->Draw("same");
      tm->Draw("same");
      
  }
}
