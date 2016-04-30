#pragma once 

#include "HistsBase.h"


#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TEfficiency.h"
#include "TGaxis.h"

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
   * option 0: Denominator is integral bin i -> NBins eg trigger efficiency
   * option 1: Denominator is integral bin 1 -> NBins eg cut efficiency
   *
   */
  void plotEffi(int options=0); 
  void setTitle(string my_title){title.push_back(my_title);}   
  void imposeHist(bool impose_switch =true);
  void setRange(double min, double max){rangeMin =min; rangeMax = max;}
  void loadTHHists(string denominator, string numerator="");
  void plotTH(); 
  void switch_legend(bool myswitch =false){legend_bool=myswitch;}
  void set_debug(bool debug_ =true){debug=debug_;}
  void clear_histos(){vector<effiContainer>().swap(histos); vector<TH1EffiCont>().swap(histsTH);}
  void set_Rebin(int reb){rebin_int = reb;}

 private:
  void drawDistri(TH1F* hist,double max);
  TLegend* leg;
  bool legend_bool;
  vector<string> title;
  vector<effiContainer> histos;
  vector<TH1EffiCont> histsTH;
  bool imposeDistri;
  double rangeMax, rangeMin;
  bool debug = false;
  int rebin_int=-1;
};
