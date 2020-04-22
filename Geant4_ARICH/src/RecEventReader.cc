#include <TChain.h>
#include <fstream>
#include <iostream>

#include "RecEventReader.hh"

using std::ifstream;


TChain* ReadListFile(std::string list, RecEvent& event){
  std::cout<<"ReadListFile "<<list<<std::endl;
  event.pid = NULL;
  event.xSim = NULL;
  event.ySim = NULL;
  event.txSim = NULL;
  event.tySim = NULL;
  event.qdpSim = NULL;
  event.xRec = NULL;
  event.yRec = NULL;
  event.txRec = NULL;
  event.tyRec = NULL;
  event.qdpRec = NULL;
  event.covMat = NULL;
  
  TChain *chain = new TChain("recTracks");
  
  ifstream listFile (list);
  
  if (listFile.is_open())
    {
      
      std::string line;
      while ( getline (listFile,line) )
	{
	  std::cout<<"Adding file "<<line.c_str()
		   <<" to list of root vectors to read"<<std::endl; 
	  chain->Add(line.c_str());
	}    
      listFile.close();
    }
  else {
    std::cerr << "Unable to open file" << std::endl;	
    exit(100);
  }
  
  chain->SetBranchAddress("eventID", &(event.eventID));
  chain->SetBranchAddress("pid", &(event.pid));
  chain->SetBranchAddress("xSim", &(event.xSim));
  chain->SetBranchAddress("ySim", &(event.ySim));
  chain->SetBranchAddress("txSim", &(event.txSim));
  chain->SetBranchAddress("tySim", &(event.tySim));
  chain->SetBranchAddress("qdpSim", &(event.qdpSim));
  chain->SetBranchAddress("xRec", &(event.xRec));
  chain->SetBranchAddress("yRec", &(event.yRec));
  chain->SetBranchAddress("txRec", &(event.txRec));
  chain->SetBranchAddress("tyRec", &(event.tyRec));
  chain->SetBranchAddress("qdpRec", &(event.qdpRec));
  chain->SetBranchAddress("covMat", &(event.covMat));
  
  std::cout<<"Done ReadListFile."<<std::endl;
  return chain;
}
