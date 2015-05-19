#pragma once

#include <string>
#include <stdio.h>
#include <iomanip> 
#include <math.h> 

#include "HistsBase.h"

#include "TH1F.h"
#include "TH1.h"
#include "TFile.h"


class cutFlow : public HistsBase{
 public: 
  cutFlow(std::string saveName);
  ~cutFlow();
  void printToFile(std::string histname);
};
