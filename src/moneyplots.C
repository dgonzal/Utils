#include <iostream>

#include "TreeHists.h"
#include "TChainHists.h"
#include "simplePlots.h"
#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"


int main(int argc, char** argv){
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_new";
  bool electron = false;
  bool errors = true;
  bool blind = false;
  string resultfile = "loose_btag_background_plots.ps";//"moneyplots.pdf";
  bool single = true; 
  
  if(argc>1){
    std::string channel(argv[1]);
    if(channel == "electron")
      electron = true;
    else if(channel=="muon")
      electron = false;
    else{
      std::cout<<"you need to choose between muon & electron"<<std::endl;
      std::cout<<"your input was: "<<channel<<std::endl;
      return 1;
    }
  }
  if(argc>2){
    std::string calc_err(argv[2]);
    if(calc_err=="true")
      errors = true;
    else if(calc_err=="false")
      errors = false;
    else{
      std::cout<<"options for error handling are true & false"<<std::endl;
      std::cout<<"your input was: "<<calc_err<<std::endl;
      return 2;
    }
  }
  if(argc>3){
    resultfile = std::string (argv[3]);
  }

  if (electron){
    folder = "EleSel_new";
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Ele_"+resultfile;
    if(single) resultfile= "plots/singleEleMoney_nomsig_reco_noforwardreweight/";
  }
  else{
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Mu_"+resultfile;
    if(single) resultfile= "plots/singleMuMoney_nomsig_reco_nolegend/";
  }
  cout<<"Folder: "<<folder<<" end file: "<<resultfile<<endl;
  //std::cout<<"going to print "<<resultfile<<" from "<< electron ? "electron" : "muon"<<" channel " <<errors ? " with sys errors":" without sys errors"<<endl; 

  string eletriggerfactors = "";
  if(electron)  eletriggerfactors ="("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  string muontrkfactors =""; 
  if(!electron) muontrkfactors = "("+muon_trk_factors()+"*(1-isRealData)+isRealData)*";
  string factors = muontrkfactors+eletriggerfactors;//+forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400");
  //factors = eletriggerfactors;
  cout<<"applying factors: "<<factors<<endl;



  
  TreeHists treehists(resultfile,single);
  //if(single) treehists.switch_singleplots(true);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  treehists.set_ignorePages(13);
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/Merge_Data.root","PE",1,20,false,"Data");
  //treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/eletrigger_data.root","PE",1,20,false,"Data");
   
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt*.root","",3,-1,true,"W+Jets"); 
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"single t");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root","",4,-1,true,"QCD");
  

  //
  //Scaled to 5 pb
  //
  /*/ 
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",4,-1,false,"B+b M(0.8) RH #times 3",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",7,-1,false,"B+t M(0.8) RH #times 9",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",20,-1,false,"B+b M(1) RH #times 6",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",30,-1,false,"B+t M(1) RH #times 18",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",40,-1,false,"B+b M(1.5) RH #times 24",0,5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",800,-1,false,"B+t M(1.5) RH #times 71",0,5);
   /*/  
  //
  // Scaled to cross section
  // [4.339,3.016,2.219,1.653,1.192,0.896,0.679,0.529,0.415,0.319,0.248,0.195]*0.5
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",4,-1,false,"B+b M(0.8) RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",20,-1,false,"B+b M(1) RH",0);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",40,-1,false,"B+b M(1.5) RH",0);

  treehists.set_ratiolimits(1.69,0.39);
  treehists.SetTree("AnalysisTree");
  //treehists.switch_legend(false);
  
  if(errors){
    treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
    treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");   
    if(!electron){
      treehists.AddErrorWeight("weight_sfmu_tight_REPLACE/weight_sfmu_tight",TreeHists::error_method::envelop,"up,down");
      treehists.AddErrorWeight("weight_sfmu_muonTrigger_REPLACE/weight_sfmu_muonTrigger",TreeHists::error_method::envelop,"up,down");
      treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_jer_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_jer_down_MuSelUNC/"});
      //treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_MuSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_MuSelUNC/"});
    }
    else{
      //ele trigger variations
      treehists.AddErrorWeight("REPLACE",TreeHists::error_method::envelop,eletriggerscale(1)+","+eletriggerscale(2));

      treehists.AddErrorWeight("weight_sfelec_eleid_REPLACE/weight_sfelec_eleid",TreeHists::error_method::envelop,"up,down");
      treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_jer_up_EleSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jec_jer_down_EleSelUNC/"});
      //treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_up_EleSelUNC/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jer_down_EleSelUNC/"});
    }
    treehists.AddErrorFolderAlias("slimmedJets.","jet_jec_jer_up."  ,"jec_jer_up");
    treehists.AddErrorFolderAlias("slimmedJets.","jet_jec_jer_down.","jec_jer_down");

    treehists.AddErrorFolderAlias("slimmedMETs.","met_jec_jer_up."  ,"jec_jer_up");
    treehists.AddErrorFolderAlias("slimmedMETs.","met_jec_jer_down.","jec_jer_down");
  }
  //treehists.switch_logy(true);
  
  TLatex *lumi_text = new TLatex(0.9, 0.95,"#mu channel 35.9 fb^{-1} (13 TeV)");
  lumi_text->SetNDC();
  lumi_text->SetTextAlign(33);
  lumi_text->SetTextFont(42);
  lumi_text->SetTextSize(0.04);

  if(electron){
    lumi_text = new TLatex(0.9, 0.95,"e channel 35.9 fb^{-1} (13 TeV)");
    lumi_text->SetNDC();
    lumi_text->SetTextAlign(33);
    lumi_text->SetTextFont(42);
    lumi_text->SetTextSize(0.04);
  }
  
  TLatex *cms_text = new TLatex(0.1, 0.95,"CMS");
  cms_text->SetNDC();
  cms_text->SetTextAlign(13);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.045);
 
  TLatex *chi2_text = new TLatex(0.8, 0.3,"X^{2} reconstruction");
  chi2_text->SetNDC();
  chi2_text->SetTextAlign(33);
  chi2_text->SetTextFont(42);
  chi2_text->SetTextSize(0.05);
  
  TLatex *toptag_text = new TLatex(0.8, 0.3,"t-tag reconstruction");
  toptag_text->SetNDC();
  toptag_text->SetTextAlign(33);
  toptag_text->SetTextFont(42);
  toptag_text->SetTextSize(0.05);


  treehists.addText(lumi_text);  
  treehists.addText(cms_text);
  
  
  string eta = "2.4";
  string chi2_central_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "(TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && Chi2Dis.chi<10 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400) && abs(TopTagDis.forwardJet.eta()) >="+eta;
  string toptag_scalefactor = "1.01";
  //string wtag_scalefactor = "";

  
  treehists.Draw("slimmedJets.slimmedJets.m_pt",factors+"weight","70,30,900","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_eta",factors+"weight","70,-5,5","ak4 #eta","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_phi",factors+"weight","50,-3.14,3.14","ak4 #phi","Events");
  treehists.Draw("Sum$(slimmedJets.slimmedJets.m_pt>50)",factors+"weight","11,-0.5,10.5","Number of ak4","Events");
  //AK8 Jets
  treehists.Draw("Length$(slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_eta)",factors+"weight","11,-0.5,10.5","Number of ak8","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_pt",factors+"weight","50,170,1200","ak8 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_eta",factors+"weight","50,-5,5","ak8 #eta","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_phi",factors+"weight","50,-3.14,3.14","ak8 #phi","Events");

  treehists.Draw("Length$(slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_subjets.m_eta)",factors+"weight","9,-0.5,8.5","Number of ak8 subjets","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau3/slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau2",factors+"weight","50,0.,1.","#tau_{3}/#tau_{2}","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau2/slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau1",factors+"weight","50,0.,1.","#tau_{2}/#tau_{1}","Events");
  
  
   //electron
  if(electron){
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta",factors+"weight","50,-2.4,2.4","electron #eta","Events");
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt",factors+"weight","70,118,800"," electron p_{T} [GeV]","Events");
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_phi",factors+"weight","50,-3.14,3.14"," electron #phi","Events");
  }
  //muon
  else{
    treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta",factors+"weight","50,-2.4,2.4","muon #eta","Events");
    treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt",factors+"weight","70,55,800"," muon p_{T} [GeV]","Events");
    treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_phi",factors+"weight","50,-3.14,3.14"," muon #phi","Events");
  }
  //event stuff 
  treehists.Draw("HT",factors+"weight","50,100,2000","H_{T} [GeV]","Events");
  if(electron){
    treehists.Draw("slimmedMETs.m_pt","weight","70,60,1000","MET [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedElectronsUSER.slimmedElectronsUSER.m_pt",factors+"weight","35,290,1000","H_{T,lep} [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedElectronsUSER.slimmedElectronsUSER.m_pt + HT",factors+"weight","70,290,2000","S_{T} [GeV]","Events");
  }
  else{
    treehists.Draw("slimmedMETs.m_pt",factors+"weight","70,50,1000","MET [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedMuonsUSER.slimmedMuonsUSER.m_pt",factors+"weight","30,250,1000","H_{T,lep} [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedMuonsUSER.slimmedMuonsUSER.m_pt + HT",factors+"weight","30,250,2000","S_{T} [GeV]","Events");
  }
  
  /*/
  treehists.Draw("slimmedMETs.eta()","weight","70,-5,5","MET [GeV]","Events");
  treehists.Draw("slimmedMETs.eta()","weight*(fabs(Chi2Dis.forwardJet.eta())<2.4)","70,-5,5","MET [GeV]","Events");
  treehists.Draw("slimmedMETs.eta()","weight*(fabs(Chi2Dis.forwardJet.eta())>=2.4)","70,-5,5","MET [GeV]","Events");
  
  treehists.switch_logy(true);
  treehists.Draw("slimmedMETs.eta()","weight","70,-5,5","MET [GeV]","Events");
  treehists.Draw("slimmedMETs.eta()","weight*(fabs(Chi2Dis.forwardJet.eta())<2.4)","70,-5,5","SR MET [GeV]","Events");
  treehists.Draw("slimmedMETs.eta()","weight*(fabs(Chi2Dis.forwardJet.eta())>=2.4)","70,-5,5","CR MET [GeV]","Events");


  return 0;
  /*/
  
  treehists.Draw("Chi2Dis.btagEventNumber",factors+"weight*(TopTagDis.mass==-1 && Chi2Dis.btagEventNumber>-0.1)","6,-.5,5.5"," Number of b-tags");
  
  //forward jet
  //X2
  treehists.Draw("fabs(Chi2Dis.forwardJet.eta())>=2.4",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1)","6,-.5,5.5"," X^{2} forward ak4");
  treehists.Draw("Chi2Dis.forwardJet.eta()",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 && (isRealData==0 || fabs(Chi2Dis.forwardJet.eta())<2))","50,-5,5","X^{2} forward ak4 #eta","Events");
  treehists.Draw("Chi2Dis.forwardJet.eta()",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 )","50,-5,5","forward ak4 #eta","Events");
  treehists.Draw("Chi2Dis.forwardJet.energy()",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 )","50,100,1200","forward ak4 E","Events");
  treehists.Draw("Chi2Dis.forwardJet.pt()",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 )","50,30,200","forward ak4 p_{T}","Events");

  //toptag
  treehists.Draw("fabs(Chi2Dis.forwardJet.eta())>=2.4",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1)","6,-.5,5.5","toptag forward ak4");
  treehists.Draw("TopTagDis.forwardJet.eta()" ,factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0 && (isRealData==0 || fabs(TopTagDis.forwardJet.eta())<2))","20,-5,5","forward ak4 #eta","Events");
  treehists.Draw("TopTagDis.forwardJet.eta()",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0)","20,-5,5","forward ak4 #eta","Events");
  treehists.Draw("TopTagDis.forwardJet.energy()",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0)","50,100,1200","forward ak4 E","Events");
  treehists.Draw("TopTagDis.forwardJet.pt()",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0)","50,30,200","forward ak4 p_{T}","Events");
  //W-tag
  treehists.Draw("fabs(Chi2Dis.forwardJet.eta())>=2.4",factors+"weight*(TopTagDis.mass==-1 && WTagDis.mass>0)","6,-.5,5.5","W-tag forward ak4");
  treehists.Draw("Chi2Dis.forwardJet.eta()",factors+"weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 && (isRealData==0 || fabs(WTagDis.forwardJet.eta())<2))","20,-5,5","forward ak4 #eta","Events");
  treehists.Draw("Chi2Dis.forwardJet.eta()",factors+"weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 && WTagDis.mass>0)","20,-5,5","forward ak4 #eta","Events");
  treehists.Draw("Chi2Dis.forwardJet.energy()",factors+"weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 && WTagDis.mass>0)","50,100,1200","forward ak4 E","Events");
  treehists.Draw("Chi2Dis.forwardJet.pt()",factors+"weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 && WTagDis.mass>0)","50,30,200","ak4 p_{T}","Events");
 
 
  //X2 reco masses
  treehists.Draw("Chi2Dis.topHad.M()",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp==12)","50,120,300","top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp==11)","50,120,300","top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.topHad.pt()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp==12)","50,0,1000","top_{had} p_{T} [GeV]");
  treehists.Draw("Chi2Dis.topLep.pt()",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && Chi2Dis.recoTyp==11)","50,0,1000","top_{lep} p_{T} [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()",factors+"weight*(TopTagDis.mass==-1    || TopTagDis.topHad.pt()<400)","50,60,120","W_{had} mass [GeV]","Events");
  treehists.Draw("Chi2Dis.wHad.pt()",factors+"weight*(TopTagDis.mass==-1   || TopTagDis.topHad.pt()<400)","50,0,1000","W_{had} p_{T} [GeV]","Events");
  treehists.Draw("Chi2Dis.wLep.M()",factors+"weight*(TopTagDis.mass==-1    || TopTagDis.topHad.pt()<400)","50,60,120","W_{lep} mass [GeV]");



  //toptag mass
  treehists.Draw("TopTagDis.topHad.M()",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.topHad.pt()>=400)","30,105,300","top_{had} mass [GeV]");
  treehists.Draw("TopTagDis.topHad.pt()",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1&& TopTagDis.topHad.pt()>=400)","30,350,1000","top_{had} p_{T} [GeV]");


  //VLQ mass central
  treehists.addText(chi2_text);
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","m_{reco} [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","m_{reco} [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","m_{reco} [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) < 2.4 )","50,500,3000","m_{reco} [GeV]","Events");

  if(blind){
    treehists.removeFile(0);
    if(electron) treehists.removeFile(0);   
    treehists.mcratio_only();
    //treehists.switch_ratio(false);
  }
  
 
  //treehists.switch_logy(true);
  //VLQ mass forward/total
  treehists.Draw("Chi2Dis.mass",factors+"weight*(TopTagDis.mass==-1)","30,500,3000","m_{reco} [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","m_{reco} [GeV]");	    
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","m_{reco} [GeV]" );	    
  treehists.Draw("Chi2Dis.mass",factors+"weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","m_{reco} [GeV]" );
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400) && WTagDis.mass>-1 && abs(Chi2Dis.forwardJet.eta()) >= 2 && Chi2Dis.btagEventNumber> 0)","50,500,3000","m_{reco}","Events");
  
  //treehists.switch_logy(false);
  treehists.clearText();
  treehists.addText(lumi_text);  
  treehists.addText(cms_text);
  treehists.addText(toptag_text);
		    
  treehists.Draw("TopTagDis.mass",factors+"weight*("+toptag_central_string+")","30,500,3000","m_{reco} [GeV]");
  treehists.Draw("TopTagDis.mass",factors+toptag_scalefactor+"*weight*(TopTagDis.mass>-1 && TopTagDis.topHad.pt()>=400)","30,500,3000","m_{reco} [GeV]");
  treehists.Draw("TopTagDis.mass",factors+toptag_scalefactor+"*weight*("+toptag_forward_string+")","30,500,3000","m_{reco} [GeV]");

 
  return 0;
}
