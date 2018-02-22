#include <vector>
#include <string>
#include <iostream>
#include <cstdlib> 
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "TH1.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TPad.h"
#include "TApplication.h"
#include "TROOT.h"

#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"

using namespace std;

struct TreeDrawInfo{
  TreeDrawInfo( std::string draw_command_, std::string selection_, std::string hist_name_, std::string binning_,double scale_):draw_command(draw_command_),selection(selection_),hist_name(hist_name_), binning(binning_), scale(scale_){;}
  std::string draw_command;
  std::string selection;
  std::string hist_name;
  std::string binning;
  double scale;
};

class TreeDrawMain{
public:
  TreeDrawMain(std::string binning_, std::string treename_){binning = binning_;treename =treename_;};
  enum error_method{envelop,rms};

  void AddFileDir(std::string file){FileDir.push_back(file);}
  void AddFileDir(std::vector<std::string> dirs){ FileDir = dirs;}
  void AddSamples(std::vector<std::string> sample_){Sample =sample_;}

  void AddHistCategory(std::string draw_command_, std::string selection_, std::string hist_name_, std::string hist_binning_="",double scale=-1.0){infoVec.push_back(TreeDrawInfo(draw_command_,selection_,hist_name_, hist_binning_.empty() ? binning: hist_binning_,scale));}
  void AddWeightError(std::string weight_option, std::string error_name, error_method method=envelop, std::string replace_what="", std::string replace_with="", std::string replace_w_what="", std::string replace_w_with="" ){error_weight.push_back(weight_option);error_methods_container.push_back(method);error_names.push_back(error_name); error_replace_draw.push_back(make_pair(replace_what,replace_with)); error_replace_weight.push_back(make_pair(replace_w_what,replace_w_with));}
  void AddDirError(std::string dir_error, std::string nick){dir_errors.push_back(dir_error); dir_errors_nick.push_back(nick);}

  void inject_signal(std::string signal, std::string signal_weight_){signal_name = signal; signal_weight = signal_weight_;}
  
  bool create_file(std::string fileName);

private:
  TH1F* make_hist(TTree* mytree, std::string variable, std::string draw_option, std::string hist_binning);
  TTree* load_tree(std::string fileDir);
  std::vector<std::string> find_matches(std::string dir,std::string name);
  void find_samples_nicks (std::string dir);
  void set_channel(std::string dir);

  //std::vector<TH1F*> hist;
  //std::vector<TH1F*> error_hist;
  std::vector<std::string> error_weight;
  std::vector<error_method> error_methods_container;
  std::vector<std::pair<std::string,std::string>> error_replace_draw;
  std::vector<std::pair<std::string,std::string>> error_replace_weight;
  std::vector<std::string> error_names;
  std::vector<std::string> dir_errors;
  std::vector<std::string> dir_errors_nick;
  std::vector<TreeDrawInfo> infoVec;
  std::vector<std::string> FileDir;
  std::vector<std::string> Sample;
  std::vector<std::string> working_samples;
  std::vector<std::string> nicks;
  std::string treename;
  std::string binning;
  std::string channel="";
  std::string signal_name, signal_weight;
  bool debug= false;
  
  
};

TTree* TreeDrawMain::load_tree(std::string fileDir){
  Long_t id, size, flag, modtime;
  
  if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
    cerr<<"File "<<fileDir.c_str()<<" does not exist"<<endl;
    exit(EXIT_FAILURE);
  }
  TFile* file = new TFile(fileDir.c_str(),"READ");
  if(file->IsZombie()){
    cerr<<"File "<<fileDir<<" is a Zombie"<<endl;
    exit(EXIT_FAILURE);
  }
  //cout<<"FileName "<<fileDir<<" Tree "<<treename<<endl;
  return (TTree*)file->Get(treename.c_str());
}

TH1F* TreeDrawMain::make_hist(TTree* mytree, std::string variable, std::string draw_option,string hist_binning){
  if(hist_binning.empty()){
    mytree->Draw((variable+">>myTmpHist").c_str(),draw_option.c_str());
  }
  else{
    //std::cout<<"binning "<<binning<<std::endl;
    mytree->Draw((variable+">>myTmpHist("+hist_binning+")").c_str(),draw_option.c_str());
    //std::cout<<"Hist drawn "<<std::endl; 
  }
  return (TH1F*)gPad->GetPrimitive("myTmpHist");
}

