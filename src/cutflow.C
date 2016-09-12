#include "cutFlow.h"

#include "TFile.h"

#include <iostream>


using namespace std;

int main(){
  int i = 12;
  string cmssw_version = "CMSSW_7_6_3";

  cutFlow PreSelectionCutFlow("cutflow_v"+to_string(i)+".txt");     
  PreSelectionCutFlow.set_separator(";");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_LH_25ns.root");
 
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_LH_25ns.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_1500.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2000.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2500.root");
  /*
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_LH_25ns.root");
  */
  PreSelectionCutFlow.normToCut(0);
  PreSelectionCutFlow.setPrecision(4);
  PreSelectionCutFlow.printToFile("cutflow/muonEffis");

  PreSelectionCutFlow.clear_filedirs();
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_LH_25ns.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_LH_25ns.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_1500.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2000.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2500.root");


  PreSelectionCutFlow.printToFile("cutflow/topHad");
  PreSelectionCutFlow.printToFile("cutflow/topLep"); 

  //cmssw_version = "CMSSW_7_4_9";
  i=18;
 
  cutFlow SelectionCutFlow("sel_cutflow_v"+to_string(i)+".txt");
  //SelectionCutFlow.set_separator(";");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");

  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_LH_25ns.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_LH_25ns.root");
 
 /*
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_LH.root");
  */


  SelectionCutFlow.normToCut(0);
  SelectionCutFlow.setPrecision(4);
  SelectionCutFlow.printToFile("cutflow/Tags");
  SelectionCutFlow.printToFile("cutflow/TopTagReco");
  SelectionCutFlow.printToFile("cutflow/Chi2Reco");
  
  
  i=1;
  
  if(i >0){
    cutFlow ElePreSelectionCutFlow("ele_cutflow_v"+to_string(i)+".txt");     
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.DATA.SingleEleDATA.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_LH_25ns.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_LH_25ns.root");
    
    /*
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_LH_25ns.root");
    */
    ElePreSelectionCutFlow.normToCut(0);
    ElePreSelectionCutFlow.setPrecision(4);
    ElePreSelectionCutFlow.printToFile("cutflow/eleEffis");
  }
   
  cout<<"-------------------------"<<endl;
  cout<<"-------------------------"<<endl;
  cout<<"hello world, I'm done and will start to destroy my clases"<<endl;
  return 0;
}
