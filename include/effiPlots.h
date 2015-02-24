#pragma once 

#include "HistsBase.h"


#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TEfficiency.h"

using namespace std;


struct TH1EffiCont{
  TH1EffiCont(TH1* num_, TH1* denom_){num=num_;denom=denom_;}
  TH1* num, *denom;
};


struct effiContainer{
  effiContainer(TH1F* num, TH1F* denom){numerator=num;denominator=denom;}
  TH1F* numerator, *denominator;
};


class effiPlots: public HistsBase{
 public:
  effiPlots(string saveName);

  void loadHists(string denominator, string numerator="");


  /*Two possible options implemented
   * option 0: Denominator is integral bin i -> NBins  zB. trigger efficiency
   * option 1: Denominator is integral bin 1 -> NBins zB. cut efficiency
   *
   */

  void setTitle(string my_title){title.push_back(my_title);}   

  void plotEffi(int options=0); 
  void setRange(double min, double max){rangeMin =min; rangeMax = max;}

  void loadTHHists(string denominator, string numerator="");
  void plotTH(); 
  
 private:
  TLegend* leg;
  vector<string> title;
  vector<effiContainer> histos;
  vector<TH1EffiCont> histsTH;

  double rangeMax, rangeMin;

};