bool TreeDrawMain::create_file(std::string fileName){
  string fileName_ = fileName;
  if(!boost::algorithm::ends_with(fileName,".root"))
    fileName_ = fileName_+".root";
  cout<<"Creating file "<<fileName_<<endl;  
  TFile* result_file  = new TFile(fileName_.c_str(),"RECREATE");
  for(auto & dir : FileDir){
    find_samples_nicks(dir);
    set_channel(dir);
    int nick_number = 0;
    TTree* signal_tree =0;
    if(!signal_name.empty() && !signal_weight.empty()){
      for(auto & process : working_samples){
	if(signal_name.compare(nicks[nick_number])==0){
	  signal_tree = load_tree(dir+"/"+process);
	  nick_number=0;
	  break;
	}
	nick_number++;
      }
      if(nick_number>0){
	std::cerr<<"Did not find the signal to inject. Aborting!"<<std::endl;
	std::cerr<<"Signal "<< signal_name<<std::endl;
	std::cerr<<"List of nicks"<<std::endl;
	for(auto i : nicks)
	  std::cerr<<i<<std::endl;
	return 10;
      }
    }
    //The main histograms + errors that can be stored into weights
    for(auto & process : working_samples){
      std::string nick =  nicks[nick_number];
      string test_string = nick;
      boost::algorithm::to_lower(test_string);
      cout<<"loading tree for "<< process<<endl;
      if (debug)cout<<"loading tree for "<< process<<endl;
      TTree* mytree = load_tree(dir+"/"+process);
      for(auto & hist : infoVec){
	//debug =true;
	if (debug)cout<<"making histogram "<<hist.draw_command<<" selection "<<hist.selection<<endl;
	//debug=false;
	TH1F* tmp_hist = make_hist(mytree,hist.draw_command,hist.selection,hist.binning);
	if(tmp_hist->GetEntries() ==0 || tmp_hist->GetBinContent(0)!= tmp_hist->GetBinContent(0)){
	  std::cout<<"no entries found ignoring this process "<< nick<<" draw command "<<hist.draw_command <<std::endl;
	  continue;
	}
	if(hist.scale >0) tmp_hist->Scale(hist.scale);
	if(!signal_name.empty() && !signal_weight.empty() && !boost::algorithm::contains(nick,"Bprime") && !boost::algorithm::contains(nick,"X") && boost::algorithm::contains(hist.hist_name,"background") && signal_tree){
	  if (debug)std::cout<<"getting histogram for "<<hist.hist_name<<" "<<hist.draw_command<<" "<<hist.selection<<std::endl;
	  TH1F* signal_injected = make_hist(signal_tree,hist.draw_command,hist.selection,hist.binning);
	  signal_injected->Scale(stod(signal_weight));
	  tmp_hist->Add(signal_injected);
	}
	//missing some naming part
	cout<<"main hist name: "<<hist.hist_name+channel+"__"+nick<<endl;
	tmp_hist->SetName((hist.hist_name+channel+"__"+nick).c_str());
	tmp_hist->SetTitle("VLQ mass [GeV]");
	 //cout<<"Hist Name: "<<tmp_hist->GetName()<<" pre fit entries "<<tmp_hist->GetEntries()<<endl;
	if(!boost::algorithm::contains(test_string,"data") && boost::algorithm::contains(hist.hist_name,"background")){
	  continue;
	}
	if(boost::algorithm::contains(hist.hist_name,"background"))
	  tmp_hist->SetName((hist.hist_name.substr(0,hist.hist_name.size()-11)+channel+"__"+"Background").c_str());
	result_file->cd();
	tmp_hist->Write();
	if(boost::algorithm::contains(test_string,"data"))continue;
	int error_counter=0;
	//All errors that can be stored into weights in the final tree
	int index = -1;
	//cout<<"starting with the weight errors"<<endl;
	for(auto & error : error_weight){
	  //cout<<error<<endl;
          index++;
	  TH1F* tmp_errorHist;
	  error_method method  = error_methods_container.at(error_counter);
          string tmp_draw_command = hist.draw_command;
          string tmp_hist_sel = hist.selection;
          if(!error_replace_draw[index].first.empty() && !error_replace_draw[index].second.empty()){
	    //std::cout<<tmp_draw_command<<" replacing "<<error_replace_draw[index].first<<" with  "<<error_replace_draw[index].second<<std::endl;
	    boost::replace_all(tmp_draw_command,error_replace_draw[index].first,error_replace_draw[index].second);
	    //std::cout<<tmp_draw_command<<std::endl;
	  }
          if(!error_replace_weight[index].first.empty() && !error_replace_weight[index].second.empty()){
            boost::replace_all(tmp_hist_sel,error_replace_weight[index].first,error_replace_weight[index].second);
          }
	  
	  if(method == envelop){    
	    tmp_errorHist = make_hist(mytree,tmp_draw_command,error+"*"+tmp_hist_sel+"*("+error+">-1)",hist.binning);
	    if(debug)std::cout<< error_names.at(error_counter)<<" number of entries "<<tmp_errorHist->GetEntries()<<std::endl;
	    if(tmp_errorHist->GetEntries() ==0 || tmp_errorHist->GetBinContent(0)!= tmp_errorHist->GetBinContent(0)){
	      std::cout<<"no entries found ignoring this error "<< error_names.at(error_counter)<<" for process "<<nick<<std::endl;
	      error_counter++;
	      continue;
	    }
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)).c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	  }
	  else if(method == rms){
	    //double hist_integral = tmp_hist->Integral();
	    tmp_errorHist = make_hist(mytree,tmp_draw_command,error+"*"+tmp_hist_sel+"*("+error+">-1)",hist.binning);
	    if(debug)std::cout<<tmp_draw_command<<" "<<tmp_hist_sel+"*("+error+">-1)"<<std::endl;
	    if(debug)std::cout<< error_names.at(error_counter)<<" number of entries "<<tmp_errorHist->GetEntries()<<std::endl;
	    if(tmp_errorHist->GetEntries() ==0 || tmp_errorHist->GetBinContent(0)!= tmp_errorHist->GetBinContent(0)){
	      std::cout<<"no entries found ignoring this error "<< error_names.at(error_counter)<<" for process "<<nick<<std::endl;
	      error_counter++;
	      continue;
	    }
	    //double histerror_integral = tmp_errorHist->Integral();
	    //tmp_errorHist->Scale(hist_integral/histerror_integral);
	    tmp_errorHist->Add(tmp_hist);
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)+"__plus").c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	    tmp_errorHist = make_hist(mytree,tmp_draw_command,error+"*"+tmp_hist_sel+"*("+error+">-1)",hist.binning);
	    //histerror_integral = tmp_errorHist->Integral();
	    tmp_errorHist->Add(tmp_errorHist,tmp_hist,-1);
	    //tmp_errorHist->Scale(hist_integral/histerror_integral);
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)+"__minus").c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	  }
	  error_counter++;
	}
      }
      //exit(0);
      nick_number++;
      delete mytree;
      //delete signal_tree;
    }
  }
  if(debug)cout<<"****************************************************"<<endl;
  //For the cases I need a to run the preselection again and things are stored into a different dir
  int error_nick = 0;
  for(auto dir :  dir_errors){
    if(debug)cout<<"==================================================="<<endl;
    if(debug)cout<<"==================================================="<<endl;
    if(debug)cout<<dir<<endl;
    set_channel(dir);
    find_samples_nicks(dir);
    int nick_number = 0;
    for(auto & process : working_samples){
      std::string nick =  nicks[nick_number];
      if(boost::algorithm::ends_with(nick,"DATA")){
	nick_number++;
	continue;
      }
      if(debug)cout<<"==================================================="<<endl;
      //debug=true;
      if(debug)cout<<dir+"/"+process<<endl;
      if(debug)cout<<"rick "<< nick <<" nick number "<<nick_number<<" of "<< nicks.size()<<" error nick "<< error_nick <<" of "<<dir_errors_nick.size() <<endl;
     
      TTree* mytree = load_tree(dir+"/"+process);
      for(auto & hist : infoVec){ 
	if(debug)cout<<hist.draw_command<<" "<<hist.selection<<endl;
	if(boost::algorithm::contains(hist.hist_name,"background")) continue;
	string tmp_name = hist.hist_name+channel+"__"+nick+"__"+dir_errors_nick[error_nick];
	TH1F* tmp_hist = make_hist(mytree,hist.draw_command,hist.selection,hist.binning);
	//if(tmp_hist->GetEntries()==0)continue;
	if(debug)cout<<dir_errors_nick[error_nick]<<" number of entries "<<tmp_hist->GetEntries()<<endl;
	if(debug)cout<<"tmp hist name "<<tmp_name <<endl;
	tmp_hist->SetName(tmp_name.c_str());
	result_file->cd();
	tmp_hist->Write();
      }
       //debug=false;
      nick_number++;
      delete mytree;
    }
    error_nick++;
  }

  result_file->Close();
  //delete result_file;
  gROOT->GetListOfFiles()->Delete();
  return true;
}

