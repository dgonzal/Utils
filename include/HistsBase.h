#pragma once

//Root libs
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"

//c++ libs
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

//boost libs
#include <boost/filesystem.hpp>

using namespace std;

class HistsBase{
 public:
  HistsBase(string saveName);
  ~HistsBase();

  void addFile(string filedir, string hist_draw_option ="",int color = -1, int marker =-1 ,bool stack = false, string nickname = "", double uncer = 0);
  void addFolder(string dir, string contains ="", string ends="");

  TCanvas* get_can(){return can;}
  //TLegend* get_leg(){return leg;}
  
  std::vector<std::string> get_filedirs(){return filedirs;}
  std::vector<std::string> get_draw_options(){return hist_draw_options;}
  std::vector<bool> get_stackInfo(){return hist_stack;}
  std::vector<int> get_histColors(){return hist_colors;}
  std::vector<int> get_histMarker(){return hist_marker;}
  std::vector<std::string> get_nicknames(){return nicknames;}
  std::vector<double> get_uncertainties(){return uncertainties;}
  TString get_resultFile(){return resultFile;}
  
  void clear_filedirs(){filedirs.clear();}

 private:
  TString resultFile;
  std::vector<std::string> filedirs;
  std::vector<std::string> hist_draw_options;
  std::vector<int> hist_colors;
  std::vector<bool> hist_stack;
  std::vector<int> hist_marker;
  std::vector<std::string> nicknames;
  std::vector<double> uncertainties;
  TCanvas* can;
  //TLegend* leg;

};

