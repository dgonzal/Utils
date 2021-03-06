#include "cutFlow.h"

#include "TFile.h"

#include <iostream>


using namespace std;

int main(){
  int i =1;
  string cmssw_version = "CMSSW_8_0_24_patch1";
  string folder = "MuPreSel_v2";	


  cutFlow PreSelectionCutFlow("cutflow_v"+to_string(i)+".txt");     
  PreSelectionCutFlow.set_separator("&","\\\\");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_LH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_LH.root");
 
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_LH.root");

  PreSelectionCutFlow.normToCut(0);
  PreSelectionCutFlow.setPrecision(4);
  PreSelectionCutFlow.printToFile("cutflow/muonEffis");

  //cmssw_version = "CMSSW_7_4_9";
  i=5;
  string sel_folder = "MuSel_new";//"/MuSel_v"+to_string(i);
  //BprimeB-1000_LH


  cutFlow SelectionCutFlow("sel_cutflow_v"+to_string(i)+".txt");
  SelectionCutFlow.set_separator("&","\\\\");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.Merge_Data.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.WJets*.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root");
  /*/
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root");  
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_LH.root");
  /*/
  
  SelectionCutFlow.normToCut(0);
  SelectionCutFlow.setPrecision(4);
  SelectionCutFlow.printToFile("cutflow/Tags");
  SelectionCutFlow.printToFile("cutflow/TopTagReco");
  SelectionCutFlow.printToFile("cutflow/Chi2Reco");



  i = 1;
  sel_folder =  "EleSel_new";//+to_string(i);

  cutFlow EleSelCutFlow("ele_sel_cutflow_v"+to_string(i)+".txt");
  EleSelCutFlow.set_separator("&","\\\\");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleData.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.TTbar.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");

  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root");  
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_LH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_LH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_LH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_LH.root");
  EleSelCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+sel_folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_LH.root");

  EleSelCutFlow.normToCut(0);
  EleSelCutFlow.setPrecision(4);
  EleSelCutFlow.printToFile("cutflow/Tags");
  //EleSelCutFlow.printToFile("cutflow/TopTagReco");
  //EleSelCutFlow.printToFile("cutflow/Chi2Reco");

  
  
  i=2;
  std::string elepresel = "ElePreSel_cross";

  cutFlow ElePreSelectionCutFlow("ele_cutflow_v"+to_string(i)+".txt");     
  ElePreSelectionCutFlow.set_separator("&","\\\\");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.DATA.SingleEleData.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.TTbar.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.ZJets.root");

  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_LH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_LH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/"+elepresel+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_LH.root");
    /*
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_RH.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_RH.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-800_LH.root");
      ElePreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/ElePreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb-1500_LH.root");
    */
  ElePreSelectionCutFlow.normToCut(0);
  ElePreSelectionCutFlow.setPrecision(4);
  ElePreSelectionCutFlow.printToFile("cutflow/eleEffis");
 
   
  cout<<"-------------------------"<<endl;
  cout<<"-------------------------"<<endl;
  cout<<"hello world, I'm done and will start to destroy my clases"<<endl;
  return 0;
}
