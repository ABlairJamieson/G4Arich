
#include <TTree.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TGraph.h>
#include <TMatrixD.h>
#include <TChain.h>

#include <vector>
#include <iostream>
#include <cstdio>

#include "TruthTTree.hh"
#include "PTFTTree.hh"
#include "RecEventReader.hh"

#include "ellipse.h"
#include "hough.h"
#include "ellipsefitter.h"
#include "ringfitchi2.h"
#include "eventdisplay.h"

using std::vector;
using std::cout;
using std::endl;

vector< TMatrixD > RecEventInvCovMat( const RecEvent & rev ){
  std::cout<<"RecEvent eventID="<<rev.eventID<<std::endl;
  vector < TMatrixD > invcovmats;
  /// only need covariance in x,y,tx,ty
  for ( unsigned i=0; i<rev.pid->size(); ++i ){
    double cov[16] = { rev.covMat->at(i)[0],  rev.covMat->at(i)[1],  rev.covMat->at(i)[2],  rev.covMat->at(i)[3],  
		       rev.covMat->at(i)[1],  rev.covMat->at(i)[4],  rev.covMat->at(i)[5],  rev.covMat->at(i)[6],
		       rev.covMat->at(i)[2],  rev.covMat->at(i)[5],  rev.covMat->at(i)[7],  rev.covMat->at(i)[8],
		       rev.covMat->at(i)[3],  rev.covMat->at(i)[7],  rev.covMat->at(i)[8],  rev.covMat->at(i)[9] };
    TMatrixD covmat(4,4);
    covmat.SetMatrixArray( cov );
    TMatrixD invcovmat = covmat.InvertFast();
    invcovmats.push_back( invcovmat );
  }
  return invcovmats;
}

/// CircleRecEventMatcher takes RecEvent data at finds which
/// Hough circle goes with which RecEvent
/// The matching is done based on smallest difference between circle centers.
struct CircleRecEventMatcher {

  CircleRecEventMatcher();

  void match_circles( const RecEvent &rev, const HoughResults & hcr, const TruthTTree& truth );

  /// getters don't check anything, so make sure your index is okay!
  unsigned size() const { return matches.size(); }
  unsigned get_rec_event_idx( unsigned houghidx ) const { return matches[ houghidx ]; }
  double   get_match_distance( unsigned houghidx) const { return matchdist[ houghidx ]; }
  
private:
  void   book_histograms();
  TH2D * hnhough_vs_ntrue; // number of hough circles vs true tracks
  TH2D * hnhough_vs_nrec;  // number of hough circles vs rec tracks
  TH2D * hnrec_vs_ntrue;   // number of rec tracks vs true tracks
  TH1D * hhoughdr;         // shortest distance between centers (hough vs rec)
  TH1D * hhoughdr_true;    // shortest distance between centers (hough vs true)
  TH1D * hrecdr_true;      // shortest distance between centers (rec   vs true)

  std::vector< unsigned>  matches;         // index into RecEvents for each HoughCircle
  std::vector< double >   matchdist;       // distance of closest match
};

void CircleRecEventMatcher::book_histograms() {
  hnhough_vs_ntrue = new TH2D("hnhough_vs_ntrue", " Num Hough vs True ; N_{true}; N_{hough}",       10,-0.5,9.5,10,-0.5,9.5);
  hnhough_vs_nrec  = new TH2D("hnhough_vs_nrec" , " Num Hough vs Recon Tracks ; N_{rec}; N_{hough}",10,-0.5,9.5,10,-0.5,9.5);
  hnrec_vs_ntrue   = new TH2D("hnrec_vs_ntrue"  , " Num Rec vs True Tracks ; N_{true}; N_{rec}",    10,-0.5,9.5,10,-0.5,9.5);
  hhoughdr         = new TH1D("hhoughdr"        , " ; | r_{hough} - r_{rec} | ; Counts / bin",  100, 0.,200.0 );
  hhoughdr_true    = new TH1D("hhoughdr_true"   , " ; | r_{hough} - r_{true} | ; Counts / bin", 100, 0.,200.0 );
  hrecdr_true      = new TH1D("hrecdr_true"     , " ; | r_{rec} - r_{true} | ; Counts / bin",   100, 0.,200.0 );
}

CircleRecEventMatcher::CircleRecEventMatcher() {
  book_histograms();
}


