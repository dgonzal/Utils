#include "simplePlots.h"


using namespace std;

int main(){

string cms_string = "CMSSW_7_6_3";
  string sel_version ="47";

  simplePlots TopTagForwardSplit("plots/TopTagForwardCentralSplit.ps");
   TopTagForwardSplit.normToArea(true,0.2);
  TopTagForwardSplit.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  TopTagForwardSplit.loadHists("TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.loadHists("TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp");
  TopTagForwardSplit.setLegend(0.3,0.7,0.8,.9);
  TopTagForwardSplit.addLegendEntry("Data Central");
  TopTagForwardSplit.addLegendEntry("Data Forward");
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
  TopTagForwardSplit.plotHists(2,false);
  TopTagForwardSplit.clearHists();
  TopTagForwardSplit.clearLegend();

  simplePlots controlForward("plots/Chi2ForwardCentralSplit.ps");
  controlForward.normToArea(true,0.2);
  controlForward.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root");
  controlForward.loadHists("Chi2_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.setLegend(0.3,0.7,0.8,.9);
  controlForward.addLegendEntry("Data Central");
  controlForward.addLegendEntry("Data Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Data AntiBTag Central");
  controlForward.addLegendEntry("Data AntiBTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Data 1 BTag Central");
  controlForward.addLegendEntry("Data 1 BTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");  
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Data 2 BTag Central");
  controlForward.addLegendEntry("Data 2 BTag Forward");
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
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("ttbar AntiBTag Central");
  controlForward.addLegendEntry("ttbar AntiBTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("ttbar 1 BTag Central");
  controlForward.addLegendEntry("ttbar 1 BTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");  
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("ttbar 2 BTag Central");
  controlForward.addLegendEntry("ttbar 2 BTag Forward");
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
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("W+Jets AntiBTag Central");
  controlForward.addLegendEntry("W+Jets AntiBTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("W+Jets 1 BTag Central");
  controlForward.addLegendEntry("W+Jets 1 BTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("W+Jets 2 BTag Central");
  controlForward.addLegendEntry("W+Jets 2 BTag Forward");
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
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("Single Tops AntiBTag Central");
  controlForward.addLegendEntry("Single Tops AntiBTag Forward");
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
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("B 1500 RH AntiBTag Central");
  controlForward.addLegendEntry("B 1500 RH AntiBTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("B 1500 RH 1 BTag Central");
  controlForward.addLegendEntry("B 1500 RH 1 BTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.loadHists("Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp");
  controlForward.loadHists("Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp");
  controlForward.addLegendEntry("B 1500 RH 2 BTag Central");
  controlForward.addLegendEntry("B 1500 RH 2 BTag Forward");
  controlForward.plotHists(2,false);
  controlForward.clearHists();
  controlForward.clearLegend();
  controlForward.clearFiles();



  return 0;
}
