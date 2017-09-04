#include "effiPlots.h"
#include <iostream>


using namespace std;

int main(){


  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/Trigger_v5/";
  bool single = true;
  string appendix ="/";
  if(!single) appendix=".ps";
  
  effiPlots MuonTrigger_Bpb_TW_800_LH("plots/MuonTrigger_BprimeB-800_LH.ps");
  MuonTrigger_Bpb_TW_800_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  //MuonTrigger_Bpb_TW_800_LH.imposeHist();
  MuonTrigger_Bpb_TW_800_LH.switch_legend();
  //MuonTrigger_Bpb_TW_800_LH.set_debug();
  MuonTrigger_Bpb_TW_800_LH.set_Rebin(2);
  //MuonTrigger_Bpb_TW_800_LH.set_axisTitle("muon p_{T}","Efficency");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu50_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_IsoMu24_muon_trigger_MuonHists/pt_1","IsoMu24");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(4);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu50_2DIso_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_IsoMu24_Iso_muon_trigger_MuonHists/pt_1","IsoMu24");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_req_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(4);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu50_Sel_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_IsoMu24_Sel_muon_trigger_MuonHists/pt_1","IsoMu24");
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_Sel_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(4);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu50_Sel_2DCut_muon_trigger_MuonHists/pt_1","Mu50 2D-iso");
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_IsoMu24_Sel_Iso_muon_trigger_MuonHists/pt_1","IsoMu24 iso");
  //MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_Sel_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(4);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
  MuonTrigger_Bpb_TW_800_LH.set_axisTitle("leading ak4 p_{T}");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_Mu50_muon_trigger_JetHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_IsoMu24_muon_trigger_JetHists/pt_1","IsoMu24");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_muon_trigger_JetHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(4);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();


  //MuonTrigger_Bpb_TW_800_LH.setRange(15,400);


  effiPlots EleTrigger_Bpb_TW_800_LH("plots/EleTrigger_BprimeB-800_LH.ps");
  EleTrigger_Bpb_TW_800_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root");
  //EleTrigger_Bpb_TW_800_LH.imposeHist();
  EleTrigger_Bpb_TW_800_LH.switch_legend();
  //EleTrigger_Bpb_TW_800_LH.set_debug();
  EleTrigger_Bpb_TW_800_LH.set_Rebin(2);
  EleTrigger_Bpb_TW_800_LH.set_axisTitle("electron p_{T}");
  EleTrigger_Bpb_TW_800_LH.loadHists("Sel_ele_trigger_ElectronHists/pt_1","HLT_Ele45_PFJet200_PFJet50_Sel_ele_trigger_ElectronHists/pt_1","Ele45 jet cross");
  EleTrigger_Bpb_TW_800_LH.loadHists("Sel_ele_trigger_ElectronHists/pt_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_Sel_ele_trigger_ElectronHists/pt_1","Ele105");
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);
  EleTrigger_Bpb_TW_800_LH.clear_histos();
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele45_PFJet200_PFJet50_ele_trigger_ElectronHists/pt_1","Ele45 jet cross");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_ElectronHists/pt_1","Ele105");
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);
  EleTrigger_Bpb_TW_800_LH.clear_histos();
  EleTrigger_Bpb_TW_800_LH.set_axisTitle("#Delta R_{min}(e, jet)");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_ElectronHists/deltaRmin_1","HLT_Ele45_PFJet200_PFJet50_ele_trigger_ElectronHists/deltaRmin_1","Ele45 jet cross");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_ElectronHists/deltaRmin_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_ElectronHists/deltaRmin_1","Ele105");
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);
  EleTrigger_Bpb_TW_800_LH.clear_histos();
  EleTrigger_Bpb_TW_800_LH.set_axisTitle("electron #eta");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_ElectronHists/eta_1","HLT_Ele45_PFJet200_PFJet50_ele_trigger_ElectronHists/eta_1","Ele45 jet cross");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_ElectronHists/eta_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_ElectronHists/eta_1","Ele105");
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);
  EleTrigger_Bpb_TW_800_LH.clear_histos();
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_JetHists/eta_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_JetHists/eta_1","Ele105");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_JetHists/eta_1","HLT_Ele45_PFJet200_PFJet50_ele_trigger_JetHists/eta_1","Ele45 jet cross");
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);
  EleTrigger_Bpb_TW_800_LH.clear_histos();
  EleTrigger_Bpb_TW_800_LH.set_axisTitle("leading ak4 p_{T}");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_JetHists/pt_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_JetHists/pt_1","Ele105");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_JetHists/pt_1","HLT_Ele45_PFJet200_PFJet50_ele_trigger_JetHists/pt_1","Ele45 jet cross");
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);
  EleTrigger_Bpb_TW_800_LH.clear_histos();
  EleTrigger_Bpb_TW_800_LH.set_axisTitle("number of ak4");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_JetHists/number","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_JetHists/number","Ele105");
  EleTrigger_Bpb_TW_800_LH.loadHists("ele_trigger_JetHists/number","HLT_Ele45_PFJet200_PFJet50_ele_trigger_JetHists/number","Ele45 jet cross");
  //EleTrigger_Bpb_TW_800_LH.setRange(15,400);
  EleTrigger_Bpb_TW_800_LH.plotEffi(2);


  effiPlots MuonTrigger_Bpb_LH("plots/MuonTrigger_LH"+appendix,single);
  //addFile(string filedir, string hist_draw_option ="",int color = -1, int marker =-1 ,bool stack = false, string nickname = "", double uncer = 0, double scalefactor=1);
  MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root","",-1,-1 ,false," M(800)", 0,1);
  //MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1000_LH.root","",-1,-1 ,false," M(1000)", 0,1);
  MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1200_LH.root","",-1,-1 ,false," M(1200)", 0,1);
  //MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1400_LH.root","",-1,-1 ,false," M(1400)", 0,1);
  MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1600_LH.root","",-1,-1 ,false," M(1600)", 0,1);
  MuonTrigger_Bpb_LH.set_axisTitle("muon p_{T}","Efficency");
  MuonTrigger_Bpb_LH.imposeHist();
  MuonTrigger_Bpb_LH.switch_legend();
  //MuonTrigger_Bpb_LH.set_debug();
  MuonTrigger_Bpb_LH.set_Rebin(2);
  MuonTrigger_Bpb_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_IsoMu24_muon_trigger_MuonHists/pt_1","IsoMu24");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();

  MuonTrigger_Bpb_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu50_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  
  MuonTrigger_Bpb_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  MuonTrigger_Bpb_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu50_2DIso_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  MuonTrigger_Bpb_LH.loadHists("2D_Selection_muon_trigger_MuonHists/pt_1","HLT_Mu50_2DIso_muon_trigger_MuonHists/pt_1","Mu50 2D");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();

  MuonTrigger_Bpb_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_IsoMu24_Iso_muon_trigger_MuonHists/pt_1","IsoMu24");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  MuonTrigger_Bpb_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_req_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  

 

  effiPlots EleTrigger_Bpb_LH("plots/EleTrigger_LH"+appendix,single);
  //addFile(string filedir, string hist_draw_option ="",int color = -1, int marker =-1 ,bool stack = false, string nickname = "", double uncer = 0, double scalefactor=1);
  EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root","",-1,-1 ,false," M(800)", 0,1);
  //EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1000_LH.root","",-1,-1 ,false," M(1000)", 0,1);
  EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1200_LH.root","",-1,-1 ,false," M(1200)", 0,1);
  //EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1400_LH.root","",-1,-1 ,false," M(1400)", 0,1);
  EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1600_LH.root","",-1,-1 ,false," M(1600)", 0,1);
  EleTrigger_Bpb_LH.set_axisTitle("electron p_{T}","Efficency");
  EleTrigger_Bpb_LH.imposeHist();
  EleTrigger_Bpb_LH.switch_legend();
  //EleTrigger_Bpb_LH.set_debug();
  EleTrigger_Bpb_LH.set_Rebin(2);
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_ele_trigger_ElectronHists/pt_1","Ele105");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele115_CaloIdVT_GsfTrkIdT_ele_trigger_ElectronHists/pt_1","Ele115");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_ele_trigger_ElectronHists/pt_1","Ele50_PF165");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele27_eta2p1_ele_trigger_ElectronHists/pt_1","IsoEle27");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  /*/
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele45_PFJet200_PFJet50_ele_trigger_ElectronHists/pt_1","Ele45 jet cross");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  /*/
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele105_CaloIdVT_GsfTrkIdT_2DIso_ele_trigger_ElectronHists/pt_1","Ele105");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_2DIso_ak4_ele_trigger_ElectronHists/pt_1","Ele50_PF165");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  //EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_2DIso_ak8_ele_trigger_ElectronHists/pt_1","Ele50_PF165");
  //EleTrigger_Bpb_LH.plotEffi(4);
  //EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele115_CaloIdVT_GsfTrkIdT_2DIso_ele_trigger_ElectronHists/pt_1","Ele115");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele27_eta2p1_Iso_ele_trigger_ElectronHists/pt_1","IsoEle27");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();

  EleTrigger_Bpb_LH.loadHists("2D_Selection_ele_trigger_ElectronHists/pt_1","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_2DIso_ak4_ele_trigger_ElectronHists/pt_1","Ele50 PF165 2D");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("2D_Selection_ele_trigger_ElectronHists/pt_1","HLT_Ele115_CaloIdVT_GsfTrkIdT_2DIso_ele_trigger_ElectronHists/pt_1","Ele115 2D");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  




  
  /*/
  EleTrigger_Bpb_LH.loadHists("ele_trigger_ElectronHists/pt_1","HLT_Ele45_PFJet200_PFJet50_req_ele_trigger_ElectronHists/pt_1","Ele45 jet cross");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  /*/
  EleTrigger_Bpb_LH.set_axisTitle("ak4 p_{T}","Efficency");
  EleTrigger_Bpb_LH.loadHists("ele_trigger_JetHists/pt_1","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_ele_trigger_JetHists/pt_1","Ele50_PF165");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("ele_trigger_JetHists/pt_1","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_2DIso_ak4_ele_trigger_JetHists/pt_1","Ele50_PF165 ak4 2D iso");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  
  return 0;
}
