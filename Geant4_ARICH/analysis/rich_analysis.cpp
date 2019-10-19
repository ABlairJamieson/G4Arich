
#include <TTree.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TProfile.h>
#include <TVector3.h>
#include <TF1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TBox.h>
#include "TEllipse.h"

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
#include "hough.h"
#include "ellipsefitter.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;


// new event_display
void new_event_display( const unsigned long long iev,
			CircleHough & ch,
			const HoughResults & hcr,
			const EllipseFitter & el ){

  /// only draw first 11 events
  if (iev>12) return;
  
  TDirectory * curdir = gDirectory;
  std::string dirname = std::string{"newdisplay_"}+std::to_string( iev );
  TDirectory * evdir = curdir->mkdir( dirname.c_str() );
  evdir->cd();
  
  const std::vector<int> colors = { kRed   , kBlue, kGreen,   kOrange,
				    kViolet, kCyan, kMagenta, kPink+6 };   

  // build histogram range from transformed histogram
  std::vector< TH2D* > vcht = ch.get_transform();
  if ( vcht.size() < 1 ) return;
  unsigned nbins_x = vcht[0]->GetNbinsX();
  unsigned nbins_y = vcht[0]->GetNbinsX();
  double   xmin    = vcht[0]->GetXaxis()->GetXmin();
  double   xmax    = vcht[0]->GetXaxis()->GetXmax();
  double   ymin    = vcht[0]->GetYaxis()->GetXmin();
  double   ymax    = vcht[0]->GetYaxis()->GetXmax();
  // build histogram name
  std::string hname = std::string{"evdisp_"} + std::to_string( iev );
  TH2D* hist = new TH2D( hname.c_str(), " ; X (cm); Y (cm) ",
			 nbins_x, xmin, xmax,
			 nbins_y, ymin, ymax );

  // put the histogram on a canvas
  std::string canname = std::string{"tc_"} + hname;
  TCanvas * tc = new TCanvas( canname.c_str(), canname.c_str() );
  tc->cd();
  hist->Draw();

  // Now we will add each of the datapoints color coded for each ellipse
  float xbwid = (xmax-xmin)/nbins_x;
  float ybwid = (ymax-ymin)/nbins_y;
  for ( unsigned iel = 0; iel < hcr.size(); ++iel ){
    int curcol = 0;
    if ( iel<colors.size() ) curcol = colors[iel];
    if ( hcr[iel].type == HoughUnusedPoints ) curcol=kGray;
    
    // Add datapoints as boxes to the plot
    for ( const xypoint & pt : hcr[iel].data ){
      TBox *b =new TBox( pt.x-xbwid, pt.y-ybwid, pt.x+xbwid, pt.y+ybwid );
      b->SetFillStyle(1001);
      b->SetFillColor( curcol );
      b->Draw();
      b->SetBit( kCanDelete );
      b->SetBit( kMustCleanup );
    }
    if ( hcr[iel].type == HoughUnusedPoints ) continue;
    
    // Draw circle from hough transform
    TEllipse *tel= new TEllipse( hcr[iel].xyc.x, hcr[iel].xyc.y, hcr[iel].rc);
    tel->SetFillStyle(0);
    tel->SetLineColor( curcol );
    tel->Draw();
    hist->GetListOfFunctions()->Add( tel );
    
    // Draw ellipse
    // First get the ellipse from the fit params
    ellipse_st eans( el.b[iel], el.e[iel], el.phi[iel], xypoint( el.x[iel], el.y[iel] ) );
    
    // make graph for the best ellipse
    vector<double> xpoints;
    vector<double> ypoints;
  
    for (unsigned i=0; i<180; ++i ){
      double theta = i*2*pi/180;
      xypoint p = eans.xy( theta ); 
      xpoints.push_back( p.x );
      ypoints.push_back( p.y );
    }
    TGraph * g = new TGraph( xpoints.size(), &xpoints[0], &ypoints[0] );
    // name the graph?
    std::string gname = std::string{"gel_"}+hname;
    g->SetName( gname.c_str() );
    g->SetLineWidth(2); 
    g->SetLineColor( curcol + 2 );
    g->Draw("lsame");
    hist->GetListOfFunctions()->Add( g );
  }

  tc->Write();
  curdir->cd();
}




void rich_analysis( char * infilename = "geant4ptf_000000.root", unsigned long long nevmax =5 ){

  // Open input file for reading events
  TFile * fin = new TFile( infilename, "read" );

  // Setup readin of TTrees from input file
  TruthTTree * truth = new TruthTTree;
  PTFTTree * ptf = new PTFTTree;
  TTree * truth_tree = (TTree*) fin->Get("truth_tree");
  TTree * ptf_tree = (TTree*) fin->Get("ptf_tree");
  truth->SetBranchAddresses( truth_tree );
  ptf->SetBranchAddresses( ptf_tree );

  // Setup ouput file for ring fit results
  TFile * fout = new TFile( "rich_analysis.root", "recreate" );
  fout->cd();

  /// create results TTree
  TTree* ttfits = new TTree("ellipsefits","ellipsefits");
  EllipseFitter elfit;
  elfit.MakeTTreeBranches( ttfits );
  
  /// Setup hough transform class 
  CircleHough hc;
  hc.set_distance_factor( 2.0 );

  // loop over events in input TTree
  unsigned long long nevents = ptf_tree->GetEntries();
  for (unsigned long long iev =0 ; iev < std::min( nevmax, nevents); ++iev ){
    if ( iev%10 == 0 ){
      std::cout<<"Analyze event "<<iev<<" of "<<std::min( nevmax, nevents)<<std::endl;
    }
    truth_tree->GetEvent( iev );
    ptf_tree->GetEvent( iev );

    // loop over all photons in event
    vector< xypoint > pts;
    for ( int iph = 0; iph < ptf->NPhotons; ++iph ){
      // apply quantum efficiency!
      if ( !ptf->true_used[ iph ] ) continue;

      // digitize the points.
      double curx = ptf->true_x[ iph ];      
      double cury = ptf->true_y[ iph ];
      int ix = curx/6;
      int iy = cury/6;
      double digix = ix * 6.0;
      double digiy = iy * 6.0;
      // check existence of point
      bool exists=false;
      for ( const xypoint& pt : pts ){
	if ( pt.x == digix && pt.y == digiy ) {
	  exists = true;
	  break;
	}
      }
      // only add one point per pixel
      if ( !exists ){
	pts.push_back( xypoint( digix, digiy ) );
      }
    }

    HoughResults hcr = hc.find_circles( pts );
    //std::cout << hcr << std::endl;

    /// Fit all circles
    elfit.fit_rings( hcr, ptf->true_idx );
    /// Save results to TTree
    ttfits->Fill();

    if ( iev%1000 == 500 ) ttfits->SetAutoSave();

    // plot the result
    new_event_display( iev, hc, hcr, elfit );

  }

  fout->Write();
  fout->Close();
  fin->Close();
  // cleanup
  if (fin!=nullptr) delete fin;
  if (fout!=nullptr) delete fout;
  if (truth!=nullptr) delete truth;
  if (ptf!=nullptr) delete ptf;
  //if (truth_tree!=nullptr) delete truth_tree;
  //if (ptf_tree!=nullptr) delete ptf_tree;
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
