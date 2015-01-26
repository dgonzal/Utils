#ifndef HISTSBASE
#define HISTSBASE

//Root libs
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TH1.h"

//c++ libs
#include <iostream>
#include <vector>
#include <string>


using namespace std;

class HistsBase{
 public:
  HistsBase(string saveName);
  ~HistsBase();

  void addFile(string filedir){filedirs.push_back(filedir);}
  void addFolder(string dir, string contains ="", string ends="");

 private:
  TString resultFile;
  vector<string> filedirs;



};
#endif
