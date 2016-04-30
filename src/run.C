#include "slicePlots.h"
#include "simplePlots.h"
#include "effiPlots.h"
#include "cutFlow.h"

#include "TFile.h"

#include <iostream>


using namespace std;

int main(){

  vector<string> qcd_samples = {"QCD","MC_QCD_Pt-1000toInf_Mu","MC_QCD_Pt-800to1000_Mu","MC_QCD_Pt-600to800_Mu","MC_QCD_Pt-470to600_Mu","MC_QCD_Pt-300to470_Mu","MC_QCD_Pt-170to300_Mu","MC_QCD_Pt-120to170_Mu","MC_QCD_Pt-80to120_Mu","MC_QCD_Pt-50to80_Mu","MC_QCD_Pt-20to30_Mu"};

  
  simplePlots genjetsCheck("plots/QCDJetsCheck.ps");
  genjetsCheck.setLegend(0.6,0.7,1.,.9);

  vector<string> histnames = {"1_muonCut_muonChannel_JetHists/number","30GeV_JetCut_muonChannel_JetHists/number","2DCut_muonChannel_JetHists/number","50GeV_METCut_muonChannel_JetHists/number"};


  string histname76 = "2DCut_muonChannel_JetHists/number";//"2DCut_muonChannel_GenJetHists/number";
  string hsitname74 = histname76;//"2DCut_muonChannel_GenJetsHists/number"

  for(auto name : qcd_samples){
    for(auto histname : histnames){
      genjetsCheck.addLegendEntry("CMSSW_7_6_3 "+name);
      genjetsCheck.addLegendEntry("CMSSW_7_4_15");
      TFile * file76 = new TFile(("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/PreSelection_74Comp/uhh2.AnalysisModuleRunner.MC."+name+".root").c_str(),"READ");
      TFile * file74 = new TFile(("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_15_patch1/src/UHH2/VLQToTopAndLepton/config/PreSelection_v25/uhh2.AnalysisModuleRunner.MC."+name+".root").c_str(),"READ"); 

      TH1F* hist76 = (TH1F*)file76->Get(histname.c_str());
      TH1F* hist74 = (TH1F*)file74->Get(histname.c_str());
      hist76->SetTitle(histname.c_str());
      hist74->SetTitle(histname.c_str());
      

      genjetsCheck.loadHists(hist76);
      genjetsCheck.loadHists(hist74);
      genjetsCheck.normToArea(false);
      genjetsCheck.plotHists(2,false);
      genjetsCheck.clearHists();
     
      if(hist76->GetEntries()==0 || hist74->GetEntries()==0)
	continue;
      genjetsCheck.loadHists(hist76);
      genjetsCheck.loadHists(hist74);
      genjetsCheck.normToArea(true,1);
      genjetsCheck.plotHists(2,false);
      genjetsCheck.clearHists();
      /*
	genjetsCheck.loadHists((TH1F*)file76->Get("muonChannel_GenJetHists/pt_genjet"));
	genjetsCheck.loadHists((TH1F*)file74->Get("muonChannel_GenJetsHists/pt_genjet"));
	//genjetsCheck.normToArea(true,1);
	genjetsCheck.plotHists(2,false);
	genjetsCheck.clearHists();
      */
      genjetsCheck.clearLegend();
      genjetsCheck.clearFiles();
    }
  }
  return 0;


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

  string cms_string = "CMSSW_7_6_3";
  string sel_version ="4";

  simplePlots TopTagForwardSplit("plots/TopTagForwardCentralSplit.ps");
  TopTagForwardSplit.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  TopTagForwardSplit.loadHists("TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.loadHists("TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.setLegend(0.3,0.7,0.8,.9);
  TopTagForwardSplit.addLegendEntry("Data Central");
  TopTagForwardSplit.addLegendEntry("Data Forward");
  TopTagForwardSplit.normToArea(true,0.4);
  TopTagForwardSplit.plotHists(2,false);
  TopTagForwardSplit.clearHists();
  TopTagForwardSplit.clearLegend();
  TopTagForwardSplit.clearFiles();
  TopTagForwardSplit.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  TopTagForwardSplit.loadHists("TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.loadHists("TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.setLegend(0.3,0.7,0.8,.9);
  TopTagForwardSplit.addLegendEntry("ttbar Central");
  TopTagForwardSplit.addLegendEntry("ttbar Forward");
  TopTagForwardSplit.normToArea(true,0.4);
  TopTagForwardSplit.plotHists(2,false);
  TopTagForwardSplit.clearHists();
  TopTagForwardSplit.clearLegend();
  TopTagForwardSplit.clearFiles();
  TopTagForwardSplit.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.WJets.root");
  TopTagForwardSplit.loadHists("TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.loadHists("TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp");
  //TopTagForwardSplit.setLegend(0.4,0.7,0.6,0.8);
  TopTagForwardSplit.addLegendEntry("W+Jets Central");
  TopTagForwardSplit.addLegendEntry("W+Jets Forward");
  TopTagForwardSplit.normToArea(true,0.4);
  TopTagForwardSplit.plotHists(2,false);
  TopTagForwardSplit.clearHists();
  TopTagForwardSplit.clearLegend();
  TopTagForwardSplit.clearFiles();
  TopTagForwardSplit.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  TopTagForwardSplit.loadHists("TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.loadHists("TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp");
  //TopTagForwardSplit.setLegend(0.4,0.7,0.6,0.8);
  TopTagForwardSplit.addLegendEntry("Single Top Central");
  TopTagForwardSplit.addLegendEntry("Single Top Forward");
  TopTagForwardSplit.normToArea(true,0.4);
  TopTagForwardSplit.plotHists(2,false);
  TopTagForwardSplit.clearHists();
  TopTagForwardSplit.clearLegend();
  TopTagForwardSplit.clearFiles();

  TopTagForwardSplit.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  TopTagForwardSplit.loadHists("TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.loadHists("TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp");
  //TopTagForwardSplit.setLegend(0.4,0.7,0.6,0.8);
  TopTagForwardSplit.addLegendEntry("B 1500 RH Central");
  TopTagForwardSplit.addLegendEntry("B 1500 RH Forward");
  TopTagForwardSplit.normToArea(true,0.4);
  TopTagForwardSplit.plotHists(2,false);
  TopTagForwardSplit.clearHists();
  TopTagForwardSplit.clearLegend();

  simplePlots controlForward("plots/Chi2ForwardCentralSplit.ps");
  controlForward.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  controlForward.loadHists("Chi2_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.setLegend(0.3,0.7,0.8,.9);
  controlForward.addLegendEntry("Data Central");
  controlForward.addLegendEntry("Data Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Data AntiBTag Central");
  controlForward.addLegendEntry("Data AntiBTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Data 1 BTag Central");
  controlForward.addLegendEntry("Data 1 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");  
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Data 2 BTag Central");
  controlForward.addLegendEntry("Data 2 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.clearFiles();

  controlForward.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  controlForward.loadHists("Chi2_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.setLegend(0.3,0.7,0.8,.9);
  controlForward.addLegendEntry("ttbar Central");
  controlForward.addLegendEntry("ttbar Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("ttbar AntiBTag Central");
  controlForward.addLegendEntry("ttbar AntiBTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("ttbar 1 BTag Central");
  controlForward.addLegendEntry("ttbar 1 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");  
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("ttbar 2 BTag Central");
  controlForward.addLegendEntry("ttbar 2 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.clearFiles();

  controlForward.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.WJets.root");
  controlForward.loadHists("Chi2_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_Central_Chi2_BprimeHypHists/mass_hyp");
  //controlForward.setLegend(0.4,0.7,0.6,0.8);
  controlForward.addLegendEntry("W+Jets Central");
  controlForward.addLegendEntry("W+Jets Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("W+Jets AntiBTag Central");
  controlForward.addLegendEntry("W+Jets AntiBTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("W+Jets 1 BTag Central");
  controlForward.addLegendEntry("W+Jets 1 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("W+Jets 2 BTag Central");
  controlForward.addLegendEntry("W+Jets 2 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.clearFiles();
  
  controlForward.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.SingleT.root");
  controlForward.loadHists("Chi2_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_Forward_Chi2_BprimeHypHists/mass_hyp");
  //controlForward.setLegend(0.4,0.7,0.6,0.8);
  controlForward.addLegendEntry("Single Top Central");
  controlForward.addLegendEntry("Single Top Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Single Tops AntiBTag Central");
  controlForward.addLegendEntry("Single Tops AntiBTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Single Tops 1 BTag Central");
  controlForward.addLegendEntry("Single Tops 1 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Single Tops 2 BTag Central");
  controlForward.addLegendEntry("Single Tops 2 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.clearFiles();
 
  controlForward.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  controlForward.loadHists("Chi2_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_Forward_Chi2_BprimeHypHists/mass_hyp");
  //controlForward.setLegend(0.4,0.7,0.6,0.8);
  controlForward.addLegendEntry("B 1500 RH Central");
  controlForward.addLegendEntry("B 1500 RH Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("B 1500 RH AntiBTag Central");
  controlForward.addLegendEntry("B 1500 RH AntiBTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("B 1500 RH 1 BTag Central");
  controlForward.addLegendEntry("B 1500 RH 1 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("B 1500 RH 2 BTag Central");
  controlForward.addLegendEntry("B 1500 RH 2 BTag Forward");
  controlForward.normToArea(true,0.4);
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.clearFiles();

  simplePlots control2tightBTags("plots/2tightBTags.ps");
  control2tightBTags.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  control2tightBTags.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  control2tightBTags.loadHists("Chi2_BprimeHypHists/mass_hyp");
  //control2tightBTags.loadHists("Chi2_1_BTag_Chi2_BprimeHypHists/mass_hyp");
  //control2tightBTags.loadHists("Chi2_2_BTags_Chi2_BprimeHypHists/mass_hyp");
  //control2tightBTags.loadHists("Chi2_2_thighBTag_Chi2_BprimeHypHists/mass_hyp");
  control2tightBTags.normToArea(true,0.4);
  control2tightBTags.plotHists(0,false);
  
  TFile* data = new TFile(("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root").c_str());
  TH1F * hist1 = (TH1F*) data->Get("TTbarHists/mass_topLep");
  TH1F * hist2 = (TH1F*) data->Get("TTbarHists/mass_topHad");
  hist2->Add(hist1);
  hist2->SetMarkerStyle(20);

  TFile* ttbar = new TFile(("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root").c_str());
  TH1F * hist3 = (TH1F*) ttbar->Get("TTbarHists/mass_topLep");
  TH1F * hist4 = (TH1F*) ttbar->Get("TTbarHists/mass_topHad");
  hist4->Add(hist3);
  hist4->SetFillColor(2);
  hist4->SetFillStyle(1001);

  simplePlots matchingPlots("plots/matching.ps");
  matchingPlots.loadHists(hist4);
  matchingPlots.loadHists(hist2);
  matchingPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root");
  //matchingPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  //matchingPlots.loadHists("TTbarHists/mass_topLep");
  matchingPlots.loadStackHists("TTbarHists/mass_matched_tops",5);
  matchingPlots.loadStackHists("TTbarHists/mass_semi_matched_tops",4);
  //matchingPlots.loadStackHists(hist4);
  //matchingPlots.loadHists("TTbarHists/mass_matched_tops");
  matchingPlots.plotHists(2,false);
  /*
  simplePlots resolutionPlots("plots/MassResolution.ps");
  //resolutionPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  resolutionPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  resolutionPlots.normToArea(true,0.25);
  
  resolutionPlots.loadHists("Chi2_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.loadHists("TopTagReco_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  
  resolutionPlots.loadHists("Chi2_TopTag_Chi2_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.loadHists("TopTagReco_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  */
  /*
  resolutionPlots.loadHists("Chi2_BTag_Chi2_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.loadHists("BTagReco_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  
  resolutionPlots.loadHists("Chi2_TopTag_Chi2_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.loadHists("TopTagReco_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();

  resolutionPlots.loadHists("Chi2_HEPTopTag_Chi2_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.loadHists("HEPReco_BprimeHypHists/Bprime_res_deltaR");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  */
  
  int i = 5;
  string cmssw_version = "CMSSW_7_6_3";
 
  cutFlow PreSelectionCutFlow("cutflow_v"+to_string(i)+".txt");     
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
 
  /*
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  PreSelectionCutFlow.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cmssw_version+"/src/UHH2/VLQToTopAndLepton/config/PreSelection_v"+to_string(i)+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_LH_25ns.root");
  */
  PreSelectionCutFlow.normToCut(0);
  PreSelectionCutFlow.setPrecision(4);
  PreSelectionCutFlow.printToFile("cutflow/muonEffis");
  //PreSelectionCutFlow.printToFile("cutflow/topHad");
  //PreSelectionCutFlow.printToFile("cutflow/topLep");
 
  //cmssw_version = "CMSSW_7_4_9";
  i=4;
 
  cutFlow SelectionCutFlow("sel_cutflow_v"+to_string(i)+".txt");
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
  
  
  i=-1;
  
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

