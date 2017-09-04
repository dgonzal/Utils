#pragma once

#include "HistsBase.h"

//root 
#include "TH1.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TSystem.h" 
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TColor.h"

//c++ libs
#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <stdlib.h> 
#include <glob.h>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

//From -pi<phi<pi to 0<phi<2pi convention 
std::string deltaPhistring(std::string var1, std::string var2);
std::string deltaEtastring(std::string var1, std::string var2);
std::string deltaRstring(std::string var1, std::string var2);

struct chain_folderinfo{
  std::vector<TChain*> error_chains_up;
  std::vector<TChain*> error_chains_down;
  std::string dir_up;
  std::string dir_down;
};


std::vector<std::string> glob(const std::string& pat);

class ChainHists: public HistsBase{
 public: 
  ChainHists(string saveName, bool single = false);
  enum error_method{envelop,rms};

  void SetTree(string treeName_){treeName = treeName_;}  
  void SetLegend(double x1=0.6, double y1=0.7, double x2= 0.8, double y2=0.8){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}
  void AddErrorWeight(std::string error_string, error_method method=envelop, std::string replace="");
  void set_ignorePages(int ig=-1, int ig_up=-1){ignore_pages=ig; igup_pages=ig_up;}
  void load_chain_once(bool val=false){if(chainload_once!=val){chainload_once=val; chainloaded=false;}}
  void AddErrorFolder(std::vector<std::string> folder_name);
  void AddErrorFolderAlias(std::string var , std::string new_name, std::string condition="");
  void switch_logy(bool logy_=false){logy =logy_;}
  //no data points are plotted into the ratio
  void mcratio_only(bool mcratio_=true){mcratio=mcratio_;}
  void switch_ratio(bool rat=false){draw_ratio=rat;}

  bool Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true);
  std::vector<TH1F*> return_hists(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events");

 private:
  TChain* load_chain(std::string fileDir);
  TH1F* make_hist(TChain* mychain, std::string variable, std::string binning, std::string draw_option);
  TH1F* calc_MCstat(THStack* stack);
  TH1F* calc_ratio(TH1F* stack_hist, TH1F* hist);
  void calc_weightErr(unsigned int i_error,error_method method, TH1F* result);
  void calc_fixedErr(const std::vector<double> & uncertainties, const std::vector<bool> & stacked, TH1F* result, THStack* stack);
  void calc_errorfolder(std::vector<unsigned int> is, TH1F* result, TH1F* nominal);
  
  TLegend* leg;
  double legx1,legx2,legy1,legy2;
  double max, normArea;
  std::string treeName="";
  std::vector<TH1F*> histos;
  std::vector<std::vector<TH1F*>> error_histos;
  std::vector<std::string> error_weights;
  std::vector<error_method> methods_forerrors;
  std::vector<std::vector<std::string>> replace_strings;
  std::vector<std::vector<std::string>> error_folders;
  std::vector<std::vector<TH1F*>> error_folder_histos;
  //if something changed for the folders
  std::vector<std::string> alias;
  std::vector<std::string> oldnames;
  std::vector<std::string> conditions;
  int page_number =0, ignore_pages =-1,igup_pages=-1;
  bool chainload_once=false, chainloaded=false;
  bool logy=false, debug=false, draw_ratio=true, mcratio=false;
  //Tpads for pretty print
  TPad *pad1, *pad2;
};