void TreeDrawMain::set_channel(std::string dir){
  string lower_filedir = dir;
  if(boost::algorithm::contains(lower_filedir,"Ele"))
    channel = "Ele";
  else
    channel = "Mu";
}
void TreeDrawMain::find_samples_nicks(std::string dir){
  std::vector<std::string> empty_nicks, empty_work;
  nicks.swap(empty_nicks);working_samples.swap(empty_work);
  for(unsigned int i =0 ; i < Sample.size(); i++){
    for(auto match : find_matches(dir+"/",Sample[i])){
      working_samples.push_back(match);
      std::vector<std::string> splitted_match;
      boost::split(splitted_match,match,boost::is_any_of("."));
      string lower_nick = splitted_match[splitted_match.size()-2];
      boost::algorithm::to_lower(lower_nick);
      //cout<<"nick lowered "<<lower_nick<<endl;
      if(boost::algorithm::contains(lower_nick,"data"))
	nicks.push_back("DATA");
      else{
        std::string name = splitted_match[splitted_match.size()-2];
	boost::replace_all(name,"-","_");
	//boost::replace_all(name,"X53","X");
	nicks.push_back(name);
      }
      if(debug)cout<<splitted_match[splitted_match.size()-2]<<endl;
    }
  }
}
std::vector<std::string> TreeDrawMain::find_matches(std::string dir,std::string name){
  std::vector<std::string> matches;
  std::vector<std::string> splitted_name;
  boost::split(splitted_name,name,boost::is_any_of("*"));
  boost::filesystem::directory_iterator end_iter;
  for(boost::filesystem::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter){
    if(!boost::algorithm::ends_with(dir_iter->path().filename().string(),".root")) continue;
    bool contains_parts = true;
    for(auto part : splitted_name){
      if(!boost::algorithm::contains(dir_iter->path().filename().string(),part)) contains_parts = false;
    }
    if(boost::algorithm::ends_with(dir_iter->path().filename().string(),".root") && contains_parts){
      matches.push_back(dir_iter->path().filename().string());
    }
  }
  return matches;
}


