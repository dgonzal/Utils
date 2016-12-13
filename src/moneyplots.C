#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(){
  string version = "v50";
  string CMSSW = "7_6_3";
  string folder = "Selection_"+version;
  bool electron = true;
  bool errors = true;
  string resultfile = "moneyplots.ps";
  if (electron){
    folder = "EleSelection_v9";
    resultfile = "plots/Ele_"+resultfile;
  }
  else
    resultfile = "plots/Mu_"+resultfile;


  /*
  TreeHists signalhists("plots/signals_only.ps");
  signalhists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root","hist",4,-1,false,"B+b M(800)");
  signalhists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root","hist",7,-1,false,"B+t M(800)");
  signalhists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_RH_25ns.root","hist",20,-1,false,"B+b M(1000)");
  signalhists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1000_RH_25ns.root","hist",30,-1,false,"B+t M(1000)");
  signalhists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",40,-1,false,"B+b M(1500)");
  signalhists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",320,-1,false,"B+t M(1500)");
  signalhists.SetTree("AnalysisTree");
  signalhists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1)","50,50,3000","B mass [GeV]");
  signalhists.Draw("TopTagDis.mass","weight*(TopTagDis.mass>-1)","50,50,3000","B mass [GeV]");
  */




  TreeHists treehists(resultfile);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  if(!electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  if(electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleDATA.root","PE",1,20,false,"Data");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root","hist",4,-1,false,"B+b M(800)",0,10);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root","hist",7,-1,false,"B+t M(800)",0,10);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_RH_25ns.root","hist",20,-1,false,"B+b M(1000)",0,10);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1000_RH_25ns.root","hist",30,-1,false,"B+t M(1000)",0,10);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",40,-1,false,"B+b M(1500)",0,10);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",12,-1,false,"B+t M(1500)",0,10);

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets");
  treehists.SetTree("AnalysisTree");

  if(errors){
    treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
    treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_sfmu_mediumID_REPLACE/weight_sfmu_mediumID",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_toptag_REPLACE",TreeHists::error_method::envelop,"up,down");
  
    treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_up_Sel_Mu/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_down_Sel_Mu/"});
  }
  //treehists.switch_logy(true);

  TLatex *cms_text = new TLatex(3.5, 24," 2.2 fb^{-1} (13 TeV)"); //CMS Preliminary
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetX(0.9);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);
  cms_text->SetY(.95);
  treehists.addText(cms_text);

  TLatex *zero = new TLatex(3.5, 24,"0");
  zero->SetNDC();
  zero->SetTextAlign(33);
  zero->SetX(0.05);
  zero->SetTextFont(42);
  zero->SetTextSize(0.04);
  zero->SetY(0.);  
  treehists.addText(zero);

  treehists.Draw("slimmedJets.slimmedJets.m_eta","weight","50,-5,5","ak4 #eta","Events",false);
  treehists.Draw("slimmedJets.slimmedJets.m_pt","weight","50,30,400","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_phi","weight","50,-3.14,3.14","ak4 #phi","Events",false);
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_pt","weight","50,170,800","AK8 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_eta","weight","50,-5,5","AK8 #eta","Events",false);
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_phi","weight","50,-3.14,3.14","AK8 #phi","Events",false);
  if(electron){
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta","weight","50,-2.4,2.4","electron #eta","Events",false);
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt","weight","50,105,600"," electron p_{T} [GeV]","Events");
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_phi","weight","50,-3.14,3.14"," electron #phi","Events",false);
  }
  else{
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta","weight","50,-2.4,2.4","muon #eta","Events",false);
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt","weight","50,30,400"," muon p_{T} [GeV]","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_phi","weight","50,-3.14,3.14"," muon #phi","Events",false);
  }
  treehists.Draw("slimmedMETs.m_pt","weight","50,50,800","MET [GeV]","Events");
  treehists.Draw("HT","weight","50,100,2000","S_{T} [GeV]","Events");

  treehists.Draw("Chi2Dis.forwardJet.eta()","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0)","50,-5,5","forward #eta","Events",false);
  treehists.Draw("Chi2Dis.forwardJet.eta()","weight*(TopTagDis.mass==-1&& Chi2Dis.forwardJet.pt() == 0)","6,-.5,5.5","No forward ak4");
  treehists.Draw("TopTagDis.forwardJet.eta()","weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0)","20,-5,5","forward #eta","Events",false);
  treehists.Draw("TopTagDis.forwardJet.eta()","weight*(TopTagDis.mass>-1&& TopTagDis.forwardJet.pt() == 0)","6,-.5,5.5","No forward ak4");
  
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1)","30,500,3000","B mass [GeV]");
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.mass>-1)","30,500,3000","B mass [GeV]");
   
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12)","50,120,300","top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11)","50,120,300","top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1)","50,60,120","W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1)","50,60,120","W_{lep} mass [GeV]");

  treehists.Draw("TopTagDis.topHad.M()","weight*(TopTagDis.mass>-1)","30,100,300","top_{had} mass [GeV]");
  
  
  return 0;
}
