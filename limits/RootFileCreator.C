#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

struct TreeDrawInfo{
  TreeDrawInfo( std::string draw_command_, std::string selection_, std::string hist_name_):draw_command(draw_command_),selection(selection_),hist_name(hist_name_)
  std::string draw_command;
  std::string selection;
  std::string hist_name;
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
  return (TTree*)file->Get(treename.c_str());
}

TH1F* TreeDrawMain::make_hist(TTree* mytree, std::string variable, std::string draw_option){
  if(binning.empty()){
    mytree->Draw((variable+">>myTmpHist").c_str(),draw_option.c_str());
  }
  else{
    mytree->Draw((variable+">>myTmpHist("+binning+")").c_str(),draw_option.c_str());
  }
  return (TH1F*)gPad->GetPrimitive("myTmpHist");
}

bool TreeDrawMain::create_file(std::string fileName){
  TFile* result_file  = new TFile((fileName+".root").c_str(),"RECREATE");
  for(auto & dir : FileDir){
    for(auto & process : Sample){
      TTree* mytree = load_tree(dir+"/"+process);
      for(auto & hist : infoVec){
	TH1F* tmp_hist = make_hist(mytree,hist.draw_command,hist.selection);
	//missing some naming part
	tmp_hist->SetName(hist.hist_name+"_"+process);
	tmp_hist->Write();
	int error_counter=0;
	for(auto & error : error_weight){
	  TH1F* tmp_errorHist;
	  if(method == envelop){
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection);
	    tmp_errorHist->SetName(hist.hist_name+"_"+process+""+error_names.at(error_counter));
	    tmp_errorHist->Write();
	  }
	  else if(method == rms){
	    tmp_errorHist = make_hist(mytree,hist.draw_command,error+"*"+hist.selection+"+"+hist.selection);
	    tmp_errorHist->SetName(hist.hist_name+"_"+process+""+error_names.at(error_counter)+"__plus");
	    tmp_errorHist->Write();
	    tmp_errorHist = make_hist(mytree,hist.draw_command,hist.selection+"-"+error+"*"+hist.selection);
	    tmp_errorHist->SetName(hist.hist_name+"_"+process+""+error_names.at(error_counter)+"__minus");
	    tmp_errorHist->Write();
	  }
	  error_counter++;
	}
      }
    }
  }
  result_file->Close();
}

class TreeDrawMain{
public:
  TreeDrawMain(std::string binning_, std::string treename_){binning = binning_;treename =treename_};
  enum error_method{envelop,rms};

  void AddFileDir(std::string file){FileDir.push_back(file);}
  void AddFileDir(std::vector<std::string> dirs){ FileDir = dirs;}
  void AddSamples(std::vector<std::string> sample_){Sample =sample_;}

  void AddHistCategory(std::string draw_command_, std::string selection_, std::string hist_name_){infoVec.push_back(TreeDrawInfo(draw_command_,selection_,hist_name_));}
  void AddWeightError(std::string weight_option, std::string error_name, error_method method=envelop){error_weight.push_back(weight_option);error_methods_container.push_back(method);error_names.push_back(error_name);}

  bool create_file(std::string fileName);

private:
  TH1F* make_hist(TTree* mytree, std::string variable, std::string draw_option);

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
};

void RootFileCreator(const char * signal,const char * resultfile , const char *dirname="/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"){
  std::string directories = {dirname+"/Selection_v28_btag/",dirname+"/EleSelection_v4_btag/"};
  std::vector<std::string> samples = {"SingleTsChannel","SingleTtChannel","SingleTWAntitop","SingleTWTop","ZJets","TTJets","WJets","QCD","DATA",signal};

  TreeDrawMain mainClass("100,50,3000");
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
}
 
