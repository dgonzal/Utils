#pragma once

#include "HistsBase.h"

//Root libs
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include <TSystem.h> 
#include "THStack.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TColor.h"

//c++ libs
#include <iostream>
#include <vector>
#include <sstream> 
#include <iomanip>
#include <limits>
#include <string>
#include "assert.h"

//boost libs
#include <boost/algorithm/string.hpp>


using namespace std;



class simplePlots: public HistsBase{
 public:
  simplePlots(string saveName);
  void addLegendEntry(string legendEntry){legend.push_back(legendEntry);}
  void setLegend(double x1, double y1, double x2, double y2){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}

  void loadHists(string histname, string title = "");
  void loadHists(TH1F * hist);
  void loadStackHists(string histname,int color);
  void loadStackHists(TH1F * hist);
  void loadTH2(string histname);

  void plotHists(int options=0, bool logy=false); 
  void plotTH2(int options=0); 
  void normToArea(bool area=true, double max_ =-1){normArea=area;max=max_;}
  void setErrorBand(double min_, double max_){errorband_max = max_; errorband_min =min_;}//used for the ratio plot

  void clearHists(){histos.clear();}
  void clearLegend(){legend.clear();}
  void clearFiles(){clear_filedirs();}
  
 private:
  TH1F* ratio(TH1F* num, TH1F* denom, bool norm);
  vector<TH2F*> twoDhists;
  vector<TH1F*> histos;
  vector<string> legend;
  double errorband_max = 2, errorband_min = 0;
  TLegend* leg;
  double legx1,legx2,legy1,legy2;
  bool normArea;
  double max;
  THStack* stack;
};
