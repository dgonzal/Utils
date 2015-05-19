#include "cutFlow.h"

using namespace std;

cutFlow::cutFlow(string saveName): HistsBase(saveName){}
cutFlow::~cutFlow(){}

void cutFlow::printToFile(string histname){
  vector<TH1F*> cutflow_h;
  vector<string> cuts;
  vector<string> filenames;
  unsigned int longest_cutname = 0;
  unsigned int longest_filename = 0;
  vector<vector<double>> numbers;
  for(const auto & fileDir : get_filedirs()){
    TFile* file = new TFile(fileDir.c_str());
    TH1F* hist = (TH1F*) file->Get(histname.c_str());
    cutflow_h.push_back(hist);
    string name_help = fileDir.substr(fileDir.find(".MC.")+4,fileDir.find(".root")-fileDir.find(".MC.")-4);
    filenames.push_back(name_help);
    if(strlen(name_help.c_str())>longest_filename)longest_filename = strlen(name_help.c_str());
  }
  for(unsigned int i=0; i<cutflow_h.size(); ++i){
    TH1F* hist  = cutflow_h.at(i);
    vector<double> hist_num;
    for(int m=1; m<hist->GetNbinsX()+1; ++m){ //GetXaxis()->)
      if(i==0){
	cuts.push_back(hist->GetXaxis()->GetBinLabel(m));
	if(strlen(hist->GetXaxis()->GetBinLabel(m)) > longest_cutname) longest_cutname = strlen(hist->GetXaxis()->GetBinLabel(m)); 
      }
      hist_num.push_back(hist->GetBinContent(m));
    }
    numbers.push_back(hist_num);
  }
  longest_cutname++;
  longest_filename++;
  for(unsigned int it =0; it<longest_filename+1; ++it)
    cout<<" ";
  for(unsigned int p=0; p<cuts.size(); ++p){
    cout<<cuts[p];
    for(unsigned int ip=0; ip<longest_cutname-strlen(cuts[p].c_str()); ++ip) cout<<" ";
  }
  cout<<endl;
  unsigned int i=0;
  for(const auto & fileDir : get_filedirs()){
    cout<<fileDir.substr(fileDir.find(".MC.")+4,fileDir.find(".root")-fileDir.find(".MC.")-4);
    for(unsigned int it = 0; it<longest_filename-strlen(fileDir.substr(fileDir.find(".MC.")+4,fileDir.find(".root")-fileDir.find(".MC.")-4).c_str());++it)
      cout<<" ";
    for(unsigned int m =0; m<numbers.at(i).size(); ++m){
      string what_is_printed = to_string(trunc(numbers.at(i).at(m)));
      cout<<" "<<what_is_printed.substr(0,what_is_printed.find("."));
      for(unsigned int mp=0 ;mp<longest_cutname-strlen(what_is_printed.substr(0,what_is_printed.find(".")).c_str())-1 ;++mp)
	cout<<" ";
    }
    cout<<endl;
      for(unsigned int itt =0; itt<longest_filename; ++itt)
	cout<<" ";
    for(unsigned int m =0; m<numbers.at(i).size(); ++m){
      string what_is_printed = to_string(numbers.at(i).at(m)/numbers.at(i).at(2)*100);
      cout<<" "<<what_is_printed.substr(0,what_is_printed.find(".")+2);
      for(unsigned int mp=0 ;mp<longest_cutname-strlen(what_is_printed.substr(0,what_is_printed.find(".")+2).c_str())-1;++mp)
	cout<<" ";
    }
    cout<<endl;
    
    
    ++i;
  }
}
