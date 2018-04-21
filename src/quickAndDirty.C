#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(int argc, char** argv){
  string version = "new";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_"+version;
  bool electron = false;
  bool errors = true;
  bool blind = true;
  string resultfile = "quick.ps";

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
  }
  else{
    if(resultfile=="moneyplots.ps")
      resultfile = "plots/Mu_"+resultfile;
  }

  //std::cout<<"going to print "<<resultfile<<" from "<< electron ? "electron" : "muon"<<" channel " <<errors ? " with sys errors":" without sys errors"<<endl; 



  TreeHists treehists(resultfile);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  //if(!electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root","PE",1,20,false,"Data");
  //if(electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleData.root","PE",1,20,false,"Data");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",4,-1,false,"B+b M(800)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",7,-1,false,"B+t M(800)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1200_RH.root","hist",20,-1,false,"B+b M(1200)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1200_RH.root","hist",20,-1,false,"B+b M(1200)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1400_RH.root","hist",30,-1,false,"B+t M(1400)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1400_RH.root","hist",30,-1,false,"B+t M(1400)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1600_RH.root","hist",20,-1,false,"B+b M(1600)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1600_RH.root","hist",30,-1,false,"B+t M(1600)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1800_RH.root","hist",40,-1,false,"B+b M(1800)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1800_RH.root","hist",12,-1,false,"B+t M(1800)",0,1);

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.X53-800_RH.root","hist",7,-1,false,"B+t M(800)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.X53-1200_RH.root","hist",20,-1,false,"B+b M(1200)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.X53-1400_RH.root","hist",30,-1,false,"B+t M(1400)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.X53-1600_RH.root","hist",30,-1,false,"B+t M(1600)",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.X53-1800_RH.root","hist",12,-1,false,"B+t M(1800)",0,1);


