#include <iostream>

#include "TreeHists.h"

int main(){
  string version = "v31";
  TreeHists treehists("plots/Test.ps");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets",0.1);
 
  treehists.SetTree("AnalysisTree");
  treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
  treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_sfmu_mediumID_REPLACE/weight_sfmu_mediumID",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_toptag_REPLACE",TreeHists::error_method::envelop,"up,down");
  
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta >= 2.4)","50,50,3000","Mass B [GeV]");
  
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta < 2.4)","50,50,3000","Mass B [GeV]");
  
  
  //treehists.Draw("slimmedJets.m_eta","weight","100,-5,5","Ak4 jets #eta");
  treehists.Draw("slimmedJets.m_pt","weight","50,50,1500","Ak4 jet p_{T} [GeV]");
  treehists.Draw("numberofjets","weight","21,-0.5,20.5","Number of Ak4 jets");
  treehists.Draw("Sum$(slimmedJets.m_pt > 25)","weight","21,-0.5,20.5","Number of Ak4 jets test lenght");

  treehists.Draw("slimmedMuonsUSER.m_pt","weight","50,0,500","Muon p_{T} [GeV]");
  

  //to do, get muon plots, jet plots, electrons, MET, HTLep

  
  return 0;
}

// && slimmedJets.m_btag_combinedSecondaryVertex[] > 0.79
