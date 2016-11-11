#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(){
  string version = "v47";
  string CMSSW = "7_6_3";
  string folder = "Selection_"+version;
  bool electron = false;
  if (electron)folder = "EleSelection_v8";



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




  TreeHists treehists("plots/"+folder+".ps");
  treehists.SetLegend(0.7, 0.5, 0.85, 0.85);
  if(!electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  if(electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleDATA.root","PE",1,20,false,"Data");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root","hist",4,-1,false,"B+b M(800)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root","hist",7,-1,false,"B+t M(800)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1000_RH_25ns.root","hist",20,-1,false,"B+b M(1000)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1000_RH_25ns.root","hist",30,-1,false,"B+t M(1000)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",40,-1,false,"B+b M(1500)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",320,-1,false,"B+t M(1500)");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets");
  treehists.SetTree("AnalysisTree");


  treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
  treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_sfmu_mediumID_REPLACE/weight_sfmu_mediumID",TreeHists::error_method::envelop,"up,down");
  //treehists.AddErrorWeight("weight_toptag_REPLACE",TreeHists::error_method::envelop,"up,down");
  
  //treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_up_Sel_Mu/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_down_Sel_Mu/"});
  //treehists.switch_logy(true);
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1)","50,50,3000","B mass [GeV]");
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.mass>-1)","50,50,3000","B mass [GeV]");
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12)","50,120,300","top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11)","50,120,300","top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1)","50,60,120","W_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1)","50,60,120","W_{lep} mass [GeV]");

  return 0;
}
