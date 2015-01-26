
#ifndef SLICEPLOTS
#define SLICEPLOTS

//Root libs
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"


//c++ libs
#include <iostream>
#include <vector>
#include <sstream> 
#include <iomanip>
#include <limits>
#include <string>
#include "TROOT.h"
#include "TStyle.h"

using namespace std;


struct HistoSlices{
  vector<TH1D*> slices;
  vector<double> sliceMean;
  bool b_denom; 
};



class slicePlots{
 public:
  slicePlots(string saveName);
  ~slicePlots();

  void addFile(string filedir){filedirs.push_back(filedir);}
  void setupDenom(string denominator){file_denom = denominator;}

  void setupRMSAxis( double RMSYmax, double RMSYmin){resultRMSYmax=RMSYmax;resultRMSYmin=RMSYmin;}
  void setupMeanAxis(double meanYmax, double meanYmin){resultMeanYmax=meanYmax;resultMeanYmin=meanYmin;}
  void setBinTitle(string name){binTitle=name;}

  void sliceHists(string histname, unsigned int numSlices);
  void setSlicesLogy(bool logy=true){sliceLogy = logy;};
  void setSliceXRange(double min, double max){sliceXRangeMin =min,sliceXRangeMax =max;}
  void setGraphLogyLogx(bool logy=true, bool logx=true){GraphLogy = logy;GraphLogx=logx;};

  void clearHists(){histos.clear();}

  void plotHists(int options); //plot 
  void plotRatios();
    
 private:
  
  vector<string> filedirs;
  vector<HistoSlices> histos;
  string file_denom;
  TH1F* h_denom;

  TString resultFile;
  unsigned int number_of_Slices;

  double infinity; 
  double sliceXRangeMin, sliceXRangeMax;
  double resultMeanYmax, resultMeanYmin;
  double resultRMSYmax, resultRMSYmin;
  string binTitle = "";

  bool sliceLogy,GraphLogy,GraphLogx;

  TCanvas* can; 


};

#endif
