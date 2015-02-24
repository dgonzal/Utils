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

//c++ libs
#include <iostream>
#include <vector>
#include <sstream> 
#include <iomanip>
#include <limits>
#include <string>

using namespace std;



class simplePlots: public HistsBase{
 public:
  simplePlots(string saveName);
 
  void addLegendEntry(string legendEntry){legend.push_back(legendEntry);}
  void clearLegend(){legend.clear();}
  void setLegend(double x1, double y1, double x2, double y2){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}

  void loadHists(string histname);
  void clearHists(){histos.clear();}

  void plotHists(int options, bool logy=false); 

  void loadTH2(string histname);
  void plotTH2(int options=0); 

    
 private:
  
  vector<TH2F*> twoDhists;
  vector<TH1F*> histos;
  vector<string> legend;

  TLegend* leg;
  double legx1,legx2,legy1,legy2;
  
};
