#pragma once

#include "HistsBase.h"

//root 
#include "TH1.h"
#include "TTree.h"
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

class TreeHists: public HistsBase{
 public: 
  TreeHists(string saveName);
  enum error_method{envelop,rms};

  void SetTree(string treeName_){treeName = treeName_;}  
  void SetLegend(double x1=0.6, double y1=0.7, double x2= 0.8, double y2=0.8){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}
  void AddErrorWeight(std::string error_string, error_method method=envelop, std::string replace="");
  void AddErrorFolder(std::vector<std::string> folder_name);

  bool Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events");
 

 private:
  TTree* load_tree(std::string fileDir);
  TH1F* make_hist(TTree* mytree, std::string variable, std::string binning, std::string draw_option);
  TH1F* calc_MCstat(THStack* stack);
  TH1F* calc_ratio(THStack* stack, TH1F* hist);
  void calc_weightErr(unsigned int i_error,error_method method, TH1F* result);
  void calc_fixedErr(const std::vector<double> & uncertainties, const std::vector<bool> & stacked, TH1F* result, THStack* stack);

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

  //Tpads for pretty print
  TPad *pad1, *pad2;
};
