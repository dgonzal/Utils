#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "TLatex.h"

bool string_bool(std::string input);

//TreeHists::Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true, std::string data_draw_option="");
class Draw{
 public:
  Draw(std::vector<std::string> input, std::string factors="");
  void print();
  std::string command="", weight="", binning="", x_axis="", y_axis="Events", data_option="";
  bool legend, logy=false;
   
};

class Data{
 public:
  Data(std::vector<std::string> input);
  std::string filedir, hist_draw_option ="", nickname = "";
  int color = -1, marker =-1;
  bool stack = false;
  double uncer = 0, scalefactor=1.;
};

class TreeConfig{
 public:
  TreeConfig(std::vector<std::string> configfiles);

  std::string get_name(){return name;}
  bool get_single(){return single;}
  bool get_mcratio(){return mcratioonly;}
  bool get_ratio(){return ratio;}
  bool get_mutrkcorr(){return mutrk;}
  bool get_eletrigcorr(){return eletrigger;}
  bool get_forwardcorr(){return forward_corr;}
  
  std::vector<Data> get_files(){return file_dirs;}
  std::vector<double> get_legendCoordinates();
  std::vector<double> get_ratioLimits(){return {ratio_up,ratio_down};}
  std::vector<TLatex*> get_tlatex(){return fig_text;}
  std::string get_analysisTree(){return analysistree;}
  
  std::vector<std::vector<std::string>> get_errorWeights(){return errors;}
  std::vector<std::vector<std::string>> get_errorFolders(){return error_folders;}
  std::vector<std::vector<std::string>> get_folderalias(){return alias;}
  std::vector<Draw> get_drawCommands(){return draw_commands;}
  

 private:
  TLatex* make_tlatex(std::vector<std::string> tlatex_input);
    
  std::string name="", analysistree = "AnalysisTree", dir_prefix="", factors="";
  bool single = false, mcratioonly=false, ratio=true, forward_corr=false,mutrk=false,eletrigger=false;
  double ratio_up = 1.69, ratio_down = 0.39;
  std::vector<Data> file_dirs;
  std::string file_prefix="";
  std::vector<Draw> draw_commands;
  std::vector<std::vector<std::string>> errors;
  std::vector<std::vector<std::string>> error_folders;
  std::vector<std::vector<std::string>> alias;
  std::vector<TLatex*> fig_text;
};



