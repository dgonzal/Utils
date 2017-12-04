#include "effiPlots.h"
#include <iostream>


using namespace std;

int main(){


  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/TriggerSel_v2/";
  bool single = false;
  string appendix ="";
  if(!single) appendix=".ps";
  
  effiPlots MuonTrigger_Bpb_LH("plots/MuonTrigger"+appendix,single);
  /*/ 
  //addFile(string filedir, string hist_draw_option ="",int color = -1, int marker =-1 ,bool stack = false, string nickname = "", double uncer = 0, double scalefactor=1);
  MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root","",-1,-1 ,false," M(800)", 0,1);
  //MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1000_LH.root","",-1,-1 ,false," M(1000)", 0,1);
  MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1200_LH.root","",-1,-1 ,false," M(1200)", 0,1);
  //MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1400_LH.root","",-1,-1 ,false," M(1400)", 0,1);
  MuonTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1600_LH.root","",-1,-1 ,false," M(1600)", 0,1);
  /*/
  MuonTrigger_Bpb_LH.addFile(dir+"Data.Mu.root","",-1,-1 ,false,"DATA", 0,1);


  //MuonTrigger_Bpb_LH.set_Rebin(2);
  MuonTrigger_Bpb_LH.set_axisTitle("muon p_{T}","Efficency");
  //MuonTrigger_Bpb_LH.imposeHist();
  MuonTrigger_Bpb_LH.switch_legend();
  //MuonTrigger_Bpb_LH.set_debug();
  MuonTrigger_Bpb_LH.set_Rebin(2);
  
  MuonTrigger_Bpb_LH.loadHists("data_trigger_MuonHists/pt_1","HLT_Mu50_data_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();  
  MuonTrigger_Bpb_LH.loadHists("data_trigger_MuonHists/pt_1","PFMETNoMu120_MET_data_trigger_MuonHists/pt_1","METNoMu 120 with MET 250");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  MuonTrigger_Bpb_LH.loadHists("data_trigger_MuonHists/pt_1","MuCombi_data_trigger_MuonHists/pt_1","Combi Mu50 & METNoMu");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  MuonTrigger_Bpb_LH.loadHists("MuCombi_data_trigger_MuonHists/pt_1","PFMETNoMu120_MET_data_trigger_MuonHists/pt_1","METNoMu 120 with MET 250");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
  MuonTrigger_Bpb_LH.loadHists("MuCombi_data_trigger_MuonHists/pt_1","HLT_Mu50_data_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();

  MuonTrigger_Bpb_LH.set_Rebin(4);
  MuonTrigger_Bpb_LH.setRange(100,650,0,1.05);
  //MuonTrigger_Bpb_LH.set_Rebin({96,80./15*28,80./15*37,80./15*47,304,352,800});
  MuonTrigger_Bpb_LH.loadHists("data_trigger_MuonHists/pt_1","PFMETNoMu120_MET_data_trigger_MuonHists/pt_1","METNoMu 120");
  MuonTrigger_Bpb_LH.loadHists("data_trigger_MuonHists/pt_1","MuCombi_data_trigger_MuonHists/pt_1","Combi Mu50 & METNoMu");
  MuonTrigger_Bpb_LH.loadHists("data_trigger_MuonHists/pt_1","HLT_Mu50_data_trigger_MuonHists/pt_1","Mu50");
  MuonTrigger_Bpb_LH.plotEffi(4);
  MuonTrigger_Bpb_LH.clear_histos();
 
  return 0;
 
  effiPlots EleTrigger_Bpb_LH("plots/EleTrigger"+appendix,single);
  
  //addFile(string filedir, string hist_draw_option ="",int color = -1, int marker =-1 ,bool stack = false, string nickname = "", double uncer = 0, double scalefactor=1);
  
  EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-800_LH.root","",-1,-1 ,false," M(800)", 0,1);
  EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1200_LH.root","",-1,-1 ,false," M(1200)", 0,1);
  EleTrigger_Bpb_LH.addFile(dir+"uhh2.AnalysisModuleRunner.MC.BprimeB-1600_LH.root","",-1,-1 ,false," M(1600)", 0,1);
  
  //EleTrigger_Bpb_LH.addFile(dir+"Data.Ele.root","",-1,-1 ,false,"DATA", 0,1);
  
  EleTrigger_Bpb_LH.set_axisTitle("electron p_{T}","Efficency");
  //EleTrigger_Bpb_LH.imposeHist();
  EleTrigger_Bpb_LH.switch_legend();
  EleTrigger_Bpb_LH.set_debug();
  //EleTrigger_Bpb_LH.set_Rebin(4);
  EleTrigger_Bpb_LH.loadHists("data_trigger_ElectronHists/pt_1","EleCombi_Sel_data_trigger_ElectronHists/pt_1","EleCombi");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("data_trigger_ElectronHists/pt_1","HT800_ST_data_trigger_ElectronHists/pt_1","EcalHT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("data_trigger_ElectronHists/pt_1","EleCombiHT800_Sel_data_trigger_ElectronHists/pt_1","EleCombi+EcalHT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("EleCombiHT800_Sel_data_trigger_ElectronHists/pt_1","HT800_ST_data_trigger_ElectronHists/pt_1","EleCombi+EcalHT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("EleCombiHT800_Sel_data_trigger_ElectronHists/pt_1","EleCombi_Sel_data_trigger_ElectronHists/pt_1","EleCombi+EcalHT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("EleCombi_Sel_data_trigger_ElectronHists/pt_1","HT800+EleCombi_data_trigger_ElectronHists/pt_1","EleCombi+HT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("HLT_HT800_data_trigger_ElectronHists/pt_1","HT800+EleCombi_data_trigger_ElectronHists/pt_1","(EleCombiTrigger+HT800)/HT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("HT800_ST_data_trigger_ElectronHists/pt_1","HT800+ST+EleCombi_data_trigger_ElectronHists/pt_1","(EleCombiTrigger+HT800)/HT800  & S_{T} > 2000");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  EleTrigger_Bpb_LH.loadHists("EleCombi_Sel_data_trigger_ElectronHists/pt_1","HT800+ST+EleCombi_data_trigger_ElectronHists/pt_1","EleCombi+HT800+ST");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();

  
  EleTrigger_Bpb_LH.set_axisTitle("S_{T}","Efficency");
  EleTrigger_Bpb_LH.loadHists("data_trigger_EventHists/ST","HLT_HT800_data_trigger_EventHists/ST","EcalHT800");
  EleTrigger_Bpb_LH.plotEffi(4);
  EleTrigger_Bpb_LH.clear_histos();
  
  
  return 0;
}
