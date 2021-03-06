#pragma once

#include "HistsBase.h"

//Root libs
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TF1.h"
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
#include "TList.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TGaxis.h"

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
  simplePlots(string saveName, bool single=false);
  void setLegendTitle(string leg_title_e){leg_title = leg_title_e;}
  void addLegendEntry(string legendEntry){legend.push_back(legendEntry);}
  void setLegend(double x1, double y1, double x2, double y2){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}

  void loadHists(string histname, string title = "",string plotting_style = "",bool ratio=true);
  void loadHists(TH1F * hist, string legend_entry="",string plotting_style = "",bool ratio=true);
  void loadStackHists(string histname,int color);
  void loadStackHists(TH1F * hist, string legend_entry="");
  void loadTH2(string histname);
  void loadTF1(TF1* func, string legend_entry="", string plotting_style="");
  void loadTGraph(TGraph* graph, string legend_entry="", string plotting_style="");

  void plotHists(int options=0, bool logy=false); 
  void plotTH2(int options=0); 
  void normToArea(bool area=true, double max_ =-1){normArea=area;max=max_;}
  void setPerGeV(bool switchPerGeV=true){perGeV=switchPerGeV;}
  void setErrorBand(double min_, double max_){errorband_max = max_; errorband_min =min_;}//used for the ratio plot
  void change_colors(bool col){changecolors = col;}
  void switch_ratio(bool rat_){draw_ratio=rat_;}
  void set_ratioYTitle(string ratio_ytitle_){ratio_ytitle = ratio_ytitle_;}
  void set_histYTitle(string hist_ytitle_){hist_ytitle= hist_ytitle_;}
  void set_ratioLimtis(double ratio_min_, double ratio_max_){ratio_min = ratio_min_; ratio_max=ratio_max_;}
  void set_XTitle(string xtitle_){xtitle= xtitle_;}
  void set_title(string hist_title_){hist_title=hist_title_;}
  void set_ratiodrawoption(string nom_opt="e2", string var_opt="same E0"){ratio_drawopt_nom=nom_opt;ratio_drawopt_next=var_opt;}
  void set_zerobinsratio(bool zeroratiobin_){zeroratiobin=zeroratiobin_;}
  void set_histminimum(double min=0.00001){hist_minimum=min;}
  void set_ratiofit(bool fit=false){fit_ratio=fit;}

  
  void clearHists(){histos.clear();}
  void clearLegend(){legend.clear();stack_legend.clear();func_leg.clear();graph_leg.clear();}
  void clearFiles(){clear_filedirs();}
  void clearStack(){stack =  new THStack("hs","stacked histograms");}
  void clearGraphs(){graphs.clear();}
  void clearFunc(){functions.clear();}	

  void clearAll(){clearHists();clearLegend();clearFiles();clearStack();clearGraphs();clearFunc();}

 private:
  TH1F* normtoGeV(TH1F* hist);
  TH1F* ratio(TH1F* num, TH1F* denom, bool norm, bool zeroBinAsOne=false);
  vector<TGraph*> graphs;
  vector<TF1*> functions;
  vector<TH2F*> twoDhists;
  vector<TH1F*> histos;
  vector<string> legend, stack_legend, func_leg, graph_leg;
  std::vector<std::string> plotting_styles, func_plotting,graph_plotting;
  vector<bool> plotInratio;
  string ratio_ytitle = "Data/MC", hist_ytitle = "events", xtitle="", hist_title="";
  string leg_title = "";
  string ratio_drawopt_nom = "e2", ratio_drawopt_next="same E0";
  double ratio_min = 0.1, ratio_max = 2.9;
  double errorband_max = 2, errorband_min = 0;
  TLegend* leg;
  double legx1,legx2,legy1,legy2;
  bool normArea,changecolors=true,zeroratiobin =false;
  double max;
  THStack* stack;
  bool using_stack=false;
  bool perGeV=false;
  bool draw_ratio =true;
  bool fit_ratio = false;
  int page_number =0;
  double hist_minimum = 0.00001;
};
