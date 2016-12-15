#include "effiPlots.h"
#include <iostream>


using namespace std;

int main(){


  effiPlots MuonTrigger_Bpb_TW_800_LH("plots/MuonTrigger_Bpb_TW_800_LH.ps");
  MuonTrigger_Bpb_TW_800_LH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Trigger_v4/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  //MuonTrigger_Bpb_TW_800_LH.imposeHist();
  MuonTrigger_Bpb_TW_800_LH.switch_legend();
  MuonTrigger_Bpb_TW_800_LH.set_debug();
  MuonTrigger_Bpb_TW_800_LH.set_Rebin(2);
  //MuonTrigger_Bpb_TW_800_LH.set_axisTitle("muon p_{T}","Efficency");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu45_eta2p1_muon_trigger_MuonHists/pt_1","Mu45_eta2p1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu50_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_IsoMu27_muon_trigger_MuonHists/pt_1","IsoMu27");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(2);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu45_eta2p1_Sel_muon_trigger_MuonHists/pt_1","Mu45_eta2p1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu50_Sel_muon_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_IsoMu27_Sel_muon_trigger_MuonHists/pt_1","IsoMu27");
  MuonTrigger_Bpb_TW_800_LH.loadHists("Sel_muon_trigger_MuonHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_Sel_muon_trigger_MuonHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(2);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
  MuonTrigger_Bpb_TW_800_LH.set_axisTitle("leading ak4 p_{T}");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_Mu45_eta2p1_muon_trigger_JetHists/pt_1","Mu45_eta2p1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_Mu50_muon_trigger_JetHists/pt_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_IsoMu27_muon_trigger_JetHists/pt_1","IsoMu27");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_JetHists/pt_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_muon_trigger_JetHists/pt_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(2);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();/*
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/deltaRmin_1","HLT_Mu45_eta2p1_muon_trigger_MuonHists/deltaRmin_1""Mu45_eta2p1");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/deltaRmin_1","HLT_Mu50_muon_trigger_MuonHists/deltaRmin_1","Mu50");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/deltaRmin_1","HLT_IsoMu27_muon_trigger_MuonHists/deltaRmin_1","IsoMu27");
  MuonTrigger_Bpb_TW_800_LH.loadHists("muon_trigger_MuonHists/deltaRmin_1","HLT_Mu40_eta2p1_PFJet200_PFJet50_muon_trigger_MuonHists/deltaRmin_1","Mu40 jet cross");
  MuonTrigger_Bpb_TW_800_LH.plotEffi(2);
  MuonTrigger_Bpb_TW_800_LH.clear_histos();
 */


  //MuonTrigger_Bpb_TW_800_LH.setRange(15,400);


  effiPlots EleTrigger_Bpb_TW_800_LH("plots/EleTrigger_Bpb_TW_800_LH.ps");
  EleTrigger_Bpb_TW_800_LH.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Trigger_v4/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  //EleTrigger_Bpb_TW_800_LH.imposeHist();
  EleTrigger_Bpb_TW_800_LH.switch_legend();
  EleTrigger_Bpb_TW_800_LH.set_debug();
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


  return 0;
}
