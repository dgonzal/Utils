#pragma once

#include "HistsBase.h"

#include "TSystem.h" 
#include "TFile.h"

#include <vector>
#include <string>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>


struct uncer_info{
  std::string method;
  std::string weight_up;
  std::string weight_down;
  std::string nick;
};

struct folder_alias{
  std::string old_v;
  std::string new_v;
  std::string condition;
};

class TreeRootHist: public  HistsBase{
 public:
  TreeRootHist(string saveName, bool single = false);
  
  //bool Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true, std::string data_draw_option="", std::string plot_name="");
  bool Draw_ROOT(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true, std::string data_draw_option="", std::string plot_name="");
  void addUnc(std::string error_up, std::string error_down, std::string method="", std::string name = "");
  void addFolderAlias(std::string old_v, std::string new_v, std::string condition);
  
  void SetTree(std::string treeName_){treeName = treeName_;}  
  std::vector<TH1F*> return_hists(string variable, string draw_option, string binning, std::string x_axis, std::string y_axis="events");
  void SetLegend(double x1=0.6, double y1=0.7, double x2= 0.8, double y2=0.8){ legx1=x1;legx2=x2;legy1=y1;legy2=y2;}

  
 private:
  TH1F* make_hist(TTree* mytree, std::string variable, std::string binning, std::string draw_option, std::string plot_name="");
  TH1F* folder_hist(std::vector<TTree*> trees, std::string var, std::string binning, std::string mod, std::string plots_name);

  void envelopmodule(sample &proc, uncer_info info, std::string variable, std::string binning, std::string modified_draw_option, std::string plot_name="", std::string x_axis="");
  void rmsmodule(sample &proc, uncer_info info, std::string variable, std::string binning, std::string modified_draw_option, std::string plot_name="", std::string x_axis="");

  TTree* load_tree(std::string fileDir);
  bool load_trees_for_samples();
  bool fill_histograms(std::string variable, std::string draw_option, std::string binning, std::string x_axis, std::string y_axis, bool legend, std::string data_draw_option, std::string plot_name);
  bool save_histograms_to_root(std::string file_name);
  bool histo_cleanup();
  std::vector<std::string> autocomplete_filedir(std::string filedir);
  bool load_folder_trees(sample &proc, std::string filedir, uncer_info info);
    
  std::vector<sample> work_samples;
  std::vector<uncer_info> unc_info;
  std::vector<folder_alias> alias;
  
  TLegend* leg;
  double legx1,legx2,legy1,legy2;
  double ratio_up=1.99, ratio_down =0.;
  double max, normArea;
  TPad *pad1, *pad2;
  bool single_error_ratio=false, single_ratios=false;
  std::string ratio_title = "Data/MC", treeName = "AnalysisTree";
  bool debug = false;
  bool loaded_trees = false;
  int page_counter =1;
};