int RootFileCreator(string signal="LH_25ns.root", string resultfile="TESTME1.root", string dirnames="/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v31/", string channel = "", string signal_injection ="", string signal_weight="", string mode=""){
  TH1::AddDirectory(kFALSE);
  //std::vector<std::string> directories = {dirname+"/Selection_v31/",dirname+"/EleSelection_v6_tree/"};//,dirname+"/EleSelection_v5_tree/"
  std::vector<std::string> directories; 
  boost::split(directories,dirnames,boost::is_any_of(","));
  //std::vector<std::string> samples = {"SingleT_s.root","SingleT_t.root","SingleTWAntitop.root","SingleTWtop.root","ZJets.root","TTbar.root","WJets_Pt.root","QCD.root","DATA.root",signal};
  std::vector<std::string> samples = {signal};
  if(boost::algorithm::contains(signal,",")){
    boost::algorithm::trim(signal);
    boost::split(samples,signal,boost::is_any_of(","));
  }
  //std::vector<std::string> samples = {"SingleT_s.root","SingleTWAntitop.root","SingleTWtop.root","ZJets.root","TTbar.root","WJets_Pt.root","QCD.root","DATA.root",signal};
  //std::vector<std::string> samples = {"Data.root",signal};  //Options DATA.root MC.root; For 2016: Data.root  
  cout<<"Starting histogram production"<<endl;
  
  std::cout<<"Directories"<<std::endl;
  for(auto it : directories)
    std::cout<<it<<std::endl;
  std::cout<<"Result File "<<resultfile<<".root"<<std::endl;
  std::cout<<"Samples "<<std::endl;
  for(auto it : samples)
    std::cout<<it<<std::endl;
  

  bool data_driven = boost::algorithm::contains(mode,"DATA") || mode.empty() ? true: false;
  
  bool errors = true;

  
  bool test =  !(mode.empty() || boost::algorithm::contains(mode,"DATA") || mode =="mc");
  if(test) errors = false;
  string category="";
  if(mode == "forward_errors" || mode == "central_errors"){
    errors =true;
    if(mode == "forward_errors") mode = "forward";
    else if(mode== "central_errors") mode = "central";
    category = mode;
  }


  string binning ="60,0,3500";
  TreeDrawMain mainClass(binning,"AnalysisTree");
  mainClass.AddFileDir(directories);
  mainClass.AddSamples(samples);
  mainClass.inject_signal(signal_injection,signal_weight);

  string eta = "2.4";
 
  string chiprob_chi2_f = "";//"&& Chi2Dis.chi<0.05";
  string chiprob_toptag_f = "";//"&& TopTagDis.chi<0.05";
  string chiprob_chi2_c = "";//"&& Chi2Dis.chi<0.05";
  string chiprob_toptag_c = "";//"&& TopTagDis.chi<0.05";
  
  string factor_forwardjet = forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400");
  string factors = "";
  if(boost::algorithm::contains(channel,"Ele")) {
	factors = "("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
        //factors += "(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]<650)*";
  }
  else factors = "("+muon_trk_factors()+"*(1-isRealData)+isRealData)*";
  factors += factor_forwardjet;
  //factors += "(1-isRealData)+isRealData)"

  string cmssw_version = "8_0_24_patch1";
  string errordir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/";

  cout<<"Factors: "<<factors<<endl; 
  /*
  **
  ** Background from MC
  */
  string chi2_forward_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) >="+eta; // && Chi2Dis.forwardJet.E()>="+energy;
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) <" +eta;// || Chi2Dis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  //string chi2_forward_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) >="+eta; // && Chi2Dis.forwardJet.E()>="+energy;
  //string chi2_central_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) <" +eta;// || Chi2Dis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  if(test){
    cout<<"This is the test analysis module running on: "<<mode<<endl;
    if(mode == "chi2" ){      
      mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) >="+eta+" && WTagDis.mass>0)","Chi2_Forward");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) <" +eta+" && WTagDis.mass>0)","Chi2_Central");
    }
    if(mode == "wreco"){
      mainClass.AddHistCategory("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(WTagDis.forwardJet.eta()) >= "+eta+")","WTag_Forward");
      mainClass.AddHistCategory("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(WTagDis.forwardJet.eta()) <  "+eta+")","WTag_Central");
    }
    if(mode == "toptagreco"){
      mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && abs(TopTagDis.forwardJet.eta()) >= "+eta+")","TopTag_Forward");
      mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && abs(TopTagDis.forwardJet.eta()) <  "+eta+")","TopTag_Central");
    }

    if(mode == "chi2_btag_only" ){
      chi2_forward_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) >="+eta; 
      chi2_central_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) <" +eta;

      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Forward_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Forward_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Forward_2_BTag");
      
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Central_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Central_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Central_2_BTag");
    }
    if(mode == "chi2_btag" ){
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Forward_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Forward_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Forward_2_BTag");
      
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Central_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Central_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Central_2_BTag");
    }
    if(mode == "chi2_wtag_cat" ){
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0)","Chi2_Forward_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1)","Chi2_Forward_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1)","Chi2_Forward_2_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*( WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) < "+eta+")","WTag_Central");

      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Central_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Central_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Central_2_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*( WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) >="+eta+")","WTag_Forward");
    }    
    if(mode == "chi2_wtag" ){
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0)","Chi2_Forward_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1)","Chi2_Forward_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1)","Chi2_Forward_2_BTag");
      
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Central_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Central_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Central_2_BTag");
      mainClass.AddHistCategory("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(WTagDis.forwardJet.eta()) >= "+eta+")","WTag_Forward");
      mainClass.AddHistCategory("WTagDis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(WTagDis.forwardJet.eta()) < "+eta+")","WTag_Central");
    }
    if(mode == "combi" ){
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0)","Chi2_Forward_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1)","Chi2_Forward_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1)","Chi2_Forward_2_BTag");
      
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber ==0 )","Chi2_Central_AntiBTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber ==1 )","Chi2_Central_1_BTag");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber > 1 )","Chi2_Central_2_BTag");
      
      mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(WTagDis.forwardJet.eta()) >= "+eta+" && Chi2Dis.btagEventNumber > 0)","WTag_Forward");
      mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(WTagDis.forwardJet.eta()) < "+eta+")","WTag_Central");
      
      mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && abs(TopTagDis.forwardJet.eta()) >= "+eta+")","TopTag_Forward");
      mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && abs(TopTagDis.forwardJet.eta()) < "+eta+")","TopTag_Central");
    }
    if(mode =="forward"){
       mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0"+chiprob_chi2_f+")","Chi2_AntiBTag"+category,binning);
       mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1"+chiprob_chi2_f+")","Chi2_1_BTag"+category,binning);
       mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1"+chiprob_chi2_f+")","Chi2_2_BTag"+category,binning);
       mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400)"+chiprob_chi2_f+"&& WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) >= "+eta+" && Chi2Dis.btagEventNumber > 0)","Chi2_WTag"+category,binning);
       mainClass.AddHistCategory("TopTagDis.mass",factors+"weight*(TopTagDis.mass >-1 && TopTagDis.topHad.pt()>400"+chiprob_toptag_f+" && abs(TopTagDis.forwardJet.eta()) >= "+eta+")","TopTag"+category,binning);
       //mainClass.AddWeightError("scaleWeight_up","scaleWeight__plus");
       //mainClass.AddWeightError("scaleWeight_down","scaleWeight__minus");
    }
    if(mode =="central"){
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==0"+chiprob_chi2_c+")","Chi2_AntiBTag"+category,binning);
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==1"+chiprob_chi2_c+")","Chi2_1_BTag"+category,binning);
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber > 1"+chiprob_chi2_c+")","Chi2_2_BTag"+category,binning);
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400)"+chiprob_chi2_c+"&& WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) < "+eta+")","Chi2_WTag"+category,binning);
      mainClass.AddHistCategory("TopTagDis.mass",factors+"weight*(TopTagDis.mass >-1 && TopTagDis.topHad.pt()>400 "+chiprob_toptag_c+"  && abs(TopTagDis.forwardJet.eta()) < "+eta+")","TopTag"+category,binning);
       //mainClass.AddWeightError("scaleWeight_up","scaleWeight__plus");
       //mainClass.AddWeightError("scaleWeight_down","scaleWeight__minus");


    }
    if(errors){
      mainClass.AddWeightError("scaleWeight_up","scaleWeight__plus");
      mainClass.AddWeightError("scaleWeight_down","scaleWeight__minus");
      mainClass.AddWeightError("pdfWeight","PDF",TreeDrawMain::error_method::rms);
      mainClass.AddWeightError("weight_btag_up/weight_btag","btag__plus");
      mainClass.AddWeightError("weight_btag_down/weight_btag","btag__minus");
      mainClass.AddWeightError("weight_pu_up/weight_pu","pu__plus");
      mainClass.AddWeightError("weight_pu_down/weight_pu","pu__minus");
       
      mainClass.AddWeightError("("+factor_forwardjet+"1)","forwardjet__minus");
      mainClass.AddWeightError("(1/"+factor_forwardjet+"1)","forwardjet__plus");
      vector<string> channel_dirs;
      if(channel.empty() || boost::iequals(channel,"Ele")) channel_dirs.push_back("Ele");
      if(channel.empty() || boost::iequals(channel,"Mu")) channel_dirs.push_back("Mu");
      if(boost::algorithm::contains(channel,"Ele")){
	mainClass.AddWeightError("weight_sfelec_eleid_up/weight_sfelec_eleid","eleid__plus");
	mainClass.AddWeightError("weight_sfelec_eleid_down/weight_sfelec_eleid","eleid__minus");
	mainClass.AddWeightError("weight_sfelec_elereco_up/weight_sfelec_elereco","elereco__plus");
	mainClass.AddWeightError("weight_sfelec_elereco_down/weight_sfelec_elereco","elereco__minus");
	mainClass.AddWeightError(eletriggerscale(1)+"*(1+ 0.04*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]<120))","eletrigger__plus");
	mainClass.AddWeightError(eletriggerscale(2)+"*(1- 0.04*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]<120))","eletrigger__minus"); 
      }
      else{   
	mainClass.AddWeightError(muon_trk_factors(1),"muon_trk__plus");
	mainClass.AddWeightError(muon_trk_factors(-1),"muon_trk__minus");
	mainClass.AddWeightError("weight_sfmu_tight_up/weight_sfmu_tight","muid__plus");
	mainClass.AddWeightError("weight_sfmu_tight_down/weight_sfmu_tight","muid__minus");
	mainClass.AddWeightError("weight_sfmu_muonTrigger_up/weight_sfmu_muonTrigger","muontrigger__plus");
	mainClass.AddWeightError("weight_sfmu_muonTrigger_down/weight_sfmu_muonTrigger","muontrigger__minus");
      
      }
      mainClass.AddDirError(errordir+"/sig_jec_jer_up_"+channel+"SigSelUNC/"  ,"jec__plus");
      mainClass.AddDirError(errordir+"/sig_jec_jer_down_"+channel+"SigSelUNC/","jec__minus");

      
      mainClass.AddWeightError("1","toptagJEC__plus",TreeDrawMain::error_method::envelop,"TopTagDis","toptag_dis_jec_up");
      mainClass.AddWeightError("1","toptagJEC__minus",TreeDrawMain::error_method::envelop,"TopTagDis","toptag_dis_jec_down");
      mainClass.AddWeightError("1","wtagJEC__plus",TreeDrawMain::error_method::envelop,"WTagDis","wtag_dis_jec_up");
      mainClass.AddWeightError("1","wtagJEC__minus",TreeDrawMain::error_method::envelop,"WTagDis","wtag_dis_jec_down");
    }
    
    mainClass.create_file(resultfile);
    return 0;
  }
  
  if(!data_driven){
    mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Forward_AntiBTag",binning);
    mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Forward_1_BTag",binning);
    mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Forward_2_BTag",binning);
    
    mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) >= "+eta+" && Chi2Dis.btagEventNumber > 0)","WTag_Forward",binning);
    mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && abs(TopTagDis.forwardJet.eta()) >= "+eta+")","TopTag_Forward",binning);

    mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==0 )","Chi2_Central_AntiBTag",binning);
    mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber ==1 )","Chi2_Central_1_BTag",binning);
    mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+" && Chi2Dis.btagEventNumber > 1 )","Chi2_Central_2_BTag",binning);
    mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) < "+eta+")","WTag_Central",binning);
    mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && abs(TopTagDis.forwardJet.eta()) < "+eta+")","TopTag_Central",binning);

  }
  
  /*
  **
  ** Data driven background
  */
  if(data_driven){
    cout<<"Going to work datadriven"<<endl;
    
    std::vector<double>  background_weights = {0.575081861061,0.655865763495,0.676520110615,0.591298041218,0.632555143075};//0.48903,0.5586,0.565837,0.513547,0.581932};//{0.229,0.277,0.2999,0.282};//{0.37, 0.43, 0.43, 0.45 };
    
    if(boost::algorithm::contains(channel,"Ele")){
      background_weights = {0.583080772624,0.66111562261,0.689675816353,0.627456490514,0.599310688503};//0.484909,0.564976,0.565503,0.51387,0.588962};
    }

    background_weights = {1.,1.,1.,1.,1.,1.};
    
    
    bool forward = true, central =true;
    if(mode=="DATA_central")forward=false;
    else if(mode=="DATA_forward")central=false;
    

    if(forward){
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0"+chiprob_chi2_f+")","Chi2_AntiBTag",binning);	    
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1"+chiprob_chi2_f+")","Chi2_1_BTag",binning);	    
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1"+chiprob_chi2_f+")","Chi2_2_BTag",binning);	    
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) "+chiprob_chi2_f+"&& WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) >= "+eta+" && Chi2Dis.btagEventNumber > 0)","Chi2_WTag",binning);
      mainClass.AddHistCategory("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 "+chiprob_toptag_f+"&& abs(TopTagDis.forwardJet.eta())>="+eta+")","TopTag",binning);		
    }
    if(central){
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==0"+chiprob_chi2_c+")","Chi2_AntiBTag:background",binning,background_weights[0]);//0.388519); 
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==1"+chiprob_chi2_c+")","Chi2_1_BTag:background"  ,binning,background_weights[1]);//0.471137); 
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber> 1"+chiprob_chi2_c+")","Chi2_2_BTag:background"  ,binning,background_weights[2]);//0.467834);    
      mainClass.AddHistCategory("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400)"+chiprob_chi2_c+" && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) < "+eta+")","Chi2_WTag:background",binning,background_weights[3]);
      mainClass.AddHistCategory("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 "+chiprob_toptag_c+" && abs(TopTagDis.forwardJet.eta())<"+eta+")","TopTag:background",binning,background_weights[4]);
    }
  }
  //errors = false;
  if(errors){
    mainClass.AddWeightError("scaleWeight_up","scaleWeight__plus");
    mainClass.AddWeightError("scaleWeight_down","scaleWeight__minus");
    mainClass.AddWeightError("pdfWeight","PDF",TreeDrawMain::error_method::rms);
    mainClass.AddWeightError("weight_btag_up/weight_btag","btag__plus");
    mainClass.AddWeightError("weight_btag_down/weight_btag","btag__minus");
    mainClass.AddWeightError("weight_pu_up/weight_pu","pu__plus");
    mainClass.AddWeightError("weight_pu_down/weight_pu","pu__minus");
       
    /*/
    mainClass.AddDirError(errordir+"/jec_up_"+channel+"SelUNC/"  ,"jec__plus");
    mainClass.AddDirError(errordir+"/jec_down_"+channel+"SelUNC/","jec__minus");
    mainClass.AddDirError(errordir+"/jer_up_"+channel+"SelUNC/"  ,"jer__plus");
    mainClass.AddDirError(errordir+"/jer_down_"+channel+"SelUNC/","jer__minus");
    /*/
    mainClass.AddWeightError("("+factor_forwardjet+"1)","forwardjet__minus");
    mainClass.AddWeightError("(1/"+factor_forwardjet+"1)","forwardjet__plus");
    vector<string> channel_dirs;
    if(channel.empty() || boost::iequals(channel,"Ele")) channel_dirs.push_back("Ele");
    if(channel.empty() || boost::iequals(channel,"Mu")) channel_dirs.push_back("Mu");
    if(boost::algorithm::contains(channel,"Ele")){
      mainClass.AddWeightError("weight_sfelec_eleid_up/weight_sfelec_eleid","eleid__plus");
      mainClass.AddWeightError("weight_sfelec_eleid_down/weight_sfelec_eleid","eleid__minus");
      mainClass.AddWeightError("weight_sfelec_elereco_up/weight_sfelec_elereco","elereco__plus");
      mainClass.AddWeightError("weight_sfelec_elereco_down/weight_sfelec_elereco","elereco__minus");
      mainClass.AddWeightError(eletriggerscale(1)+"*(1+ 0.04*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]<120))","eletrigger__plus");
      mainClass.AddWeightError(eletriggerscale(2)+"*(1- 0.04*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]<120))","eletrigger__minus"); 
    }
    else{   
      mainClass.AddWeightError(muon_trk_factors(1),"muon_trk__plus");
      mainClass.AddWeightError(muon_trk_factors(-1),"muon_trk__minus");
      mainClass.AddWeightError("weight_sfmu_tight_up/weight_sfmu_tight","muid__plus");
      mainClass.AddWeightError("weight_sfmu_tight_down/weight_sfmu_tight","muid__minus");
      mainClass.AddWeightError("weight_sfmu_muonTrigger_up/weight_sfmu_muonTrigger","muontrigger__plus");
      mainClass.AddWeightError("weight_sfmu_muonTrigger_down/weight_sfmu_muonTrigger","muontrigger__minus");
      
    }
    /*/
    mainClass.AddDirError(errordir+"/jec_up_"+channel+"SelUNC/"  ,"jec__plus");
    mainClass.AddDirError(errordir+"/jec_down_"+channel+"SelUNC/","jec__minus");
    mainClass.AddDirError(errordir+"/jer_up_"+channel+"SelUNC/"  ,"jer__plus");
    mainClass.AddDirError(errordir+"/jer_down_"+channel+"SelUNC/","jer__minus");
    /*/
    /*/
    mainClass.AddDirError(errordir+"/sig_jec_jer_up_"+channel+"SigSelUNC/"  ,"jec__plus");
    mainClass.AddDirError(errordir+"/sig_jec_jer_down_"+channel+"SigSelUNC/","jec__minus");

    mainClass.AddWeightError("1","toptagJEC__plus",TreeDrawMain::error_method::envelop,"TopTagDis.mass","toptag_dis_jec_up.mass");
    mainClass.AddWeightError("1","toptagJEC__minus",TreeDrawMain::error_method::envelop,"TopTagDis.mass","toptag_dis_jec_down.mass");
    mainClass.AddWeightError("1","wtagJEC__plus",TreeDrawMain::error_method::envelop,"","","WTagDis","wtag_dis_jec_up");
    mainClass.AddWeightError("1","wtagJEC__minus",TreeDrawMain::error_method::envelop,"","","WTagDis","wtag_dis_jec_down");
    //mainClass.AddDirError(errordir+"/sig_jer_up_"+channel+"SigSelUNC/"  ,"jer__plus");
    //mainClass.AddDirError(errordir+"/sig_jer_down_"+channel+"SigSelUNC/","jer__minus");
    /*/
  }

  mainClass.create_file(resultfile);
  //gApplication->Terminate();
  return 0;
}

int main(int argc, char **argv){
  if(argc == 1)
    return RootFileCreator();
  else if(argc == 2)
    return RootFileCreator(argv[1]);
  else if(argc == 3)
    return RootFileCreator(argv[1],argv[2]);
  else if(argc == 4)
    return RootFileCreator(argv[1],argv[2],argv[3]);
  else if(argc == 5)
    return RootFileCreator(argv[1],argv[2],argv[3],argv[4]);
  else if(argc == 6)
    return RootFileCreator(argv[1],argv[2],argv[3],argv[4],argv[5]);
  else if(argc == 7)
    return RootFileCreator(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
  else if(argc == 8)
    return RootFileCreator(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
  else{
    std::cerr<<"Wrong number of arguments"<<std::endl;
    return 1;
  }
}

