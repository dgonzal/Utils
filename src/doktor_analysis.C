#include <iostream>

#include "TreeRootHist.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"


int main(int argc, char** argv){
  string CMSSW = "8_0_24_patch1";
  bool electron = true;
  bool errors = false;
  string resultfile = "plots/doktor/Analysis/";
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
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/eletrigger_data.root","PE",1,20,false,"DATA");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt*.root","",3,-1,true,"WJets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_*.root","",2,-1,true,"ttbar");  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"ZJets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"singleT");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root","",4,-1,true,"QCD");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",20,-1,false,"BprimeB_800GeV_RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1100_RH.root","hist",20,-1,false,"BprimeB_1100GeV_RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1400_RH.root","hist",40,-1,false,"BprimeB_1400GeV_RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1700_RH.root","hist",40,-1,false,"BprimeB_1700GeV_RH",0);


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
  /*/
  //Ak4 jets
  treehists.Draw_ROOT("slimmedJets.slimmedJets.m_pt",factors+"weight","70,30,900","ak4 p_{T} [GeV]","Events", true, "", channel+"_ak4_pt__");
  treehists.Draw_ROOT("Length$(slimmedJets.slimmedJets.m_pt)",factors+"weight","11,-0.5,10.5","Number of ak4","Events", true, "", channel+"_ak4_num__");
  //AK8 Jets
  treehists.Draw_ROOT("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_pt",factors+"weight","40,170,1200","ak8 p_{T} [GeV]","Events", true, "", channel+"_ak8_pt__");
  treehists.Draw_ROOT("Length$(slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_eta)",factors+"weight","11,-0.5,10.5","Number of ak8","Events", true, "", channel+"_ak8_num__");
  treehists.Draw_ROOT("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau3/slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau2",factors+"weight","25,0.,1.","#tau_{3}/#tau_{2}","Events", true, "", channel+"_tau32__");
 
  //electron
  if(electron){
    treehists.Draw_ROOT("slimmedMETs.m_pt","weight","35,60,1000","p^{miss}_{T} [GeV]","Events", true, "", channel+"_e_met__");
    treehists.Draw_ROOT("slimmedElectronsUSER.slimmedElectronsUSER.m_pt",factors+"weight","35,118,800","e  p_{T} [GeV]","Events", true, "", channel+"_e_pt__");
    treehists.Draw_ROOT("slimmedMETs.m_pt + slimmedElectronsUSER.slimmedElectronsUSER.m_pt",factors+"weight","30,290,1000","H_{T,lep} [GeV]","Events", true, "", channel+"_e_htlep__"); 
  }
  //muon
  else{
    treehists.Draw_ROOT("slimmedMETs.m_pt",factors+"weight","35,50,1000","p^{miss}_{T} [GeV]","Events", true, "", channel+"_mu_met__");
    treehists.Draw_ROOT("slimmedMuonsUSER.slimmedMuonsUSER.m_pt",factors+"weight","35,55,800"," #mu p_{T} [GeV]","Events", true, "", channel+"_mu_pt__");
    treehists.Draw_ROOT("slimmedMETs.m_pt + slimmedMuonsUSER.slimmedMuonsUSER.m_pt",factors+"weight","30,250,1000","H_{T,lep} [GeV]","Events", true, "", channel+"_mu_htlep__");
  }
  /*/
  //treehists.Draw_ROOT("Chi2Dis.chi*(TopTagDis.mass<=0)+TopTagDis.chi*(TopTagDis.mass>0)",factors+"weight","50,0,1","Prob.(#chi^{2})","Events", true, "", channel+"_prob__");

  treehists.Draw_ROOT("slimmedElectronsUSER.slimmedElectronsUSER.m_pt",factors+"weight","35,118,800","e  p_{T} [GeV]","Events", true, "", channel+"_e_pt__");

  //treehists.Draw_ROOT("sqrt(Chi2Dis.wHad.M2())",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp == 11 && Chi2Dis.num_whad>1)","40,0,250","mass [GeV]","Events", true, "", channel+"_chi2_whad_mass__");
  return 0;


  treehists.Draw_ROOT("Chi2Dis.mass",factors+"weight*(TopTagDis.mass<=0 && Chi2Dis.btagEventNumber==0)","30,500,3000","m_{reco} [GeV]","Events", true, "", channel+"_chi2_mass_0btag__");
  treehists.Draw_ROOT("Chi2Dis.mass",factors+"weight*(TopTagDis.mass<=0 && Chi2Dis.btagEventNumber==1)","30,500,3000","m_{reco} [GeV]","Events", true, "", channel+"_chi2_mass_1btag__");
  treehists.Draw_ROOT("Chi2Dis.mass",factors+"weight*(TopTagDis.mass<=0 && Chi2Dis.btagEventNumber> 1)","30,500,3000","m_{reco} [GeV]","Events", true, "", channel+"_chi2_mass_2btag__");
  treehists.Draw_ROOT("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) < 2.4 )","50,500,3000","m_{reco} [GeV]","Events", true, "", channel+"_chi2_mass_Wtag__");

  treehists.Draw_ROOT("TopTagDis.mass",factors+"weight*(TopTagDis.mass>0 )","30,500,3000","m_{reco} [GeV]","Events", true, "", channel+"_toptag_mass__");



  return 0;
  treehists.Draw_ROOT("Chi2Dis.chi",factors+"weight*(TopTagDis.mass<=0)","20,0,1","#it{Prob.(#chi^{2},ndof)}","Events", true, "", channel+"_chi2_prob__");
  treehists.Draw_ROOT("TopTagDis.chi",factors+"weight*(TopTagDis.mass>0)","20,0,1","#it{Prob.(#chi^{2},ndof)}","Events", true, "", channel+"_ttag_prob__");
  treehists.Draw_ROOT("Chi2Dis.forwardJet.eta()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.forwardJet.pt()>0)","20,-5,5","forward ak4 jet #eta","Events", true, "", channel+"_chi2_forwardjet_fulleta__");
  treehists.Draw_ROOT("TopTagDis.forwardJet.eta()",factors+"weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt()>0)","20,-5,5","forward ak4 jet #eta","Events", true, "", channel+"_toptag_forwardjet_fulleta__");

  
  return 0;
  treehists.Draw_ROOT("Chi2Dis.topHad.M()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp == 12 && (Chi2Dis.num_whad+Chi2Dis.num_top)>1)","40,0,350","mass [GeV]","Events", true, "", channel+"_chi2_topHad_mass__");
  treehists.Draw_ROOT("Chi2Dis.topLep.M()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp == 11)","40,100,350","Mass [GeV]","Events", true, "", channel+"_chi2_topLep_mass__");
  treehists.Draw_ROOT("sqrt(Chi2Dis.wHad.M2())",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp == 12 && Chi2Dis.num_whad>1)","40,0,250","mass [GeV]","Events", true, "", channel+"_chi2_whad_mass__");
  treehists.Draw_ROOT("Chi2Dis.wLep.pt()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp == 11)","40,100,1500" ,"p_{T} [GeV]","Events", true, "", channel+"_chi2_wlep_pt__");
  treehists.Draw_ROOT("sqrt(TopTagDis.topHad.M2())",factors+"weight*("+toptag_central_string+")","40,100,350","mass [GeV]","Events", true, "", channel+"_toptag_tophad_mass__");
  treehists.Draw_ROOT("TopTagDis.wLep.pt()",factors+"weight*("+toptag_central_string+")","40,100,1500","p_{T} [GeV]","Events", true, "", channel+"_toptag_wlep_pt__");

  treehists.Draw_ROOT("Chi2Dis.forwardJet.eta()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && fabs(Chi2Dis.forwardJet.eta())>=2.4)","20,-5,5","forward ak4 jet #eta","Events", true, "", channel+"_chi2_forwardjet_eta__");
  treehists.Draw_ROOT("TopTagDis.forwardJet.eta()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && fabs(Chi2Dis.forwardJet.eta())>=2.4)","20,-5,5","forward ak4 jet #eta","Events", true, "", channel+"_toptag_forwardjet_eta__");

  
  
}
