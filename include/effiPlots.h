#pragma once 

#include "HistsBase.h"


#include "TH1F.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"


using namespace std;

struct effiContainer{
  effiContainer(TH1F* num, TH1F* denom){numerator=num;denominator=denom;}
  TH1F* numerator, *denominator;
};


class effiPlots: public HistsBase{
 public:
  effiPlots(string saveName);

  void loadHists(string denominator, string numerator);
  void plotEffi();


 private:
  vector<effiContainer> histos;

};
