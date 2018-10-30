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
#include <experimental/filesystem>


#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/regex.hpp"
#include <boost/filesystem.hpp>

namespace fs = std::experimental::filesystem;
using namespace std;


int fill_histograms(string file_dir_, string file_prefix="", string output_prefix=""){
  string file_dir = file_dir_;

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
  
  for(auto uncer : uncer_names){
    string _filePath = "plots/"+output_prefix+file_prefix+uncer+"_plots/";
    const char* path = _filePath.c_str();
    boost::filesystem::path dir(path);
    if(boost::filesystem::create_directory(dir)){
	std::cerr<< "Directory Created: "<<_filePath<<std::endl;
    }
    simplePlots uncerPlots("plots/"+output_prefix+file_prefix+uncer+"_plots/", true);
    uncerPlots.addLegendEntry("nominal");
    uncerPlots.addLegendEntry("up");	
    uncerPlots.addLegendEntry("down");
    uncerPlots.set_histYTitle("Events");
    //uncerPlots.setPerGeV(true);
    uncerPlots.set_XTitle("m_{reco} [GeV]");
    uncerPlots.set_title(" ");
    uncerPlots.set_ratiodrawoption("e2","Hist same");
    uncerPlots.set_zerobinsratio(true);
    uncerPlots.set_ratioYTitle("Unc. ratio");			    
    uncerPlots.set_ratioLimtis(0.5,1.5);
    //simplePlots uncerStackPlots("plots/"+uncer+"_stackplots.ps");
    uncerPlots.addFile(file_dir);
    next = TIter(file->GetListOfKeys());
    while ((key = (TKey*)next())) {
      TString histnameCheck(key->GetName());
      if(histnameCheck.Contains("DATA") || histnameCheck.Contains("Data") )
	continue;
      if(histnameCheck.Contains("_1100_") || histnameCheck.Contains("_1400_") || histnameCheck.Contains("_Background_")){}
      else
        continue;
      
      if(histnameCheck.Contains(uncer+"__plus")){
	histnameCheck.ReplaceAll("__"+uncer+"__plus","");
	string name (histnameCheck.Data());
	string title = name;
	boost::replace_all(title,"Chi2","");
	boost::replace_all(title,"Background","");
	boost::replace_all(title,"_"," ");
	boost::replace_all(title,"BT","b t");
	boost::replace_all(title,"WT","W t");
	boost::replace_all(title,"TopT","t t");
	boost::replace_all(title,"Ele"," e channel");
	boost::replace_all(title,"Mu"," #mu channel");
		
	//cout<<histnameCheck<<endl;
	uncerPlots.setLegendTitle(title);
	uncerPlots.loadHists(name);
	uncerPlots.loadHists(name+"__"+uncer+"__plus");
	uncerPlots.loadHists(name+"__"+uncer+"__minus");
		
	uncerPlots.plotHists(2,false);
	uncerPlots.clearHists();
	//uncerPlots.clearLegend();
      }
    }
  }

  //cout<<"I'm done with the uncertainties"<<endl;
  return 0;
}



int main(){
  std::string path = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/Utils/limits/";
  fill_histograms(path+"ROOTDataShape_50_0_3500_eta4/Eta4_BprimeT_RH_rebinned_sr_rescale.root","","BprimeT_RH_");
  fill_histograms(path+"ROOTDataShape_50_0_3500_eta4/Eta4_BprimeB_RH_rebinned_sr_rescale.root","","BprimeB_RH_");

  /*/
  fill_histograms(path+"ROOT_50_0_3500/background_postfit.root","","background");
  fill_histograms(path+"ROOTDataShape_50_0_3500/Bay_BprimeB_RH_rebinned.root","","BprimeB_RH_");
  fill_histograms(path+"ROOTDataShape_50_0_3500/Bay_BprimeT_RH_rebinned.root","","BprimeT_RH_");
  /*/
  
  return 0;

  for (auto & p : fs::directory_iterator(path)){
    string working_path = p.path();//.string().c_str();
    if(boost::contains(working_path,"Mu") || boost::contains(working_path,"Ele") || !boost::contains(working_path,"rebinned") )
      continue;
    else{     
      vector<string> splitted_file;
      boost::split_regex(splitted_file,working_path,boost::regex("/"));
      string prefix = splitted_file[splitted_file.size()-1];
      boost::replace_all(prefix, ".root", "");
      cout<<working_path<<" "<<prefix <<endl;
      fill_histograms(working_path, prefix+"_");
    }
  }
  cout<<"I'm done with the uncertainties"<<endl;
  return 0;
}
