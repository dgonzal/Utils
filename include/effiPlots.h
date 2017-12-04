#pragma once 

#include "HistsBase.h"

#include <iostream>
#include <string>
#include <boost/math/special_functions/round.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TEfficiency.h"
#include "TGaxis.h"
#include "TList.h"


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
  effiPlots(string saveName, bool single = false);
  void loadHists(std::string denominator, std::string numerator="", std::string leg_entry_graph="");
  void loadHists(TH1F* denominator, TH1F* numerator, std::string leg_entry_graph=""){histos.push_back(effiContainer(numerator,denominator));legend_entries.push_back(leg_entry_graph);};
  /*/ Two possible options implemented
   *  option 0: Denominator is integral bin i -> NBins eg trigger efficiency
   *  option 1: Denominator is integral bin 1 -> NBins eg cut efficiency
  /*/
  void plotEffi(int options=0, vector<pair<int,int>> scalefactor={});
  
  void set_axisTitle(std::string x_, std::string y_="Efficiency"){x_axis=x_;y_axis=y_;}
  
  void setTitle(std::string my_title){title.push_back(my_title);}   
  void imposeHist(bool impose_switch =true);
  void setRange(double min, double max, double ymin, double ymax){rangeMin =min; rangeMax = max; yRangeMin=ymin; yRangeMax=ymax;}
  void loadTHHists(std::string denominator, std::string numerator="");
  void plotTH(); 
  void switch_legend(bool myswitch =false){legend_bool=myswitch;}
  void set_debug(bool debug_ =true){debug=debug_;}
  void clear_histos(){histos.clear();histsTH.clear();legend_entries.clear();}//std::vector<effiContainer>().swap(histos); std::vector<TH1EffiCont>().swap(histsTH); std::vector<std::string>().swap(legend_entries);}
  void set_Rebin(int reb){rebin_int = reb;}
  void set_Rebin(std::vector<double> reb){rebin_bins = reb;}
  void set_val(string val_="val"){val=val_;}
  string get_nominal(){return nominal;}
  string get_up(){return up_var;}
  string get_down(){return down_var;}
  
 private:
  TString create_resultfilename(int & number, bool summary=false); 
  void drawDistri(TH1F* hist,double max);
  TGraphAsymmErrors* compute_scalefactorerrors(TH1F* num_h, TH1F* den_h,TGraphAsymmErrors* scaledenom, TGraphAsymmErrors* scalenum);
  TLegend* leg;
  bool legend_bool;
  std::vector<std::string> legend_entries;
  std::vector<std::string> title;
  std::vector<effiContainer> histos;
  std::vector<TH1EffiCont> histsTH;
  std::string x_axis ="",y_axis="Efficiency"; 
  bool imposeDistri;
  double rangeMax=0, rangeMin=0, yRangeMax=0, yRangeMin=0;
  bool debug = false;
  int rebin_int=-1, page_number=1, summary_number=1;
  string nominal="",up_var="",down_var="",val="val";
  std::vector<double> rebin_bins={};
};

