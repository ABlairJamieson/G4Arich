
#include <TTree.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TProfile.h>
#include <TVector3.h>
#include <TF1.h>
#include <TGraph.h>

#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <fstream>

#include "TruthTTree.hh"
#include "PTFTTree.hh"
#include "ellipse.h"
#include "dbscan.h"
#include "ringfitchi2.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;


void event_display( const unsigned long long iev, 
		    const int xmin, const double xmax, 
		    const double ymin, const double ymax, 
		    const vector< xypoint >& pts, const ellipse_st& eans, TH2D* hall, 
		    TruthTTree * truth, TH2D *bMeas ){

  TVector3 ptrack( truth->Px[0], truth->Py[0], truth->Pz[0] );
  double pmag = ptrack.Mag();
  
  bMeas->Fill(pmag,eans.get_b());
  std::cout<<"-------------------------end of iev: "<<iev<<"---------------------"<<std::endl;

  if (iev>100) { cout<<"Not drawing past 100th event"<<endl;  return; }
  // build histo name
  char hname[100];
  sprintf( hname, "hev%02d", iev );
  // figure out how many bins
  int nx = (xmax - xmin)/6+1;
  int ny = (ymax - ymin)/6+1;

  TH2D* hdata = new TH2D( hname, " ; X (mm); Y (mm);", nx, xmin-3, xmax+3, ny, ymin-3, ymax+3 );
  // fill histogram
  for ( const xypoint & xy : pts ){
    hdata->Fill( xy.x, xy.y );
    hall->Fill( xy.x, xy.y );
  }
  // make graph for the best ellipse
  vector<double> xpoints;
  vector<double> ypoints;
  
  for (int i=0; i<180; ++i ){
    double theta = i*2*pi/180;
    xypoint p = eans.xy( theta ); 
    xpoints.push_back( p.x );
    ypoints.push_back( p.y );
  }
  TGraph * g = new TGraph( xpoints.size(), &xpoints[0], &ypoints[0] );
  g->SetLineWidth(3); 
  g->SetLineColor(kRed);
  hdata->GetListOfFunctions()->Add( g );
  hall->GetListOfFunctions()->Add( g );
}


void remove_outliers( vector< xypoint > & pts ){
  vector<xypoint> result;
  
  for ( int i = 0; i< pts.size() ; ++i  ){
    for ( int j= 0; j< pts.size() ; ++j ){
      if ( i==j ) continue;
      double dmin = sqrt( ( pts[i].x - pts[j].x )*( pts[i].x - pts[j].x ) + 
			  ( pts[i].y - pts[j].y )*( pts[i].y - pts[j].y ) ); 
      
      if ( dmin < 35.0 ) {
	result.push_back( pts[i] );
	break;
      }
    }
  }

  pts = result;
}

void get_means_rms( const vector<Point>& pts, double &meanx, double& meany, double &rms ){
  meanx=0;
  meany=0;
  rms=0;
  int npts = pts.size();
  double rmsx=0, rmsy=0;
  double sumx=0, sumy=0;
  double sum2x=0, sum2y=0;
  if ( npts == 0 ) return;
  for ( const Point& p : pts ){
    sumx += p.x;
    sumy += p.y;
    sum2x += p.x*p.x;
    sum2y += p.y*p.y;
  }
  meanx = sumx/npts;
  meany = sumy/npts;
  if (npts>1){
    rmsx = ( sum2x - sumx*meanx ) / (npts-1) ;
    rmsy = ( sum2y - sumy*meany ) / (npts-1) ;
    rms = sqrt( rmsx + rmsy );
  }

}


void printResults(vector<Point>& points, int num_points)
{
    int i = 0;
    printf("Number of points: %u\n"
        " x     y     z     cluster_id\n"
        "-----------------------------\n"
        , num_points);
    while (i < num_points)
    {
          printf("%5.2lf %5.2lf %5.2lf: %d\n",
                 points[i].x,
                 points[i].y, points[i].z,
                 points[i].clusterID);
          ++i;
    }
}


