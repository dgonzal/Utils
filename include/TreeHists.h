#pragma once

#include "HistsBase.h"

//root 
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TSystem.h" 
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TColor.h"

//c++ libs
#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <stdlib.h> 


//From -pi<phi<pi to 0<phi<2pi convention 
std::string deltaPhistring(std::string var1, std::string var2);
std::string deltaEtastring(std::string var1, std::string var2);
std::string deltaRstring(std::string var1, std::string var2);


struct error_folder_rep{
  std::vector<TTree*> error_trees;
  std::vector<std::string> replace_variables;
  std::string folder;
  std::string nick;
  std::vector<TH1F*> histos;
};

struct tree_folderinfo{
  std::vector<TTree*> error_trees_up;
  std::vector<TH1F*> histos_up;
  std::vector<TTree*> error_trees_down;
  std::vector<TH1F*> histos_down;
  std::string dir_up;
  std::string dir_down;
  std::string nick_up;
  std::string nick_down;
  std::vector<std::string> samples;
};


class TreeHists: public HistsBase{
 public: 
  TreeHists(string saveName, bool single = false);
  enum error_method{envelop,rms};

  void SetTree(string treeName_){treeName = treeName_;}  
  void SetLegend(double x1=0.6, double y1=0.7, double x2= 0.8, double y2=0.8){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}
  void AddErrorWeight(std::string error_string, error_method method=envelop, std::string replace="");
  void AddErrorWeight(std::string error_up, std::string error_down, error_method method=envelop);
  void set_ignorePages(int ig=-1, int ig_up=-1){ignore_pages=ig; igup_pages=ig_up;}
  void load_tree_once(bool val=false){if(treeload_once!=val){treeload_once=val; treeloaded=false;}}
  void AddErrorFolder(std::vector<std::string> folder_name);
  void AddErrorFolderRep(std::string folder_name, std::string variable, std::string replace, std::string nick="");
  void AddErrorFolderAlias(std::string var, std::string new_name, std::string condition="");
  void switch_logy(bool logy_=false){logy =logy_;}
  //no data points are plotted into the ratio
  void set_ratiolimits(double up, double down){ratio_up=up; ratio_down=down;}
  void mcratio_only(bool mcratio_=true){mcratio=mcratio_;}
  void switch_ratio(bool rat=false){draw_ratio=rat;}
  void fit_ratio(bool fitrat_=true, string func_string=""){fitrat=fitrat_;ratio_func=new TF1("myfitratiofunc",func_string.c_str());}
  TF1* get_ratiofunction(){return ratio_func;}
 
  bool Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true, std::string data_draw_option="");
  std::vector<TH1F*> return_hists(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events");
  //optionen: 0 no pull, 1 pull for stats, 2 pull for total error, 3 pull for 1 and 2
  // not yet implimented
  void set_pull(int pull_option_ =0){pull_option = pull_option_;}
  void set_single_errors(bool single_error_ratio_){single_error_ratio= single_error_ratio_;}
  
 private:
  TTree* load_tree(std::string fileDir);
  TH1F* make_hist(TTree* mytree, std::string variable, std::string binning, std::string draw_option);
  void ratio_makeup(TH1F* hist, std::string x_axis);
  TH1F* calc_MCstat(THStack* stack);
  TH1F* calc_ratio(TH1F* stack_hist, TH1F* hist);
  TH1F* calc_pull(TH1F* data, TH1F* pred);
  void calc_weightErr(unsigned int i_error,error_method method, TH1F* result);
  void calc_fixedErr(const std::vector<double> & uncertainties, const std::vector<bool> & stacked, TH1F* result, THStack* stack);
  void calc_errorfolder(tree_folderinfo & folder, TH1F* result, TH1F* nominal);
  void calc_errorfolder(error_folder_rep & folder, TH1F* result, TH1F* nominal);

  void calc_single_errors(tree_folderinfo folder);
 
  vector<TH1F*> weight_err_sum;
 
  TF1* ratio_func;
  bool fitrat=false;
  TLegend* leg;
  double legx1,legx2,legy1,legy2;
  double ratio_up=1.99, ratio_down =0.;
  int pull_option;
  double max, normArea;
  std::string treeName="";
  std::vector<TH1F*> histos;
  std::vector<std::vector<TH1F*>> error_histos;
  std::vector<std::string> error_weights;
  std::vector<error_method> methods_forerrors;
  std::vector<std::vector<std::string>> replace_strings;
  std::vector<std::vector<std::string>> error_folders;
  std::vector<error_folder_rep> error_folder_replace;
  //std::vector<std::vector<TH1F*>> error_folder_histos;
  //if something changed for the folders
  std::vector<std::string> alias;
  std::vector<std::string> oldnames;
  std::vector<std::string> conditions;
  int page_number =0, ignore_pages =-1,igup_pages=-1;
  bool treeload_once=false, treeloaded=false;
  bool logy=false, debug=false, draw_ratio=true, mcratio=false;
  //Tpads for pretty print
  TPad *pad1, *pad2;
  bool single_error_ratio=false;
};
