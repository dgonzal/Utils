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


using namespace std;

struct TreeDrawInfo{
  TreeDrawInfo( std::string draw_command_, std::string selection_, std::string hist_name_):draw_command(draw_command_),selection(selection_),hist_name(hist_name_){;}
  std::string draw_command;
  std::string selection;
  std::string hist_name;
};

class TreeDrawMain{
public:
  TreeDrawMain(std::string binning_, std::string treename_){binning = binning_;treename =treename_;};
  enum error_method{envelop,rms};

  void AddFileDir(std::string file){FileDir.push_back(file);}
  void AddFileDir(std::vector<std::string> dirs){ FileDir = dirs;}
  void AddSamples(std::vector<std::string> sample_){Sample =sample_;}

  void AddHistCategory(std::string draw_command_, std::string selection_, std::string hist_name_){infoVec.push_back(TreeDrawInfo(draw_command_,selection_,hist_name_));}
  void AddWeightError(std::string weight_option, std::string error_name, error_method method=envelop){error_weight.push_back(weight_option);error_methods_container.push_back(method);error_names.push_back(error_name);}

  bool create_file(std::string fileName);

private:
  TH1F* make_hist(TTree* mytree, std::string variable, std::string draw_option);
  TTree* load_tree(std::string fileDir);
  std::vector<std::string> find_matches(std::string dir,std::string name);

  //std::vector<TH1F*> hist;
  //std::vector<TH1F*> error_hist;
  std::vector<std::string> error_weight;
  std::vector<error_method> error_methods_container;
  std::vector<std::string> error_names;
  std::vector<TreeDrawInfo> infoVec;
  std::vector<std::string> FileDir;
  std::vector<std::string> Sample;
  std::string treename;
  std::string binning;
  std::string channel="";
};

TTree* TreeDrawMain::load_tree(std::string fileDir){
  Long_t id, size, flag, modtime;
  
  if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
    cerr<<"File "<<fileDir.c_str()<<" does not exist"<<endl;
    exit(EXIT_FAILURE);
  }
  TFile* file = new TFile(fileDir.c_str());
  if(file->IsZombie()){
    cerr<<"File "<<fileDir<<" is a Zombie"<<endl;
    exit(EXIT_FAILURE);
  }
  //cout<<"FileName "<<fileDir<<" Tree "<<treename<<endl;
  return (TTree*)file->Get(treename.c_str());
}

TH1F* TreeDrawMain::make_hist(TTree* mytree, std::string variable, std::string draw_option){
  if(binning.empty()){
    mytree->Draw((variable+">>myTmpHist").c_str(),draw_option.c_str());
  }
  else{
    //std::cout<<"binning "<<binning<<std::endl;
    mytree->Draw((variable+">>myTmpHist("+binning+")").c_str(),draw_option.c_str());
    //std::cout<<"Hist drawn "<<std::endl; 
  }
  return (TH1F*)gPad->GetPrimitive("myTmpHist");
}

bool TreeDrawMain::create_file(std::string fileName){
  string fileName_ = fileName;
  if(!boost::algorithm::ends_with(fileName,".root"))
    fileName_ = fileName_+".root";
  cout<<"Creating file "<<fileName<<endl;  
  TFile* result_file  = new TFile(fileName_.c_str(),"RECREATE");
  for(auto & dir : FileDir){
    std::vector<std::string> working_samples;
    std::vector<std::string> nicks;
    string lower_filedir = dir;
    //boost::algorithm::to_lower(lower_filedir);
      if(boost::algorithm::contains(lower_filedir,"Ele"))
	channel = "Ele";
      else
	channel = "Mu";

    for(unsigned int i =0 ; i < Sample.size(); i++){
      for(auto match : find_matches(dir,Sample[i])){
	working_samples.push_back(match);
	std::vector<std::string> splitted_match;
	boost::split(splitted_match,match,boost::is_any_of("."));
	string lower_nick = splitted_match[splitted_match.size()-2];
	boost::algorithm::to_lower(lower_nick);
	if(boost::algorithm::contains(lower_nick,"data"))
	  nicks.push_back("DATA");
	else
	  nicks.push_back(splitted_match[splitted_match.size()-2]);
	//cout<<splitted_match[splitted_match.size()-2]<<endl;
      }
    }
    int nick_number = 0;
    for(auto & process : working_samples){
      std::string nick =  nicks[nick_number];
      //cout<<"loading tree for "<< process<<endl;
      TTree* mytree = load_tree(dir+"/"+process);
      for(auto & hist : infoVec){
	//cout<<"making histogram "<<hist.draw_command<<" selection "<<hist.selection<<endl;
	TH1F* tmp_hist = make_hist(mytree,hist.draw_command,hist.selection);
	//cout<<"histogram done"<<endl;
	//missing some naming part
	tmp_hist->SetName((hist.hist_name+channel+"__"+nick).c_str());
	result_file->cd();
	tmp_hist->Write();
	string test_string = nick;
	boost::algorithm::to_lower(test_string);
	if(boost::algorithm::contains(test_string,"data"))continue;
	int error_counter=0;
	for(auto & error : error_weight){
	  TH1F* tmp_errorHist;
	  error_method method  = error_methods_container.at(error_counter);
	  if(method == envelop){
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection);
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)).c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	  }
	  else if(method == rms){
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection+"+"+hist.selection);
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)+"__plus").c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	    tmp_errorHist = make_hist(mytree,hist.draw_command,hist.selection+"-"+error+"*"+hist.selection);
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)+"__minus").c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	  }
	  error_counter++;
	}
      }
      nick_number++;
      delete mytree;
    }
  }
  result_file->Close();
  delete result_file;
  return true;
}