void CircleRecEventMatcher::match_circles( const RecEvent &rev, const HoughResults & hcr, const TruthTTree& truth ){
  matches.clear();
  matchdist.clear();
  
  hnhough_vs_ntrue->Fill( truth.NTracks,    hcr.size() );
  hnhough_vs_nrec ->Fill( rev.xRec->size(), hcr.size() );
  hnrec_vs_ntrue  ->Fill( truth.NTracks,    rev.xRec->size() );
  
  for ( const HoughResult& hr : hcr ){
    double mdist =9e99;
    unsigned midx = 0;
    // calculate (hough - rec)
    for ( unsigned idx = 0; idx < rev.xRec->size(); ++idx ){
      // propagate input track from aerogel to detector plane
      double x = rev.xRec->at(idx) + 20.0 * rev.txRec->at(idx);  
      double y = rev.yRec->at(idx) + 20.0 * rev.tyRec->at(idx);  
      double dist = sqrt( ( x - hr.xyc.x ) * ( x - hr.xyc.x )  +
			  ( y - hr.xyc.y ) * ( y - hr.xyc.y )  );
      if ( dist < mdist ){
	mdist = dist;
	midx = idx;
      }
    }
    matches.push_back( midx );
    matchdist.push_back( mdist );
    hhoughdr->Fill( mdist );


    double mdist_true =9e99;
    // calculate (hough - true)
    for ( unsigned idx = 0; idx < truth.NTracks; ++idx ){
      // propagate input track from aerogel to detector plane
      double x = truth.VtxX[idx] + 20.0 * truth.Px[idx] / truth.Pz[idx];
      double y = truth.VtxY[idx] + 20.0 * truth.Py[idx] / truth.Pz[idx];
      double dist = sqrt( ( x - hr.xyc.x ) * ( x - hr.xyc.x )  +
			  ( y - hr.xyc.y ) * ( y - hr.xyc.y )  );
      if ( dist < mdist_true ){
	mdist_true = dist;
      }
    }
    hhoughdr_true->Fill( mdist_true );
  }


  // calculate (rec - true )
  for ( unsigned idx = 0; idx < rev.xRec->size(); ++idx ){
    double mdist_true =9e99;
    // propagate input track from aerogel to detector plane
    double x = rev.xRec->at(idx) + 20.0 * rev.txRec->at(idx);  
    double y = rev.yRec->at(idx) + 20.0 * rev.tyRec->at(idx);
    for ( unsigned jdx = 0; jdx < truth.NTracks; ++jdx ){
	  // propagate input track from aerogel to detector plane
      double xt = truth.VtxX[jdx] + 20.0 * truth.Px[jdx] / truth.Pz[jdx];
      double yt = truth.VtxY[jdx] + 20.0 * truth.Py[jdx] / truth.Pz[jdx];
      double dist = sqrt( ( x - xt ) * ( x - xt )  +
			  ( y - yt ) * ( y - yt )  );
      if ( dist < mdist_true ){
	mdist_true = dist;
      }
    }
    hrecdr_true->Fill( mdist_true );
  }
}

ostream& operator<<( ostream& os, const CircleRecEventMatcher & crem ){
  for ( unsigned i=0; i<crem.size(); ++i ){
    os<<"Hough-circle: "<<i<<" rec track: "<<crem.get_rec_event_idx(i)<<" dist = "<<crem.get_match_distance(i)<<std::endl;
  }
  return os;

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

  // Setup readin of TTree from input to input file
  // It includes a fit to particle tracks from silicon strip detectors
  RecEvent ssrecev;
  TChain * sstruth = ReadListFile( "root_vector_files.txt", ssrecev );

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


  CircleRecEventMatcher crem;

  
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

    /// Load entry for silicon strip detector and truth input
    std::cout<<"ptf->root_eventid="<<ptf->root_eventid
	     <<"ptf->root_idx="<<ptf->root_idx<<std::endl;
    sstruth->GetEntry( ptf->root_idx ); 
    /// Get inverse covariance matrices for this event
    vector< TMatrixD > invcov = RecEventInvCovMat( ssrecev );

    /// Match reconstructed tracks to hough circles
    crem.match_circles( ssrecev, hcr, *truth );
    
    cout << crem ;
    
    /// Fit all circles
    elfit.fit_rings( hcr, ptf->root_eventid );
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
