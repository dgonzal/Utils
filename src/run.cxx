//main routine call everything else from here.
//so i do not have to change my makefile and everything compiles

#include "slicePlots.h"
#include "simplePlots.h"
#include "effiPlots.h"
#include "cutFlow.h"


#include <iostream>


using namespace std;

int main(){
  /*
  effiPlots MuonTrigger_Bpb_TW_1000_LH("plots/MuonTrigger_Bpb_TW_1000_LH.ps");
  MuonTrigger_Bpb_TW_1000_LH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Trigger_v1/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_LH.root");
  //MuonTrigger_Bpb_TW_1000_LH.imposeHist();
  MuonTrigger_Bpb_TW_1000_LH.loadHists("trigger_MuonHists/pt_1","Mu45_eta2p1_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_1000_LH.loadHists("trigger_MuonHists/pt_1","HLT_Mu50_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_1000_LH.loadHists("trigger_MuonHists/pt_1","HLT_IsoMu24_eta2p1_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_1000_LH.loadHists("trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_1000_LH.setRange(15,450);
  MuonTrigger_Bpb_TW_1000_LH.plotEffi(2);
  */
  effiPlots MuonTrigger_Bpb_TW_800_LH("plots/MuonTrigger_Bpb_TW_800_LH.ps");
  MuonTrigger_Bpb_TW_800_LH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Trigger_v3/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH.root");
  //MuonTrigger_Bpb_TW_800_LH.imposeHist();
  MuonTrigger_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_Mu45_eta2p1_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_Mu50_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_IsoMu24_eta2p1_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_OR_Mu45_IsoMu24_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_LH.setRange(15,400);
  MuonTrigger_Bpb_TW_800_LH.plotEffi(2);
  //delete MuonTrigger_Bpb_TW_800_LH;
  
  effiPlots MuonTrigger_Bpb_TW_800_RH("plots/MuonTrigger_Bpb_TW_800_RH.ps");
  MuonTrigger_Bpb_TW_800_RH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Trigger_v3/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH.root");
  //MuonTrigger_Bpb_TW_800_RH.imposeHist();
  MuonTrigger_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_Mu50_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_IsoMu24_eta2p1_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_OR_Mu45_IsoMu24_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_Mu45_eta2p1_trigger_MuonHists/pt_1");
  MuonTrigger_Bpb_TW_800_RH.switch_legend();
  MuonTrigger_Bpb_TW_800_RH.setRange(15,400);
  MuonTrigger_Bpb_TW_800_RH.plotEffi(2);
  //delete MuonTrigger_Bpb_TW_800_RH;
  /*
  effiPlots MuonTriggerSel_Bpb_TW_800_LH("plots/MuonTriggerSel_Bpb_TW_800_LH.ps");
  MuonTriggerSel_Bpb_TW_800_LH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/TriggerSel_v2/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH.root");
  //MuonTriggerSel_Bpb_TW_800_LH.imposeHist();
  MuonTriggerSel_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","Mu45_eta2p1_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_Mu50_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_IsoMu24_eta2p1_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_LH.loadHists("trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_LH.setRange(15,400);
  MuonTriggerSel_Bpb_TW_800_LH.plotEffi();
  //delete MuonTriggerSel_Bpb_TW_800_LH;
  
  effiPlots MuonTriggerSel_Bpb_TW_800_RH("plots/MuonTriggerSel_Bpb_TW_800_RH.ps");
  MuonTriggerSel_Bpb_TW_800_RH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/TriggerSel_v2/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH.root");
  //MuonTriggerSel_Bpb_TW_800_RH.imposeHist();
  MuonTriggerSel_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","Mu45_eta2p1_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_Mu50_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_IsoMu24_eta2p1_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_RH.loadHists("trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_trigger_MuonHists/pt_1");
  MuonTriggerSel_Bpb_TW_800_RH.setRange(15,400);
  MuonTriggerSel_Bpb_TW_800_RH.plotEffi();
  */
  /*
  effiPlots smEffi("plots/smMuon40Trigger.ps");
  smEffi.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/UHH2/VLQToTopAndLepton/config/Result/uhh2.AnalysisModuleRunner.MC.WJets.root");
  smEffi.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/UHH2/VLQToTopAndLepton/config/Result/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  smEffi.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/UHH2/VLQToTopAndLepton/config/Result/uhh2.AnalysisModuleRunner.MC.ZJets.root");
  
  smEffi.loadHists("muonChannel_MuonHists/pt_1","muonTrigger_muonChannel_MuonHists/pt_1");
  smEffi.setRange(30.,450.);
  smEffi.plotEffi();

  
  effiPlots muonEffi("plots/MuonCutEffi.ps");
  muonEffi.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/UHH2/VLQToTopAndLepton/config/Result_v2/uhh2.AnalysisModuleRunner.MC.BpJ_TW_1200.root");
  muonEffi.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/UHH2/VLQToTopAndLepton/config/Result_v2/uhh2.AnalysisModuleRunner.MC.BpJ_TW_800.root");
  muonEffi.loadHists("muonChannel_MuonHists/pt_1");
  muonEffi.setRange(30.,450.);
  muonEffi.plotEffi(1);
  */    



  simplePlots resolutionPlots("plots/MassResolution.ps");
  //resolutionPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v1/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  resolutionPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v1/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1200_RH_25ns.root");
  resolutionPlots.normToArea(true,0.25);
  
  resolutionPlots.loadHists("Chi2_BTag_Chi2_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.loadHists("BTagReco_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  
  resolutionPlots.loadHists("Chi2_TopTag_Chi2_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.loadHists("CMSReco_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();

  resolutionPlots.loadHists("Chi2_HEPTopTag_Chi2_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.loadHists("HEPReco_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();

  resolutionPlots.loadHists("Chi2_BTag_Chi2_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.loadHists("BTagReco_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  
  resolutionPlots.loadHists("Chi2_TopTag_Chi2_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.loadHists("CMSReco_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();

  resolutionPlots.loadHists("Chi2_HEPTopTag_Chi2_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.loadHists("HEPReco_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  /*
  resolutionPlots.loadHists("Chi2_BTag_Chi2_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.loadHists("BTagReco_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  
  resolutionPlots.loadHists("Chi2_TopTag_Chi2_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.loadHists("CMSReco_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();

  resolutionPlots.loadHists("Chi2_HEPTopTag_Chi2_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.loadHists("HEPReco_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  */

  
  int i = 1;

  cutFlow PreSelectionCutFlow("cutflow_v"+to_string(i)+".txt");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJetsM50toInf.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1200_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1200_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1200_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1200_LH_25ns.root");


  PreSelectionCutFlow.normToCut(0);
  PreSelectionCutFlow.setPrecision(4);
  PreSelectionCutFlow.printToFile("cutflow/muonEffis");
  PreSelectionCutFlow.printToFile("cutflow/topHad");
  PreSelectionCutFlow.printToFile("cutflow/topLep");
  /*  

  i=1;
  cutFlow SelectionCutFlow("sel_cutflow_v"+to_string(i)+".txt");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJets.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.ZJetsM50toInf.root");

  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1200_RH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_LH.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1200_LH.root");
 
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJetsHT100to200.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJetsHT200to400.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJetsHT400to600.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.WJetsHT600toInf.root");
  //SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD_HT500To1000Ext.root");
  SelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.QCD_HT1000ToInfExt.root");
  
  SelectionCutFlow.normToCut(0);
  SelectionCutFlow.setPrecision(4);
  SelectionCutFlow.printToFile("cutflow/Tags");
  SelectionCutFlow.printToFile("cutflow/Chi2Reco");
  SelectionCutFlow.printToFile("cutflow/CMSReco");
  */ 
  cout<<"-------------------------"<<endl;
  cout<<"-------------------------"<<endl;
  cout<<"hello world, I'm done and will start to destroy my clases"<<endl;
  return 0;
}

