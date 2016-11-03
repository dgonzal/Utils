#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


using namespace std;

int main(int argc, char* argv[]){

  if(argc!=4){
    cerr<<"The expected number of arguments is 3! Going to abort operation"<<endl;
    return 1;
  }
  string eta = argv[1];//"2.4";
  string energy = argv[2];//"150";
  string jetiso = argv[3];//"1.8";

  string version = "v42";
  string CMSSW = "7_6_3";
  string folder = "Selection_"+version;//"jecsmear_direction_up_Sel";//"Selection_"+version;
  
  
  TreeHists treehists("plots/test.ps");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root","hist",4,-1,false,"B+b M(800)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root","hist",7,-1,false,"B+t M(800)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_RH_25ns.root","hist",4,-1,false,"B+b M(1000)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1000_RH_25ns.root","hist",7,-1,false,"B+t M(1000)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets",0.1);
 
  treehists.SetTree("AnalysisTree");

  std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (Chi2Dis.forwardJetAbsEta >="+eta+"||Chi2Dis.jetiso>="+jetiso+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber==0)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (Chi2Dis.forwardJetAbsEta >="+eta+"||Chi2Dis.jetiso>="+jetiso+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber==1)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (Chi2Dis.forwardJetAbsEta >="+eta+"||Chi2Dis.jetiso>="+jetiso+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber> 1)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((Chi2Dis.forwardJetAbsEta<"+eta+"&&Chi2Dis.jetiso<"+jetiso+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber==0)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((Chi2Dis.forwardJetAbsEta<"+eta+"&&Chi2Dis.jetiso<"+jetiso+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber==1)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((Chi2Dis.forwardJetAbsEta<"+eta+"&&Chi2Dis.jetiso<"+jetiso+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber> 1)","100,400,3000","Mass B [GeV]");

  std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0&&(TopTagDis.forwardJetAbsEta>="+eta+"||TopTagDis.jetiso>="+jetiso+")&&TopTagDis.forwardJet.E()>="+energy+")","100,400,3000");
  std::vector<TH1F*> toptag_central_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0 &&((TopTagDis.forwardJetAbsEta<"+eta+"&&TopTagDis.jetiso<"+jetiso+")||TopTagDis.forwardJet.E()<"+energy+"))","100,400,3000");                            

  vector<string> sample_nick ={"Data","B+b M(800)","B+t M(800)","B+b M(1000)","B+t M(1000)","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","W+Jets"};
  vector<string> category = {"Anti-b-tag","1 b-tag","2 b-tags","" };
  vector<vector<TH1F*>> central_hists = {chi2_0btag_central_hist,chi2_1btag_central_hist,chi2_2btag_central_hist,toptag_central_hist};
  vector<vector<TH1F*>> forward_hists = {chi2_0btag_forward_hist,chi2_1btag_forward_hist,chi2_2btag_forward_hist,toptag_forward_hist};

  for(unsigned int i =0; i<central_hists.size();i++){
    string recotype = " X^2";
    if(i==3) recotype = "TopTag";
    for(unsigned int m=0;m<central_hists.at(i).size();m++){
      string histTitel = sample_nick[m]+" "+category[i]+recotype;
      cout<<histTitel<<" ratio "<<forward_hists.at(i).at(m)->GetEntries()<< " / "<<central_hists.at(i).at(m)->GetEntries()<<" = " <<forward_hists.at(i).at(m)->GetEntries()/central_hists.at(i).at(m)->GetEntries()<<endl;
    }
  }
  return 0;

}
