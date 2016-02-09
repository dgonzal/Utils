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

  void addFile(string filedir);
  void addFolder(string dir, string contains ="", string ends="");

  TCanvas* get_can(){return can;}
  //TLegend* get_leg(){return leg;}
  
  vector<string> get_filedirs(){return filedirs;}
  TString get_resultFile(){return resultFile;}
  
  void clear_filedirs(){filedirs.clear();}

 private:
  TString resultFile;
  vector<string> filedirs;
  TCanvas* can;
  //TLegend* leg;

};

