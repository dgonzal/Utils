#include "cutFlow.h"

#include "TFile.h"

#include <iostream>


using namespace std;

int main(){
  int i = 1;
  string cmssw_version = "CMSSW_8_0_8";

  cutFlow PreSelectionCutFlow("cutflow_v"+to_string(i)+".txt");     
  PreSelectionCutFlow.set_separator(";");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTbar_ext.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_LH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_LH.root");
 
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_LH.root");

  //PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_1500.root");
  //PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2000.root");
  //PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2500.root");
  /*
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_LH_25ns.root");
  */
  PreSelectionCutFlow.normToCut(0);
  PreSelectionCutFlow.setPrecision(4);
  PreSelectionCutFlow.printToFile("cutflow/muonEffis");

  PreSelectionCutFlow.clear_filedirs();
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTbar.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_LH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_LH.root");
 
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_LH.root");

  //PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_1500.root");
  //PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2000.root");
  //PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Zprime_2500.root");


  PreSelectionCutFlow.printToFile("cutflow/topHad");
  PreSelectionCutFlow.printToFile("cutflow/topLep"); 



  return 0;
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

  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-800_RH_25ns.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-1500_RH_25ns.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-800_LH_25ns.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-1500_LH_25ns.root");
 
 /*
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1000_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1000_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_LH.root");
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

    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-800_RH_25ns.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-1500_RH_25ns.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-800_LH_25ns.root");
    ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt-1500_LH_25ns.root");
    
    /*
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_RH_25ns.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_RH_25ns.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_LH_25ns.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_LH_25ns.root");
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