std::vector<std::string> TreeDrawMain::find_matches(std::string dir,std::string name){
  std::vector<std::string> matches;
  std::vector<std::string> splitted_name;
  boost::split(splitted_name,name,boost::is_any_of("*"));
  boost::filesystem::directory_iterator end_iter;
  for(boost::filesystem::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter){
    if(!boost::algorithm::ends_with(dir_iter->path().filename().string(),".root")) continue;
    bool contains_parts = true;
    /*
    cout<<"=================="<<endl;
    cout<<dir_iter->path().filename().string()<<endl;
    */
    for(auto part : splitted_name){
      if(!boost::algorithm::contains(dir_iter->path().filename().string(),part)) contains_parts = false;
      //cout<<part<<" ? "<<contains_parts<<endl;;
    }
    if(boost::algorithm::ends_with(dir_iter->path().filename().string(),".root") && contains_parts){
      matches.push_back(dir_iter->path().filename().string());
      //cout<<dir_iter->path().filename().string()<<endl;
    }
  }
  /*
  cout<<"Matches ";
  for(auto match : matches)
    cout<<match<<" ";
  cout<<endl;
  */
  return matches;
}
int RootFileCreator(string signal="LH_25ns.root", string resultfile="TESTME1.root", string dirnames="/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v31/"){
  TH1::AddDirectory(kFALSE);
  //std::vector<std::string> directories = {dirname+"/Selection_v31/",dirname+"/EleSelection_v6_tree/"};//,dirname+"/EleSelection_v5_tree/"
  std::vector<std::string> directories; 
  boost::split(directories,dirnames,boost::is_any_of(","));
  /*
  std::cout<<"Directories"<<std::endl;
  for(auto it : directories)
    std::cout<<it<<std::endl;
  std::cout<<"Result File "<<resultfile<<std::endl;
  */
  std::vector<std::string> samples = {"SingleTsChannel.root","SingleTtChannel.root","SingleTWAntitop.root","SingleTWTop.root","ZJets.root","TTJets.root","WJets.root","QCD.root","DATA.root",signal};
 
  //cout<<"Starting histogram production"<<endl;
  

  TreeDrawMain mainClass("100,50,3000","AnalysisTree");
  mainClass.AddFileDir(directories);
  mainClass.AddSamples(samples);

  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","Chi2_Forward_AntiBTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","Chi2_Forward_1_BTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber > 1)","Chi2_Forward_2_BTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==0)","Chi2_Central_AntiBTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==1)","Chi2_Central_1_BTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber > 1)","Chi2_Central_2_BTag");
  mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta >= 2.4)","TopTag_Forward");
  mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta < 2.4)","TopTag_Centraln");
  
  mainClass.AddWeightError("scaleWeight_up","scaleWeight__plus");
  mainClass.AddWeightError("scaleWeight_down","scaleWeight__minus");
  mainClass.AddWeightError("weight_btag_up/weight_btag","btag__plus");
  mainClass.AddWeightError("weight_btag_down/weight_btag","btag__minus");
  mainClass.AddWeightError("weight_pu_up/weight_pu","pu__plus");
  mainClass.AddWeightError("weight_pu_down/weight_pu","pu__minus");
  mainClass.AddWeightError("weight_sfmu_mediumID_up/weight_sfmu_mediumID","SFmu__plus");
  mainClass.AddWeightError("weight_sfmu_mediumID_down/weight_sfmu_mediumID","SFmu__minus");
  mainClass.AddWeightError("weight_toptag_up","TopTag__plus");
  mainClass.AddWeightError("weight_toptag_down","TopTag__minus");
  mainClass.AddWeightError("pdfWeight","PDF",TreeDrawMain::error_method::rms);

  mainClass.create_file(resultfile);

  gApplication->Terminate();
  return 0;
}

int main(int argc, char **argv){
  if(argc == 1)
    RootFileCreator();
  else if(argc == 2)
    RootFileCreator(argv[1]);
  else if(argc == 3)
    RootFileCreator(argv[1],argv[2]);
  else if(argc == 4)
    RootFileCreator(argv[1],argv[2],argv[3]);
}

