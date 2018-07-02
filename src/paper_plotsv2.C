#include <iostream>

#include "TreeRootHist.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"


int main(int argc, char** argv){
  string CMSSW = "8_0_24_patch1";
  bool electron = false;
  bool errors = true;
  string resultfile = "plots/paper/";
  bool single = true; 
  string folder = "MuSel_new";
  if(electron) folder = "EleSel_new";
  string eletriggerfactors = "";
  if(electron)  eletriggerfactors ="("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  string muontrkfactors =""; 
  if(!electron) muontrkfactors = "("+muon_trk_factors()+"*(1-isRealData)+isRealData)*";
  string factors = muontrkfactors+eletriggerfactors;//+forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400");
  //factors = eletriggerfactors;.
  std::string channel = "Mu";
  if(electron) channel="Ele";
  
  cout<<"applying factors: "<<factors<<endl;
  
  TreeRootHist treehists(resultfile,single);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/Merge_Data.root","PE",1,20,false,"DATA");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt*.root","",3,-1,true,"WJets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_*.root","",2,-1,true,"ttbar");  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"ZJets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"singleT");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root","",4,-1,true,"QCD");
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1100_RH.root","hist",20,-1,false,"Bprimeb_1100GeV_RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1700_RH.root","hist",40,-1,false,"Bprimeb_1700GeV_RH",0);
 

  treehists.SetTree("AnalysisTree");
 
  if(errors){
    treehists.addUnc("pdfWeight","","rms","pdf");
    treehists.addUnc("scaleWeight_up","scaleWeight_down","envelop","scale");
    treehists.addUnc("weight_btag_up/weight_btag","weight_btag_down/weight_btag","envelop","btag");
    treehists.addUnc("weight_pu_up/weight_pu","weight_pu_down/weight_pu","envelop","PU");
  
    if(!electron){
      treehists.addUnc("weight_sfmu_tight_up/weight_sfmu_tight","weight_sfmu_tight_down/weight_sfmu_tight","envelop","muonID");
      treehists.addUnc("weight_sfmu_muonTrigger_up/weight_sfmu_muonTrigger","weight_sfmu_muonTrigger_down/weight_sfmu_muonTrigger","envelop","muonTrigger");
      treehists.addUnc("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_down_MuSelUNC/","folder","jec");
      treehists.addUnc("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_MuSelUNC/","folder","jer");
    }
    else{
      //ele trigger variations
      treehists.addUnc("*("+eletriggerscale(1)+")","*("+eletriggerscale(2)+")","envelop","eletrigger");
      treehists.addUnc("weight_sfelec_eleid_up/weight_sfelec_eleid","weight_sfelec_eleid_down/weight_sfelec_eleid","envelop","eleSf");
      treehists.addUnc("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_up_EleSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_down_EleSelUNC/","folder","jec");
      treehists.addUnc("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_EleSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_EleSelUNC/","folder","jer");
    }
    treehists.addFolderAlias("slimmedJets.","jet_jec_up."  ,"jec_up");
    treehists.addFolderAlias("slimmedJets.","jet_jec_down.","jec_down");
    treehists.addFolderAlias("slimmedJets.","jet_jer_up."  ,"jer_up");
    treehists.addFolderAlias("slimmedJets.","jet_jer_down.","jer_down");

    treehists.addFolderAlias("slimmedMETs.","met_jec_up."  ,"jec_up");
    treehists.addFolderAlias("slimmedMETs.","met_jec_down.","jec_down");
    treehists.addFolderAlias("slimmedMETs.","met_jer_up."  ,"jer_up");
    treehists.addFolderAlias("slimmedMETs.","met_jer_down.","jer_down");
  }
 
  
  //treehists.switch_logy(true);
  string eta = "2.4";
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) >="+eta;
  string toptag_scalefactor = "1.01";
  //string wtag_scalefactor = "";
  treehists.Draw_ROOT("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) < 2.4 )","30,300,3500","m_{reco} [GeV]","Events", true, "", channel+"_chi2_mass__");

  treehists.Draw_ROOT("TopTagDis.mass",factors+"weight*("+toptag_central_string+")","30,300,3500","m_{reco} [GeV]","Events", true, "", channel+"_toptag_mass__");

}
