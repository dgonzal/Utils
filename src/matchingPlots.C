#include "TreeHists.h"
#include "simplePlots.h"
#include "TFile.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
// Plots for matching and resolution

/*
bool create_overlayhists(std::string plotname, std::string distibution, std::string basic_weight, std::vector<std::string> variations, std::vector<std::string> legend, std::vector<std::string> files, std::string topFile = ""){
  TreeHists treehists("plots/dummy_name_unsued.ps");
  for(auto file : files)
    treehists.addFile(file,"hist",4,-1,false,"B+b M(1500)");
  if(!topFile.empty())
    treehists.addFile(topFile,"hist",4,-1,false,"B+b M(1500)");

  
  treehists.Draw(distibution,basic_weight,"test");
  std::vector<TH1F*> basic = treehists.return_hists(distibution,basic_weight,"70,30,500","test123");
  std::vector<std::vector<TH1F*>> hist_var;
  for(auto var : variations){
    std::vector<TH1F*> hist_coll = treehists.return_hists(distibution,basic_weight+"*("+var+")","70,30,500","test123");
    hist_var.push_back(hist_coll);
  }
  
  for(auto hist_coll : hist_var)
    for(unsigned int i=1; i<files.size();i++)
      hist_coll[0]->Add(hist_coll[i]);
  for(unsigned int i= 0; i < variations.size();i++)
    hist_var[i][0]->SetFillColor(i+1);


  simplePlots plot_me("plots/"+plotname);
  if(!topFile.empty()){
    plot_me.loadHists(basic[files.size()],legend[legend.size()-1]);
  }
  for(unsigned int i=0; i<variations.size(); i++){
    std::string legend_string = "";
    if(legend.size()>i)
      legend_string = legend[i];
    plot_me.loadStackHists(hist_var[i][0],legend_string);
  }
  plot_me.plotHists(2,false);
  return true;
}
*/
std::string deltaRstring(std::string var1, std::string var2){
  return "(("+var1+".Eta()-"+var2+".Eta())*("+var1+".Eta()-"+var2+".Eta())+("+var1+".Phi()-"+var2+".Phi())*("+var1+".Phi()-"+var2+".Phi()))";
}