/*/
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root","",3,-1,true,"W+Jets");
/*/

  treehists.SetTree("AnalysisTree");
  treehists.switch_ratio(false);
  if(errors){
    treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
    treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
    if(!electron)treehists.AddErrorWeight("weight_sfmu_mediummuon2016_REPLACE/weight_sfmu_mediummuon2016",TreeHists::error_method::envelop,"up,down");
    //treehists.AddErrorWeight("weight_toptag_REPLACE",TreeHists::error_method::envelop,"up,down");
  
    // treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_up_Sel_Mu/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_down_Sel_Mu/"});
  }
  //treehists.switch_logy(true);

  TLatex *cms_text = new TLatex(3.5, 24," 35.8 fb^{-1} (13 TeV)"); //CMS Preliminary
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetX(0.9);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);
  cms_text->SetY(.95);
  treehists.addText(cms_text);

  string eta = "2.0";
  string energy = "0.";
  string chi2_central_string = "TopTagDis.mass==-1 &&((abs(Chi2Dis.forwardJet.eta()) <" +eta+") || Chi2Dis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+") && Chi2Dis.forwardJet.E()>="+energy;
  string toptag_central_string = "TopTagDis.mass>-1 &&((abs(TopTagDis.forwardJet.eta()) <" +eta+") || TopTagDis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "TopTagDis.mass>-1 && (abs(TopTagDis.forwardJet.eta()) >="+eta+") && TopTagDis.forwardJet.E()>="+energy;

  treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","forward X^{2} 0 b-tags B mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","forward X^{2} 1 b-tag B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","forward X^{2} 2 b-tag B mass [GeV]" );
  treehists.Draw("TopTagDis.mass","weight*("+toptag_forward_string+")","30,500,3000","forward toptag B mass [GeV]");

  string elept_cut = "(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]<650 &&";
  treehists.Draw("Chi2Dis.mass","weight*"+elept_cut+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","forward X^{2} 0 b-tags B mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*"+elept_cut+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","forward X^{2} 1 b-tag B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass","weight*"+elept_cut+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","forward X^{2} 2 b-tag B mass [GeV]" );
  treehists.Draw("TopTagDis.mass","weight*"+elept_cut+toptag_forward_string+")","30,500,3000","forward toptag B mass [GeV]");

  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta","weight","50,-2.4,2.4","electron #eta","Events",false);
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt","weight","70,55,1200"," electron p_{T} [GeV]","Events"); 
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_phi","weight","50,-3.14,3.14"," electron #phi","Events",false);

  vector<TH1F*> hists = treehists.return_hists("slimmedElectronsUSER.slimmedElectronsUSER.m_pt","weight","70,45,1200"," electron p_{T} [GeV]","Events");	 
  
  for(auto h : hists){
    double  content = 0;
    for(int i =1; i<h->GetNcells()-1;i++){
        if(h->GetBinLowEdge(i)+h->GetBinWidth(i) > 650)
	  break;
        content += h->GetBinContent(i);
    }
    cout<<h->GetName()<<" "<<content/h->GetSumOfWeights()<<endl;  
  }



  return 0;



  /*
  //AK4 Jets
  treehists.Draw("Length$(slimmedJets.slimmedJets.m_eta)","weight","16,-0.5,15.5","Number of ak4","Events",false);
  treehists.Draw("slimmedJets.slimmedJets.m_eta","weight","50,-5,5","ak4 #eta","Events",false);
  treehists.Draw("slimmedJets.slimmedJets.m_pt","weight","70,30,900","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_phi","weight","50,-3.14,3.14","ak4 #phi","Events",false);
  treehists.Draw("Sum$(slimmedJets.slimmedJets.m_pt>50)","weight","11,-0.5,10.5","Number of ak4","Events",false);

  //AK8 Jets
  treehists.Draw("Length$(slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_eta)","weight","11,-0.5,10.5","Number of ak8","Events",false);
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_pt","weight","50,170,1200","AK8 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_eta","weight","50,-5,5","AK8 #eta","Events",false);
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_phi","weight","50,-3.14,3.14","AK8 #phi","Events",false);

  treehists.Draw("Length$(slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_subjets.m_eta)","weight","9,-0.5,8.5","Number of ak8 subjets","Events",false);
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau3/slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau2","weight","50,0.,1.","#tau_{3}/#tau_{2}","Events",false);
  treehists.Draw("slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau2/slimmedJetsAK8_SoftDrop.slimmedJetsAK8_SoftDrop.m_tau1","weight","50,0.,1.","#tau_{2}/#tau_{1}","Events",false);


   //electron
  if(electron){
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta","weight","50,-2.4,2.4","electron #eta","Events",false);
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt","weight","70,118,800"," electron p_{T} [GeV]","Events");
    treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_phi","weight","50,-3.14,3.14"," electron #phi","Events",false);
  }
  //muon
  else{
    treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta","weight","50,-2.4,2.4","muon #eta","Events",false);
    treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt","weight","70,55,800"," muon p_{T} [GeV]","Events");
    treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_phi","weight","50,-3.14,3.14"," muon #phi","Events",false);
  }
  
  //event stuff 
  treehists.Draw("HT","weight","50,100,2000","H_{T} [GeV]","Events");
  if(electron){
    treehists.Draw("slimmedMETs.m_pt","weight","70,60,1000","MET [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedElectronsUSER.slimmedElectronsUSER.m_pt","weight","70,290,1000","H_{T,lep} [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedElectronsUSER.slimmedElectronsUSER.m_pt + HT","weight","70,290,2000","S_{T} [GeV]","Events");
  }
  else{
    treehists.Draw("slimmedMETs.m_pt","weight","70,50,1000","MET [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedMuonsUSER.slimmedMuonsUSER.m_pt","weight","30,250,1000","H_{T,lep} [GeV]","Events");
    treehists.Draw("slimmedMETs.m_pt + slimmedMuonsUSER.slimmedMuonsUSER.m_pt + HT","weight","30,250,2000","S_{T} [GeV]","Events");
  }
  */
  //forward jet
  //X2
  treehists.Draw("Chi2Dis.forwardJet.pt()>0","weight*(TopTagDis.mass==-1 )","6,-.5,5.5"," X^{2} forward ak4");
  treehists.Draw("Chi2Dis.forwardJet.eta()","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0 && (isRealData==0 || fabs(Chi2Dis.forwardJet.eta())<2))","50,-5,5","X^{2} forward ak4 #eta","Events",false);
  treehists.Draw("Chi2Dis.forwardJet.energy()","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0)","50,100,1200","X^{2} forward ak4 E","Events",false);
  treehists.Draw("Chi2Dis.forwardJet.pt()","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJet.pt() > 0)","50,30,200","X^{2} forward ak4 p_{T}","Events",false);
  //toptag
  treehists.Draw("TopTagDis.forwardJet.pt()>0","weight*(TopTagDis.mass>-1)","6,-.5,5.5","toptag forward ak4");
  treehists.Draw("TopTagDis.forwardJet.eta()","weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0 && (isRealData==0 || fabs(TopTagDis.forwardJet.eta())<2))","20,-5,5","toptag forward ak4 #eta","Events",false);
  treehists.Draw("TopTagDis.forwardJet.energy()","weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0)","50,100,1200","toptag forward ak4 E","Events",false);
  treehists.Draw("TopTagDis.forwardJet.pt()","weight*(TopTagDis.mass>-1 && TopTagDis.forwardJet.pt() > 0)","50,30,200","toptag forward ak4 p_{T}","Events",false);
  /*
  //X2 reco masses
  treehists.Draw("Chi2Dis.topHad.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==12)","50,120,300","X^{2} top_{had} mass [GeV]");
  treehists.Draw("Chi2Dis.topLep.M()","weight*(TopTagDis.mass==-1 && Chi2Dis.recoTyp==11)","50,120,300","X^{2} top_{lep} mass [GeV]");
  treehists.Draw("Chi2Dis.wHad.M()","weight*(TopTagDis.mass==-1)","50,60,120","X^{2} W_{had} mass [GeV]","Events",false);
  treehists.Draw("Chi2Dis.wLep.M()","weight*(TopTagDis.mass==-1)","50,60,120","X^{2} W_{lep} mass [GeV]");
  //toptag mass
  treehists.Draw("TopTagDis.topHad.M()","weight*(TopTagDis.mass>-1)","30,100,300","toptag top_{had} mass [GeV]");

  //VLQ mass central
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","central X^{2} 0 b-tags B mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","central X^{2} 1 b-tag B mass [GeV]" );
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_central_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","central X^{2} 2 b-tag B mass [GeV]" );

  treehists.Draw("TopTagDis.mass","weight*("+toptag_central_string+")","30,500,3000","central toptag B mass [GeV]");
  */
  if(blind){
    treehists.removeFile(0);
    treehists.mcratio_only();
  }
  
  treehists.switch_ratio(false);
  treehists.switch_logy(true);
  //VLQ mass forward/total
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1)","30,500,3000","B mass [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==0)","30,500,3000","forward X^{2} 0 b-tags B mass [GeV]");	    
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber==1)","30,500,3000","forward X^{2} 1 b-tag B mass [GeV]" );	    
  treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+"&& Chi2Dis.btagEventNumber> 1)","30,500,3000","forward X^{2} 2 b-tag B mass [GeV]" );	   

  treehists.switch_logy(false);
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.mass>-1)","30,500,3000","B mass [GeV]");
  treehists.Draw("TopTagDis.mass","weight*("+toptag_forward_string+")","30,500,3000","forward toptag B mass [GeV]");
 
 
  return 0;
}
