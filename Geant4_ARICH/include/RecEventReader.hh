#ifndef _RecEventReader_hh_
#define _RecEventReader_hh_


#include <vector>
#include <string>
using std::vector;
using std::string;

class TChain;





/// RecEvent structure is used to hold input event vectors
/// in root format that can be used to generate events when
/// the /gun/source is set to root.
/// Format of the root file is as follows.  Dimensions are cm. 
/*
Matej Pavin <MPavin@triumf.ca>
	
AttachmentsJun 16, 2019, 11:20 PM (11 hours ago)
	
to Gabriel, bl.jamieson@uwinnipeg.ca, me
Hi all,

I simulated 1.5*10^6 30 GeV/c protons hitting a 2 cm thick carbon
target. The beam is a 2D Gaussian with a width of 0.75 cm in both x
and y-axis.  The start of the aerogel is at z = 40 cm, and the center
of the target is at z = 0.5 cm.  During the reconstruction stage, I
only kept events which have pions, protons, or kaons between 0.5 and
12 GeV/c with an outgoing angle less than 300 mrad. The selected
tracks are reconstructed, and the track parameters are stored at
aerogel z position (download root file:
https://drive.google.com/file/d/1QSFJzjpknePqa_CdkfKzX436CA_kBNlC/view?usp=sharing).

List of variables stored in the MC file:
eventID - integer, starts at 1
pid        - vector<int>, a vector of particle IDs for all tracks in the given event
xSim     - vector<double>, a vector of simulated track x positions at aerogel z position
ySim     - vector<double>, a vector of simulated track y positions at aerogel z position
txSim     - vector<double>, a vector of simulated track px/pz at aerogel z position
tySim     - vector<double>, a vector of simulated track py/pz at aerogel z position
qdpSim - vector<double>, a vector of simulated charge/momentum values at aerogel z position
 
xRec     - vector<double>, a vector of reconstructed track x positions at aerogel z position
yRec     - vector<double>, a vector of reconstructed track y positions at aerogel z position
txRec     - vector<double>, a vector of reconstructed track px/pz at aerogel z position
tyRec     - vector<double>, a vector of reconstructed track py/pz at aerogel z position
qdpRec - vector<double>, a vector of reconstructed charge/momentum values at aerogel z position

covMat - vector<vector<double> >, a vector containing covariance
matrices for all tracks in the selected event
             - each covariance matrix is a vector with 15 numbers stored in it
             - upper triangle of the matrix is stored
             - stored covariance matrix elements: 
C(x,x), C(x,y), C(x,tx), C(x,ty), C(x,qdp), 
C(y,y), C(y,tx), C(y,ty), C(y,qdp), 
C(tx,tx), C(tx,ty), C(tx,qdp), 
C(ty,ty), C(ty,qdp), 
C(qdp,qdp)   */
struct RecEvent{
	int eventID;
	vector<int> *pid;
	vector<double> *xSim;
	vector<double> *ySim;
	vector<double> *txSim;
	vector<double> *tySim;
	vector<double> *qdpSim;
	vector<double> *xRec;
	vector<double> *yRec;
	vector<double> *txRec;
	vector<double> *tyRec;
	vector<double> *qdpRec;
	vector<vector<double> > *covMat;
};


// Helper function to InitializeRootVectors is ReadListFile
// ReadListFile, sets up TChain and points BranchAddresses
// so that RecEvent will be filled when TChain::GetEvent
// is later called.
TChain* ReadListFile(string list, RecEvent& event);


#endif //_RecEventReader_hh_
