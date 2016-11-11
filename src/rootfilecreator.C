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
  void AddWeightError(std::string weight_option, std::string error_name, error_method method=envelop){error_weight.push_back(weight_option);error_methods_container.push_back(method);error_names.push_back(error_name);}
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
  TFile* file = new TFile(fileDir.c_str());
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
  cout<<"Creating file "<<fileName<<endl;  
  TFile* result_file  = new TFile(fileName_.c_str(),"RECREATE");
  for(auto & dir : FileDir){
    find_samples_nicks(dir);
    set_channel(dir);
    int nick_number = 0;
    TTree* signal_tree;
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
      //if(debug)cout<<"loading tree for "<< process<<endl;
      TTree* mytree = load_tree(dir+"/"+process);
      for(auto & hist : infoVec){
	if(debug)cout<<"making histogram "<<hist.draw_command<<" selection "<<hist.selection<<endl;
	TH1F* tmp_hist = make_hist(mytree,hist.draw_command,hist.selection,hist.binning);
	if(hist.scale >0) tmp_hist->Scale(hist.scale);
	if(!signal_name.empty() && !signal_weight.empty() && (!boost::algorithm::contains(hist.hist_name,"Bp") || !boost::algorithm::contains(hist.hist_name,"X"))){
	  TH1F* signal_injected = make_hist(signal_tree,hist.draw_command,hist.selection,hist.binning);
	  signal_injected->Scale(stod(signal_weight));
	  tmp_hist->Add(signal_injected);
	}
	//missing some naming part
	tmp_hist->SetName((hist.hist_name+channel+"__"+nick).c_str());
	tmp_hist->SetTitle("B mass (GeV)");
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
	for(auto & error : error_weight){
	  TH1F* tmp_errorHist;
	  error_method method  = error_methods_container.at(error_counter);
	  if(method == envelop){    
	    if(debug)std::cout<< error_names.at(error_counter)<<endl;
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection+"*("+error+">-1)",hist.binning);
	    if(tmp_errorHist->GetEntries() ==0 ){
	      error_counter++;
	      continue;
	    }
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)).c_str());
	    result_file->cd();
	    tmp_errorHist->Write();
	  }
	  else if(method == rms){
	    double hist_integral = tmp_hist->Integral();
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection+"*("+error+">-1)",hist.binning);
	    tmp_errorHist->Scale(1/hist_integral);
	    tmp_errorHist->Add(tmp_hist);
	    tmp_errorHist->SetName((hist.hist_name+channel+"__"+nick+"__"+error_names.at(error_counter)+"__plus").c_str());
	    if(tmp_errorHist->GetEntries() ==0 ){
	      error_counter++;
	      continue;
	    }
	    result_file->cd();
	    tmp_errorHist->Write();
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection+"*("+error+">-1)",hist.binning);
	    tmp_errorHist->Scale(1/hist_integral);
	    tmp_errorHist->Add(tmp_errorHist,tmp_hist,-1);
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
      if(debug)cout<<dir+"/"+process<<endl;
      if(debug)cout<<"nick number "<<nick_number<<" of "<< nicks.size()<<" error nick "<< error_nick <<" of "<<dir_errors_nick.size() <<endl;
      TTree* mytree = load_tree(dir+"/"+process);
      for(auto & hist : infoVec){ 
	if(debug)cout<<hist.draw_command<<" "<<hist.selection<<endl;
	if(boost::algorithm::contains(hist.hist_name,"background")) continue;
	TH1F* tmp_hist = make_hist(mytree,hist.draw_command,hist.selection,hist.binning);
	if(debug)cout<<dir_errors_nick[error_nick]<<endl;
	if(debug)cout<<(hist.hist_name+channel+"__"+nick+"__"+dir_errors_nick[error_nick]).c_str()<<endl;
	tmp_hist->SetName((hist.hist_name+channel+"__"+nick+"__"+dir_errors_nick[error_nick]).c_str());
	result_file->cd();
	tmp_hist->Write();
      } 
       nick_number++;
    }
    error_nick++;
  }
  result_file->Close();
  delete result_file;
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
      else
	nicks.push_back(splitted_match[splitted_match.size()-2]);
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
int RootFileCreator(string signal="LH_25ns.root", string resultfile="TESTME1.root", string dirnames="/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v31/", string channel = "", string signal_injection ="", string signal_weight=""){
  TH1::AddDirectory(kFALSE);
  //std::vector<std::string> directories = {dirname+"/Selection_v31/",dirname+"/EleSelection_v6_tree/"};//,dirname+"/EleSelection_v5_tree/"
  std::vector<std::string> directories; 
  boost::split(directories,dirnames,boost::is_any_of(","));

  //std::vector<std::string> samples = {"SingleTsChannel.root","SingleTtChannel.root","SingleTWAntitop.root","SingleTWTop.root","ZJets.root","TTJets.root","WJets.root","QCD.root","DATA.root",signal};
  std::vector<std::string> samples = {"DATA.root",signal};  
  cout<<"Starting histogram production"<<endl;
  
  std::cout<<"Directories"<<std::endl;
  for(auto it : directories)
    std::cout<<it<<std::endl;
  std::cout<<"Result File "<<resultfile<<std::endl;
  for(auto it : samples)
    std::cout<<it<<std::endl;
  

  TreeDrawMain mainClass("50,100,3000","AnalysisTree");
  mainClass.AddFileDir(directories);
  mainClass.AddSamples(samples);
  mainClass.inject_signal(signal_injection,signal_weight);
  
  string eta = "2.0";
  string energy = "250";
  string jetiso = "200000";
  //if(boost::algorithm::contains(channel,"Ele"))
  //  eta = 2.1;
  /*
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= "+eta+" && Chi2Dis.btagEventNumber ==0)","Chi2_Forward_AntiBTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= "+eta+" && Chi2Dis.btagEventNumber ==1)","Chi2_Forward_1_BTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= "+eta+" && Chi2Dis.btagEventNumber > 1)","Chi2_Forward_2_BTag");
  //mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && TopTagDis.forwardJetAbsEta >= "+eta+")","TopTag_Forward");
  
  
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < "+eta+" && Chi2Dis.btagEventNumber ==0)","Chi2_Central_AntiBTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < "+eta+" && Chi2Dis.btagEventNumber ==1)","Chi2_Central_1_BTag");
  mainClass.AddHistCategory("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < "+eta+" && Chi2Dis.btagEventNumber > 1)","Chi2_Central_2_BTag");
  //mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >-1 && TopTagDis.forwardJetAbsEta < "+eta+")","TopTag_Central");
  */
  


  /*
  **
  ** Data driven background
  */


  string chi2_central_string = "TopTagDis.mass==-1 &&((abs(Chi2Dis.forwardJet.eta()) <" +eta+") || Chi2Dis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+") && Chi2Dis.forwardJet.E()>="+energy;

  std::vector<double>  background_weights = {0.37, 0.43, 0.43, 0.45 };
  
  /*
  if(boost::algorithm::contains(channel,"Ele")){
    background_weights = {0.38, 0.423, 0.39, 0.54};
  }*/

  mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0)","Chi2_AntiBTag","30,500,3000");	    
  mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1)","Chi2_1_BTag"  ,"30,500,3000");	    
  mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1)","Chi2_2_BTag"  ,"30,500,3000");	    
 		    
  mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==0)","Chi2_AntiBTag:background","30,500,3000",background_weights[0]);//0.388519); 
  mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==1)","Chi2_1_BTag:background"  ,"30,500,3000",background_weights[1]);//0.471137); 
  mainClass.AddHistCategory("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber> 1)","Chi2_2_BTag:background"  ,"30,500,3000",background_weights[2]);//0.467834); 

  mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >0&&(abs(TopTagDis.forwardJet.eta())>="+eta+")&&TopTagDis.forwardJet.E()>="+energy+")","TopTag","30,500,3000");		
  mainClass.AddHistCategory("TopTagDis.mass","weight*(TopTagDis.mass >0 &&((abs(TopTagDis.forwardJet.eta())<"+eta+")||TopTagDis.forwardJet.E()<"+energy+"))","TopTag:background","30,500,3000",background_weights[3]);//0.445472);                        
  
  mainClass.AddWeightError("scaleWeight_up","scaleWeight__plus");
  mainClass.AddWeightError("scaleWeight_down","scaleWeight__minus");
  mainClass.AddWeightError("weight_btag_up/weight_btag","btag__plus");
  mainClass.AddWeightError("weight_btag_down/weight_btag","btag__minus");
  mainClass.AddWeightError("weight_pu_up/weight_pu","pu__plus");
  mainClass.AddWeightError("weight_pu_down/weight_pu","pu__minus");
  if(!boost::algorithm::contains(channel,"Ele")){
    mainClass.AddWeightError("weight_sfmu_mediumID_up/weight_sfmu_mediumID","SFmu__plus");
    mainClass.AddWeightError("weight_sfmu_mediumID_down/weight_sfmu_mediumID","SFmu__minus");
  }
  
  //mainClass.AddWeightError("weight_toptag_up","TopTag__plus");
  //mainClass.AddWeightError("weight_toptag_down","TopTag__minus");
  mainClass.AddWeightError("pdfWeight","PDF",TreeDrawMain::error_method::rms);
  
  
  vector<string> channel_dirs;
  if(channel.empty() || boost::iequals(channel,"Ele")) channel_dirs.push_back("Ele");
  if(channel.empty() || boost::iequals(channel,"Mu")) channel_dirs.push_back("Mu");
  
  string cmssw_version = "7_6_3";

  for(auto chan : channel_dirs){
    mainClass.AddDirError("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_up_Sel_"+chan+"/","jec__plus");
    mainClass.AddDirError("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_down_Sel_"+chan+"/","jec__minus");
    mainClass.AddDirError("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_up_Sel_"+chan+"/","jer__plus");
    mainClass.AddDirError("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_down_Sel_"+chan+"/","jer__minus");
  }
  
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
  else if(argc == 5)
    RootFileCreator(argv[1],argv[2],argv[3],argv[4]);
  else if(argc == 6)
    RootFileCreator(argv[1],argv[2],argv[3],argv[4],argv[5]);
  else if(argc == 7)
    RootFileCreator(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
}

