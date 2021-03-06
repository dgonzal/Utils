#include "cutFlow.h"

using namespace std;

cutFlow::cutFlow(string TxtName,string saveName): HistsBase(saveName){
  outFile.open(TxtName);
  title = "";
  normcut =0;
  decimals_places=2;
  separator = "";
  endl_sep = "";
  filename = TxtName;
}
cutFlow::~cutFlow(){
  cout<<" writing cutflow txt file: "<<filename<<endl;
  outFile.close();
}

void cutFlow::printToFile(string histname){
  vector<TH1F*> cutflow_h;
  vector<string> cuts;
  vector<string> filenames;
  unsigned int longest_cutname = 0;
  unsigned int longest_filename = 0;
  if(!title.empty()){
    outFile<<"=============================="<<endl;
    outFile<<title<<endl;
    outFile<<"=============================="<<endl;
  }
  else{
    outFile<<"=============================="<<endl;
    outFile<<histname<<endl;
    outFile<<"=============================="<<endl;
  }
  string error_name = " stat. error";
  string percent = " [%]";
  vector<vector<double>> numbers;
  vector<vector<double>> errors;
  for(const auto & fileDir : get_filedirs()){
    TFile* file = new TFile(fileDir.c_str());
    if(file->GetListOfKeys()->Contains(histname.c_str())!=0){
      cout<<histname<<" does not exist skipping it"<<endl;
      continue;
    } 
    TH1F* hist = (TH1F*) file->Get(histname.c_str());
    cutflow_h.push_back(hist);
    string name_help = "NONAME";
    if(fileDir.find("MC") != std::string::npos)
      name_help = fileDir.substr(fileDir.find(".MC.")+4,fileDir.find(".root")-fileDir.find(".MC.")-4);
    else if(fileDir.find("DATA") != std::string::npos)
      name_help ="DATA";
    filenames.push_back(name_help);
    if(strlen(name_help.c_str())>longest_filename)longest_filename = strlen(name_help.c_str());
  }
  if(strlen(error_name.c_str()) > longest_filename) longest_filename = strlen(error_name.c_str());
  if(strlen(percent.c_str()) > longest_filename) longest_filename = strlen(percent.c_str());

  for(unsigned int i=0; i<cutflow_h.size(); ++i){
    TH1F* hist  = cutflow_h.at(i);
    vector<double> hist_num;
    vector<double> hist_err;
    for(int m=1; m<hist->GetNbinsX()+1; ++m){
      if(i==0){
	cuts.push_back(hist->GetXaxis()->GetBinLabel(m));
	if(strlen(hist->GetXaxis()->GetBinLabel(m)) > longest_cutname) longest_cutname = strlen(hist->GetXaxis()->GetBinLabel(m)); 
      }
      hist_num.push_back(hist->GetBinContent(m));
      hist_err.push_back(hist->GetBinError(m));
    }
    numbers.push_back(hist_num);
    errors.push_back(hist_err);
  }
  longest_cutname++;
  longest_filename++;
  outFile<<separator;
  for(unsigned int it =0; it<longest_filename+1; ++it)
    outFile<<" ";
  for(unsigned int p=0; p<cuts.size(); ++p){
    outFile<<cuts[p]<<separator;
    for(unsigned int ip=0; ip<longest_cutname-strlen(cuts[p].c_str()); ++ip) outFile<<" ";
  }
 
  outFile<<endl_sep<<endl;
  unsigned int i=0;
  for(const auto & fileDir : get_filedirs()){
    if(fileDir.find(".MC.") != std::string::npos){
      outFile<<fileDir.substr(fileDir.find(".MC.")+4,fileDir.find(".root")-fileDir.find(".MC.")-4)<<separator;
      for(unsigned int it = 0; it<longest_filename-strlen(fileDir.substr(fileDir.find(".MC.")+4,fileDir.find(".root")-fileDir.find(".MC.")-4).c_str());++it)
	outFile<<" ";
    }
    else{
      outFile<<"Data"<<separator;
      for(unsigned int it = 0; it<longest_filename-4;++it)
	outFile<<" ";
    }
    for(unsigned int m =0; m<numbers.at(i).size(); ++m){
      string what_is_printed = to_string(numbers.at(i).at(m));
      outFile<<" "<<what_is_printed.substr(0,what_is_printed.find("."))<<separator;
      for(unsigned int mp=0 ;mp<longest_cutname-strlen(what_is_printed.substr(0,what_is_printed.find(".")).c_str())-1 ;++mp)
	outFile<<" ";
    }    
    outFile<<endl_sep<<endl;
    outFile<<error_name<<separator;
    for(unsigned int itt =strlen(error_name.c_str()); itt<longest_filename; ++itt)
      outFile<<" ";
    for(unsigned int m =0; m<errors.at(i).size(); ++m){
      string what_is_printed = to_string(errors.at(i).at(m));
      outFile<<" "<<what_is_printed.substr(0,what_is_printed.find("."))<<separator;
      for(unsigned int mp=0 ;mp<longest_cutname-strlen(what_is_printed.substr(0,what_is_printed.find(".")).c_str())-1 ;++mp)
	outFile<<" ";
    }
    outFile<<endl_sep<<endl;
    outFile<<percent<<separator;
    for(unsigned int itt =strlen(percent.c_str()); itt<longest_filename; ++itt)
	outFile<<" ";
    for(unsigned int m =0; m<numbers.at(i).size(); ++m){
      string what_is_printed = numbers.at(i).at(normcut) > 0 ? to_string(numbers.at(i).at(m)/numbers.at(i).at(normcut)*100) : "-";
      outFile<<" "<<what_is_printed.substr(0,what_is_printed.find(".")+decimals_places)<<separator;
      for(unsigned int mp=0 ;mp<longest_cutname-strlen(what_is_printed.substr(0,what_is_printed.find(".")+decimals_places).c_str())-1;++mp)
	outFile<<" ";
    }
    outFile<<endl_sep<<endl;
    ++i;
  }
  outFile<<endl_sep<<endl;
}

