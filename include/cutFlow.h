#pragma once

#include <string>
#include <stdio.h>
#include <iomanip> 
#include <math.h> 
#include <fstream>

#include "HistsBase.h"

#include "TH1F.h"
#include "TH1.h"
#include "TFile.h"


class cutFlow : public HistsBase{
 public: 
  cutFlow(std::string TxtName,std::string saveName="");
  ~cutFlow();
  void printToFile(std::string histname);
  void setTitle(std::string title_){title=title_;}
  void normToCut(unsigned int cuti){normcut=cuti;}
  void setPrecision(unsigned int decimals){decimals_places =decimals;}
  void set_separator(string sep = ";"){separator = sep;}
 private:
  std::string title;
  unsigned int normcut, decimals_places;
  std::ofstream outFile;
  string separator;
  string filename;
};