int main(){

  string cms_string = "CMSSW_7_6_3";
  string sel_version = "31";

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
  
  simplePlots resolutionPlots("plots/MassResolution.ps");
  //resolutionPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_LH_25ns.root");
  resolutionPlots.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/"+cms_string+"/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root");
  resolutionPlots.normToArea(true,0.25);
  
  resolutionPlots.loadHists("Chi2_BprimeHypHists/Bprime_res_mass");
  //resolutionPlots.loadHists("TopTagReco_BprimeHypHists/Bprime_res_mass");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();
  
  //resolutionPlots.loadHists("Chi2_TopTag_Chi2_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.loadHists("TopTagReco_BprimeHypHists/Bprime_res_pt");
  resolutionPlots.plotHists(1,false);
  resolutionPlots.clearHists();

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
  string matching_radius = "0.16";

  TreeHists treehists("plots/matchingReco.ps");  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v"+sel_version+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets",0.1);

  treehists.SetTree("AnalysisTree");

  treehists.Draw("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12)","70,30,500","hadronic Top Mass[GeV]");
  treehists.Draw("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  || BprimeGen.topHad.Pt() == 0))","70,30,500","hadronic Top Mass[GeV]");
  treehists.Draw("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12)","70,30,500","hadronic Top Mass[GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(Chi2Dis.recoTyp == 11)","70,30,500","leptonic Top Mass[GeV]");

  vector<TH1F*> unmatched = treehists.return_hists("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> top_matched_w_unmatched = treehists.return_hists("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> top_unmatched_w_matched = treehists.return_hists("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> matched = treehists.return_hists("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> all =  treehists.return_hists("Chi2Dis.topHad.M()","weight*(Chi2Dis.recoTyp == 12)","70,30,500","hadronic Top Mass[GeV]");


  TH1F* matched_hist = matched[3];
  TH1F* unmatched_hist = unmatched[3];
  TH1F* semimatched_hist =  top_matched_w_unmatched[3];
  for(unsigned int i = 4; i< matched.size();i++){
    matched_hist->Add(matched[i]);
    unmatched_hist->Add(unmatched[i]);
    semimatched_hist->Add(top_matched_w_unmatched[i]);
  }
  for(unsigned int i = 3; i< matched.size();i++){
    semimatched_hist->Add(top_unmatched_w_matched[i]);
  }
  all[0]->SetTitle("hadronic Top Mass");
  all[0]->SetMarkerStyle(1);
  matched_hist->SetFillColor(632);
  matched_hist->SetLineColor(632);
  unmatched_hist->SetFillColor(3);
  unmatched_hist->SetLineColor(3);
  semimatched_hist->SetFillColor(634);
  semimatched_hist->SetLineColor(634);

  matched[1]->SetFillColor(2);
  matched[1]->SetLineColor(2);
  top_matched_w_unmatched[1]->SetFillColor(634);
  top_matched_w_unmatched[1]->SetLineColor(634);
  top_unmatched_w_matched[1]->SetFillColor(4);
  top_unmatched_w_matched[1]->SetLineColor(4);
  unmatched[1]->SetFillColor(3);
  unmatched[1]->SetLineColor(3);

  vector<TH1F*> unmatched_lep = treehists.return_hists("Chi2Dis.topLep.M()","weight*(Chi2Dis.recoTyp == 11 && "+deltaRstring("Chi2Dis.topLep","BprimeGen.topLep")+">"+matching_radius+" && "+deltaRstring("Chi2Dis.wHad","BprimeGen.wHad")+" >"+matching_radius+")","70,30,500","leptonic Top Mass[GeV]");
  vector<TH1F*> top_matched_w_unmatched_lep = treehists.return_hists("Chi2Dis.topLep.M()","weight*(Chi2Dis.recoTyp == 11 && (((Chi2Dis.topLep.Eta()-BprimeGen.topLep.Eta())*(Chi2Dis.topLep.Eta()-BprimeGen.topLep.Eta())+(Chi2Dis.topLep.Phi()-BprimeGen.topLep.Phi())*(Chi2Dis.topLep.Phi()-BprimeGen.topLep.Phi()))<"+matching_radius+"  && ((Chi2Dis.wHad.Eta()-BprimeGen.wHad.Eta())*(Chi2Dis.wHad.Eta()-BprimeGen.wHad.Eta())+(Chi2Dis.wHad.Phi()-BprimeGen.wHad.Phi())*(Chi2Dis.wHad.Phi()-BprimeGen.wHad.Phi()))>"+matching_radius+"))","70,30,500","leptonic Top Mass[GeV]");
  vector<TH1F*> top_unmatched_w_matched_lep = treehists.return_hists("Chi2Dis.topLep.M()","weight*(Chi2Dis.recoTyp == 11 && (((Chi2Dis.topLep.Eta()-BprimeGen.topLep.Eta())*(Chi2Dis.topLep.Eta()-BprimeGen.topLep.Eta())+(Chi2Dis.topLep.Phi()-BprimeGen.topLep.Phi())*(Chi2Dis.topLep.Phi()-BprimeGen.topLep.Phi()))>"+matching_radius+"  && ((Chi2Dis.wHad.Eta()-BprimeGen.wHad.Eta())*(Chi2Dis.wHad.Eta()-BprimeGen.wHad.Eta())+(Chi2Dis.wHad.Phi()-BprimeGen.wHad.Phi())*(Chi2Dis.wHad.Phi()-BprimeGen.wHad.Phi()))<"+matching_radius+"))","70,30,500","leptonic Top Mass[GeV]");
  vector<TH1F*> matched_lep = treehists.return_hists("Chi2Dis.topLep.M()","weight*(Chi2Dis.recoTyp == 11 && (((Chi2Dis.topLep.Eta()-BprimeGen.topLep.Eta())*(Chi2Dis.topLep.Eta()-BprimeGen.topLep.Eta())+(Chi2Dis.topLep.Phi()-BprimeGen.topLep.Phi())*(Chi2Dis.topLep.Phi()-BprimeGen.topLep.Phi()))<"+matching_radius+"  && ((Chi2Dis.wHad.Eta()-BprimeGen.wHad.Eta())*(Chi2Dis.wHad.Eta()-BprimeGen.wHad.Eta())+(Chi2Dis.wHad.Phi()-BprimeGen.wHad.Phi())*(Chi2Dis.wHad.Phi()-BprimeGen.wHad.Phi()))<"+matching_radius+"))","70,30,500","leptonic Top Mass[GeV]");
  vector<TH1F*> all_lep =  treehists.return_hists("Chi2Dis.topLep.M()","weight*(Chi2Dis.recoTyp == 11)","70,30,500","leptonic Top Mass[GeV]");

  TH1F* matched_hist_lep  = matched_lep[3];
  TH1F* unmatched_hist_lep  = unmatched_lep[3];
  TH1F* semimatched_hist_lep  =  top_matched_w_unmatched_lep[3];
  for(unsigned int i = 4; i< matched_lep.size();i++){
    matched_hist_lep->Add(matched_lep[i]);
    unmatched_hist_lep->Add(unmatched_lep[i]);
    semimatched_hist_lep->Add(top_matched_w_unmatched_lep[i]);
  }
  for(unsigned int i = 3; i< matched_lep.size();i++){
    semimatched_hist_lep->Add(top_unmatched_w_matched_lep[i]);
  }
  all_lep[0]->SetTitle("leptonic Top Mass");
  all_lep[0]->SetMarkerStyle(1);
  matched_hist_lep->SetFillColor(2);
  unmatched_hist_lep->SetFillColor(3);
  semimatched_hist_lep->SetFillColor(4);

  matched_lep[1]->SetFillColor(632);
  matched_lep[1]->SetLineColor(632);
  top_matched_w_unmatched_lep[1]->SetFillColor(634);
  top_matched_w_unmatched_lep[1]->SetLineColor(634);
  top_unmatched_w_matched_lep[1]->SetFillColor(4);
  top_unmatched_w_matched_lep[1]->SetLineColor(4);
  unmatched_lep[1]->SetFillColor(5);
  unmatched_lep[1]->SetLineColor(5);

  simplePlots test("plots/test.ps");
  all[1]->SetTitle("hadronic Top");
  all[1]->SetMarkerStyle(1);
  test.loadHists(all[1],"B+b M(1500)","HIST");
  test.loadStackHists(top_unmatched_w_matched[1]," top_{unmatch} W_{match}");
  test.loadStackHists(unmatched[1],"unmatched");
  test.loadStackHists(top_matched_w_unmatched[1]," top_{match} W_{unmatch}");
  test.loadStackHists(matched[1],"matched");
  test.plotHists(2,false);
  test.clearAll();

  all_lep[1]->SetTitle("leptonic Top");
  all_lep[1]->SetMarkerStyle(1);
  test.loadHists(all_lep[1],"B+b M(1500)","hist");
  test.loadStackHists(unmatched_lep[1],"unmatched");
  test.loadStackHists(top_unmatched_w_matched_lep[1]," top_{unmatch} W_{match}");
  test.loadStackHists(top_matched_w_unmatched_lep[1]," top_{match} W_{unmatch}");
  test.loadStackHists(matched_lep[1],"matched");
  test.plotHists(2,false);
  test.clearAll();



  vector<TH1F*> toptag_all  = treehists.return_hists("TopTagDis.topHad.M()","weight","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> toptag_unmatched  = treehists.return_hists("TopTagDis.topHad.M()","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> toptag_matched  = treehists.return_hists("TopTagDis.topHad.M()","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> toptag_wmatch_topunmatch  = treehists.return_hists("TopTagDis.topHad.M()","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");
  vector<TH1F*> toptag_wunmatch_topmatch  = treehists.return_hists("TopTagDis.topHad.M()","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","70,30,500","hadronic Top Mass[GeV]");


  
  toptag_wmatch_topunmatch[1]->SetFillColor(4);
  toptag_unmatched[1]->SetFillColor(5);
  toptag_wunmatch_topmatch[1]->SetFillColor(634);
  toptag_matched[1]->SetFillColor(632);
  toptag_wmatch_topunmatch[1]->SetLineColor(4);
  toptag_unmatched[1]->SetLineColor(5);
  toptag_wunmatch_topmatch[1]->SetLineColor(634);
  toptag_matched[1]->SetLineColor(632);

  toptag_all[1]->SetTitle("TopTag");
  toptag_all[1]->SetMarkerStyle(1);
  test.loadHists(toptag_all[1],"B+b M(1500)","HIST");
  test.loadStackHists(toptag_wmatch_topunmatch[1]," top_{unmatched} W_{matched}");
  test.loadStackHists(toptag_unmatched[1],"unmatched");
  test.loadStackHists(toptag_wunmatch_topmatch[1]," top_{matched} W_{unmatched}");
  test.loadStackHists(toptag_matched[1],"matched");
  test.plotHists(2,false);
  test.clearAll();


  vector<TH1F*> toptagbprime_all  = treehists.return_hists("TopTagDis.mass","weight","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> toptagbprime_unmatched  = treehists.return_hists("TopTagDis.mass","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> toptagbprime_matched  = treehists.return_hists("TopTagDis.mass","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> toptagbprime_wmatch_topunmatch  = treehists.return_hists("TopTagDis.mass","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> toptagbprime_wunmatch_topmatch  = treehists.return_hists("TopTagDis.mass","weight*((((TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())*(TopTagDis.topHad.Eta()-BprimeGen.topHad.Eta())+(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi())*(TopTagDis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())*(TopTagDis.wLep.Eta()-BprimeGen.wLep.Eta())+(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi())*(TopTagDis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");

  toptagbprime_wmatch_topunmatch[1]->SetFillColor(4);
  toptagbprime_unmatched[1]->SetFillColor(5);
  toptagbprime_wunmatch_topmatch[1]->SetFillColor(6);
  toptagbprime_matched[1]->SetFillColor(632);
  toptagbprime_wmatch_topunmatch[1]->SetLineColor(4);
  toptagbprime_unmatched[1]->SetLineColor(5);
  toptagbprime_wunmatch_topmatch[1]->SetLineColor(6);
  toptagbprime_matched[1]->SetLineColor(632);

  toptagbprime_all[1]->SetTitle("TopTag B");
  toptagbprime_all[1]->SetMarkerStyle(1);
  test.loadHists(toptagbprime_all[1],"B+b M(1500)","HIST");
  test.loadStackHists(toptagbprime_wmatch_topunmatch[1]," top_{unmatched} W_{matched}");
  test.loadStackHists(toptagbprime_unmatched[1],"unmatched");
  test.loadStackHists(toptagbprime_wunmatch_topmatch[1]," top_{matched} W_{unmatched}");
  test.loadStackHists(toptagbprime_matched[1],"matched");
  test.plotHists(2,false);
  test.clearAll();


  vector<TH1F*> chi2bprime_all  = treehists.return_hists("Chi2Dis.mass","weight * Chi2Dis.recoTyp == 12 ","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> chi2bprime_unmatched  = treehists.return_hists("Chi2Dis.mass","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> chi2bprime_matched  = treehists.return_hists("Chi2Dis.mass","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> chi2bprime_wmatch_topunmatch  = treehists.return_hists("Chi2Dis.mass","weight*(Chi2Dis.recoTyp == 12 && (((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))>"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))<"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");
  vector<TH1F*> chi2bprime_wunmatch_topmatch  = treehists.return_hists("Chi2Dis.mass","weight*(Chi2Dis.recoTyp == 12 &&(((Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())*(Chi2Dis.topHad.Eta()-BprimeGen.topHad.Eta())+(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi())*(Chi2Dis.topHad.Phi()-BprimeGen.topHad.Phi()))<"+matching_radius+"  && ((Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())*(Chi2Dis.wLep.Eta()-BprimeGen.wLep.Eta())+(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi())*(Chi2Dis.wLep.Phi()-BprimeGen.wLep.Phi()))>"+matching_radius+"))","100,75,3000","hadronic Top Mass[GeV]");

  chi2bprime_wmatch_topunmatch[1]->SetFillColor(4);
  chi2bprime_unmatched[1]->SetFillColor(5);
  chi2bprime_wunmatch_topmatch[1]->SetFillColor(6);
  chi2bprime_matched[1]->SetFillColor(632);
  chi2bprime_wmatch_topunmatch[1]->SetLineColor(4);
  chi2bprime_unmatched[1]->SetLineColor(5);
  chi2bprime_wunmatch_topmatch[1]->SetLineColor(6);
  chi2bprime_matched[1]->SetLineColor(632);

  chi2bprime_all[1]->SetTitle("Chi2 B");
  chi2bprime_all[1]->SetMarkerStyle(1);
  test.loadHists(chi2bprime_all[1],"B+b M(1500)","HIST");
  test.loadStackHists(chi2bprime_wmatch_topunmatch[1]," top_{unmatched} W_{matched}");
  test.loadStackHists(chi2bprime_unmatched[1],"unmatched");
  test.loadStackHists(chi2bprime_wunmatch_topmatch[1]," top_{matched} W_{unmatched}");
  test.loadStackHists(chi2bprime_matched[1],"matched");
  test.plotHists(2,false);
  test.clearAll();




  return 0;

}


