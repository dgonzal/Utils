#pragma once

//Root libs
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TTree.h"

//c++ libs
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <glob.h>
#include <utility>      // std::pair, std::make_pair

//boost libs
#include <boost/filesystem.hpp>
#include "boost/algorithm/string.hpp"

using namespace std;


std::vector<std::string> matching(const std::string& pat);


struct uncertainty{
  TH1F* up;
  TH1F* down;
  std::string unc_nick;
  std::vector<TTree*> trees_up;
  std::vector<TTree*> trees_down;
};

struct style{
  std::string draw_option;
  int color;
  int marker;
  bool stack;
};


struct sample{
  std::string dir;
  std::string nick;
  style hist_style;
  double rate_unc;
  double scale;
  TH1F* hist;
  bool created=false;
  std::vector<TTree*> trees;
  //weight uncertatinties
  std::vector<uncertainty> uncertainties;
  //folder uncertainties
  std::vector<uncertainty> folder_uncertainties;
};


class HistsBase{
 public:
  HistsBase(string saveName, bool single=false);
  ~HistsBase();

  void addFile(string filedir, string hist_draw_option ="",int color = -1, int marker =-1 ,bool stack = false, string nickname = "", double uncer = 0, double scalefactor=1., pair<string,string> replace= make_pair("",""));
  void removeFile(unsigned int position);
  void clearFiles();
  void addFolder(string dir, string contains ="", string ends="");
  void addText(TLatex * text){texts_to_print.push_back(text);}
  void clearText(){texts_to_print.clear();}
  void printText(){for(auto text : texts_to_print)text->Draw();}
  //void switch_singleplots(bool single= true){ singleplots =single;}
  std::vector<sample> get_samples(){return samples;}

  TCanvas* get_can(){return can;}
  //TLegend* get_leg(){return leg;}
  bool change_resultFile(std::string newfile);

  
  std::vector<std::string> get_filedirs(){return filedirs;}
  std::vector<std::string> get_draw_options(){return hist_draw_options;}
  vector<pair<string,string>> get_replace_strings(){return replace_strings;}
  std::vector<bool> get_stackInfo(){return hist_stack;}
  std::vector<int> get_histColors(){return hist_colors;}
  std::vector<int> get_histMarker(){return hist_marker;}
  std::vector<std::string> get_nicknames(){return nicknames;}
  std::vector<double> get_uncertainties(){return uncertainties;}
  std::vector<double> get_scalefactors(){return scale;}
  TString get_resultFile(){return resultFile;}
  bool get_single(){return singleplots;}

  void clear_filedirs(){filedirs.clear();}

 private:
  TString resultFile;
  std::vector<sample> samples;
  std::vector<std::string> filedirs;
  vector<pair<string,string>> replace_strings;
  std::vector<std::string> hist_draw_options;
  std::vector<int> hist_colors;
  std::vector<bool> hist_stack;
  std::vector<int> hist_marker;
  std::vector<std::string> nicknames;
  std::vector<double> uncertainties, scale;
  std::vector<TLatex*> texts_to_print;
  bool singleplots=false, debug = false;
  TCanvas* can;
  //TLegend* leg;

};

