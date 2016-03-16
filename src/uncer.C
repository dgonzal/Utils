//main routine call everything else from here.
//so i do not have to change my makefile and everything compiles

#include "slicePlots.h"
#include "simplePlots.h"
#include "effiPlots.h"
#include "cutFlow.h"

#include "TFile.h"
#include "TKey.h"
#include "TString.h"

#include <iostream>
#include <string>

#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/regex.hpp"

using namespace std;

int main(){
  string file_dir = "/nfs/dust/cms/user/gonvaq/fast_limits/BpReco_RH_rebinned.root";

  TFile* file = new TFile(file_dir.c_str(),"READ");
  TIter next(file->GetListOfKeys());
  TKey *key;

  vector<string> uncer_names;
  while ((key = (TKey*)next())) {
    string histname = key->GetName();
    if(boost::contains(histname,"DATA"))
      continue;
    if(boost::contains(histname,"__plus")){
      vector<string> splitted_histname;
      boost::split_regex(splitted_histname,histname, boost::regex("__"));
      bool saved = false;
      for(auto name : uncer_names)
	if(name == splitted_histname[2]) saved =true;
      if(!saved)
	uncer_names.push_back(splitted_histname[2]);
    }
  }
  
  for(auto uncer :uncer_names){ 
    simplePlots uncerPlots("plots/"+uncer+"_plots.ps");
    uncerPlots.addFile(file_dir);
    next = TIter(file->GetListOfKeys());
    while ((key = (TKey*)next())) {
      TString histnameCheck(key->GetName());
      if(histnameCheck.Contains("DATA") || histnameCheck.Contains("Data") )
	continue;
      if(histnameCheck.Contains(uncer+"__plus")){
	histnameCheck.ReplaceAll("__"+uncer+"__plus","");
	string name (histnameCheck.Data());
	//cout<<histnameCheck<<endl;
	uncerPlots.loadHists(name,name);
	uncerPlots.loadHists(name+"__"+uncer+"__plus");
	uncerPlots.loadHists(name+"__"+uncer+"__minus");
	uncerPlots.plotHists(2,false);
	uncerPlots.clearHists();
	uncerPlots.clearLegend();
      }
    }
  }

  cout<<"I'm done with the uncertainties"<<endl;
  return 0;
}
