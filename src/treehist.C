#include <iostream>

#include "TreeHists.h"

int main(){
  string version = "v30";
  TreeHists testTree("plots/Test.ps");
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}");
  testTree.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_"+version+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets");
 
  testTree.SetTree("AnalysisTree");
  testTree.AddErrorWeight("pdfWeight*(pdfWeight!=-1)",TreeHists::error_method::rms);
  testTree.AddErrorWeight("scaleWeight_REPLACE*(scaleWeight_up !=-1 && scaleWeight_down != -1)",TreeHists::error_method::envelop,"up,down");
  testTree.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000");
  testTree.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000");
  testTree.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000");
  return 0;
}

// && slimmedJets.m_btag_combinedSecondaryVertex[] > 0.79