void rich_analysis( char * infilename = "geant4ptf_000000.root", unsigned long long nevmax =5 ){

  ofstream bFile;
  bFile.open("MeasuredB.txt");

  TFile * fin = new TFile( infilename, "read" );
  TruthTTree * truth = new TruthTTree;
  PTFTTree * ptf = new PTFTTree;

  TTree * truth_tree = (TTree*) fin->Get("truth_tree");
  TTree * ptf_tree = (TTree*) fin->Get("ptf_tree");

  truth->SetBranchAddresses( truth_tree );
  ptf->SetBranchAddresses( ptf_tree );

  TFile * fout = new TFile( "rich_analysis.root", "recreate" );
  TH2D * hall_rings = new TH2D("hall_rings"," ; X (mm) ; Y(mm )",100,-297.0,303.0,100,-297.0,303.0 );
  TH2D *bMeas = new TH2D("b", "b", 150, 0, 15, 100, 0, 100);

  // prepare root minimizer
  ROOT::Math::Minimizer* minalg =
    ROOT::Math::Factory::CreateMinimizer("Minuit2", "");
  // set tolerance , etc...
  minalg->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
  minalg->SetMaxIterations(10000);  // for GSL
  minalg->SetTolerance(0.001);
  minalg->SetPrintLevel(1);
  
  unsigned long long nevents = ptf_tree->GetEntries();
  for (unsigned long long iev =0 ; iev < std::min( nevmax, nevents); ++iev ){
    truth_tree->GetEvent( iev );
    ptf_tree->GetEvent( iev );

    // loop over all photons in event
    vector< Point > pts3d;
    for ( int iph = 0; iph < ptf->NPhotons; ++iph ){
      // only look at detected photons:
      //if ( ptf->true_used[ iph ] == false ) continue;
      // only look at photons from aerogel:
      if ( ptf->true_ini_z[ iph ] <0 || ptf->true_ini_z[ iph ] > 40.0 ) continue;
      // apply quantum efficiency!
      if ( !ptf->true_used[ iph ] ) continue;

      // digitize the points.
      double curx = ptf->true_x[ iph ];      
      double cury = ptf->true_y[ iph ];
      int ix = curx/6;
      int iy = cury/6;
      double digix = ix * 6.0;
      double digiy = iy * 6.0;
      //pts.push_back( xypoint( digix, digiy ) ); 
      pts3d.push_back( Point( digix, digiy, 0.0, UNCLASSIFIED ) );
    }

    // cluster the points
    // ds( minpoints, distance-squared, points )
    DBSCAN ds( 3, 40*40, pts3d );
    ds.run();


    // for now try fitting to cluster with most points only
    // find biggest cluster
    int bigest=0;
    int idxbigest=-1;
    for (int icluster=0; icluster<ds.get_num_clusters(); ++icluster){
      vector<Point> vclust = ds.get_cluster( icluster );
      double mx,my,rms;
      get_means_rms( vclust, mx, my, rms );
      cout<<"icluster="<<icluster<<" ntps="<<vclust.size()<<" mx="<<mx<<" my="<<my<<" rms="<<rms<<endl;
      if ( vclust.size() > bigest ){
	bigest = vclust.size();
	idxbigest = icluster;
      }
    }

    vector< xypoint > pts;
    vector<Point> vclust = ds.get_cluster( idxbigest );
    std::cout<<"Using cluster "<<idxbigest<<" with "<<vclust.size()<<" hits"<<endl; 

    for ( int ipt = 0; ipt < vclust.size(); ++ipt ){
      pts.push_back( xypoint( vclust[ipt].x, vclust[ipt].y ) );
    }


    if ( pts.size() == 0 ) {
      std::cout<<"Skipping event "<<iev<<" that has no detected photons"<<endl;
      continue;
    }

    //remove_outliers( pts );

    if ( pts.size() == 0 ) {
      std::cout<<"Skipping event "<<iev<<" that has no detected photons after remove_outliers"<<endl;
      continue;
    }

    // find range of values
    double xmin = 10000.0; // 10 meters in mm
    double xmax = -10000.0; 
    double ymin = 10000.0; // 10 meters in mm
    double ymax = -10000.0; 
    for ( xypoint xy : pts ){
      double digix = xy.x;
      double digiy = xy.y;
      if ( digix < xmin ) xmin = digix;
      if ( digiy < ymin ) ymin = digiy;
      if ( digix > xmax ) xmax = digix;
      if ( digiy > ymax ) ymax = digiy;
    }

    // now I have one event's digitized hits in pts
    // build functor to do the fit
    ringfitchi2 * fringchi2 = new ringfitchi2( pts );
    ROOT::Math::Functor *fcn = new ROOT::Math::Functor(fringchi2, &ringfitchi2::operator(), 5);
    minalg->SetFunction( *fcn );

    // Set the free variables to be minimized !
    vector< string > parnames { "b  ","e  ","phi","xc ","yc " };
    double step[5] = {0.01,0.01,0.01,0.01,0.01};
    double pars[5] = { (xmax-xmin)/2., 
		       0.0, 
		       0.0, 
		       (xmin+xmax)/2, 
		       (ymin+ymax)/2  };  
    for ( int i=0; i<5; ++i ){
      minalg->SetVariable( i, parnames[i].c_str(), pars[i], step[i] );
    }


    // do the minimization
    minalg->Minimize();
 
    const double *xs = minalg->X();
    ellipse_st eans( xs[0], xs[1], xs[2], xypoint( xs[3], xs[4] ) );
    std::cout << "Minimum: chi2 = " << minalg->MinValue()
	      << " at " << eans << std::endl;


    //output measured b to text file
    TVector3 ptrack( truth->Px[0], truth->Py[0], truth->Pz[0] );
    double pmag = ptrack.Mag();

    //calculate Cherenkov radius from truth information
    double dmin2 =0;

    for (xypoint pt: pts){
      dmin2 += eans.fast_dmin2(pt);
     }
    dmin2 /= pts.size();
    //std::cout<<"average dmin2 = "<<dmin2<<std::endl;

    bFile<<xs[0]<<"\t"<<pmag<<"\n";
  
    // plot the result
    event_display( iev, xmin, xmax, ymin, ymax, pts, eans, hall_rings, truth, bMeas );
 

  }
  bFile.close();
  fout->Write();
}


int main(int argc, char * argv[]){


  if ( argc <= 1 ) {
    rich_analysis( "geant4ptf_000000.root", 5 );
  } else if ( argc <= 2 ){
    rich_analysis( argv[1], 5 );
  } else {
    rich_analysis( argv[1], atoi( argv[2] ) );
  }
  return 0;
}
