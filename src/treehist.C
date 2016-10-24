#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(){
  string version = "v40";
  string CMSSW = "7_6_3";
  string folder = "Selection_"+version;//"jecsmear_direction_up_Sel";//"Selection_"+version;
  


  TreeHists treehists("plots/"+folder+".ps");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets",0.1);
 
  treehists.SetTree("AnalysisTree");
  /*
  treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
  treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_sfmu_mediumID_REPLACE/weight_sfmu_mediumID",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_toptag_REPLACE",TreeHists::error_method::envelop,"up,down");
  
  treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_up_Sel_Mu/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_down_Sel_Mu/"});
  */
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta >= 2.4)","50,50,3000","Mass B [GeV]");
  
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehists.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta < 2.4)","50,50,3000","Mass B [GeV]");
  treehists.Draw("Chi2Dis.chi","weight*(TopTagDis.mass==-1)","100,0,200","#Chi No top-tag");
  treehists.Draw("TopTagDis.chi","weight*(TopTagDis.mass>0)","100,0,100","#Chi top-tag");

  /*
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta <  2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta <  2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta <  2.4 && Chi2Dis.btagEventNumber ==2)","50,50,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==2)","50,50,3000","Mass B [GeV]");
  
  std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass > -1 && Chi2Dis.forwardJetAbsEta >=  2.4)","50,50,3000","Mass B [GeV]");
  std::vector<TH1F*> toptag_central_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass > -1 && Chi2Dis.forwardJetAbsEta <   2.4)","50,50,3000","Mass B [GeV]");
  */

  string eta = "2.4";
  string energy = "150";
  string jetiso = "1.8";

  std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (Chi2Dis.forwardJetAbsEta >="+eta+"||Chi2Dis.jetiso>="+jetiso+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber==0)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (Chi2Dis.forwardJetAbsEta >="+eta+"||Chi2Dis.jetiso>="+jetiso+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber==1)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (Chi2Dis.forwardJetAbsEta >="+eta+"||Chi2Dis.jetiso>="+jetiso+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber> 1)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((Chi2Dis.forwardJetAbsEta<"+eta+"&&Chi2Dis.jetiso<"+jetiso+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber==0)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((Chi2Dis.forwardJetAbsEta<"+eta+"&&Chi2Dis.jetiso<"+jetiso+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber==1)","100,400,3000","Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((Chi2Dis.forwardJetAbsEta<"+eta+"&&Chi2Dis.jetiso<"+jetiso+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber> 1)","100,400,3000","Mass B [GeV]");

  std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0&&(TopTagDis.forwardJetAbsEta>="+eta+"||TopTagDis.jetiso>="+jetiso+")&&TopTagDis.forwardJet.E()>="+energy+")","100,400,3000");
  std::vector<TH1F*> toptag_central_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0 &&((TopTagDis.forwardJetAbsEta<"+eta+"&&TopTagDis.jetiso<"+jetiso+")||TopTagDis.forwardJet.E()<"+energy+"))","100,400,3000");                            



  vector<string> sample_nick ={"Data","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","W+Jets"};
  vector<string> category = {"Anti-b-tag","1 b-tag","2 b-tags","" };
  vector<vector<TH1F*>> central_hists = {chi2_0btag_central_hist,chi2_1btag_central_hist,chi2_2btag_central_hist,toptag_central_hist};
  vector<vector<TH1F*>> forward_hists = {chi2_0btag_forward_hist,chi2_1btag_forward_hist,chi2_2btag_forward_hist,toptag_forward_hist};

  simplePlots comparison("plots/comparision_forward_central.ps");
  comparison.normToArea(true,0.2);
  for(unsigned int i =0; i<central_hists.size();i++){
    string recotype = " X^2";
    if(i==3) recotype = "TopTag";
    for(unsigned int m=0;m<central_hists.at(i).size();m++){
      central_hists.at(i).at(m)->Sumw2();
      forward_hists.at(i).at(m)->Sumw2();
      string histTitel = sample_nick[m]+" "+category[i]+recotype;
      cout<<histTitel<<" ratio "<<forward_hists.at(i).at(m)->GetEntries()<< " / "<<central_hists.at(i).at(m)->GetEntries()<<" = " <<forward_hists.at(i).at(m)->GetEntries()/central_hists.at(i).at(m)->GetEntries()<<endl;
      central_hists.at(i).at(m)->SetTitle(histTitel.c_str());
      central_hists.at(i).at(m)->SetMarkerStyle(20);
      comparison.loadHists(central_hists.at(i).at(m),"Central","PE");
      comparison.loadHists(forward_hists.at(i).at(m),"Forward","hist");
      comparison.plotHists(2,false);
      comparison.clearAll();

    }
  }

  for(int i=4;i<8;i++){
    chi2_0btag_central_hist[3]->Add(chi2_0btag_central_hist[i]);
    chi2_1btag_central_hist[3]->Add(chi2_1btag_central_hist[i]);
    chi2_2btag_central_hist[3]->Add(chi2_2btag_central_hist[i]);

    chi2_0btag_forward_hist[3]->Add(chi2_0btag_forward_hist[i]);
    chi2_1btag_forward_hist[3]->Add(chi2_1btag_forward_hist[i]);
    chi2_2btag_forward_hist[3]->Add(chi2_2btag_forward_hist[i]);

    toptag_central_hist[3]->Add(toptag_central_hist[i]);
    toptag_forward_hist[3]->Add(toptag_forward_hist[i]);
  }
  
  chi2_0btag_central_hist[3]->SetTitle("Anti-b-tag X^{2}");
  chi2_0btag_central_hist[3]->SetMarkerStyle(20);
  comparison.loadHists(chi2_0btag_central_hist[3],"Central","PE");
  comparison.loadHists(chi2_0btag_forward_hist[3],"Forward","hist");
  comparison.plotHists(2,false);
  comparison.clearAll();

  chi2_1btag_central_hist[3]->SetTitle("b-tag X^{2}");
  chi2_1btag_central_hist[3]->SetMarkerStyle(20);
  comparison.loadHists(chi2_1btag_central_hist[3],"Central","PE");
  comparison.loadHists(chi2_1btag_forward_hist[3],"Forward","hist");
  comparison.plotHists(2,false);
  comparison.clearAll();
  
  chi2_2btag_central_hist[3]->SetTitle("2 b-tags X^{2}");
  chi2_2btag_central_hist[3]->SetMarkerStyle(20);
  comparison.loadHists(chi2_2btag_central_hist[3],"Central","PE");
  comparison.loadHists(chi2_2btag_forward_hist[3],"Forward","hist");
  comparison.plotHists(2,false);
  comparison.clearAll();


  toptag_central_hist[3]->SetTitle("Top-Tag");
  toptag_central_hist[3]->SetMarkerStyle(20);
  comparison.loadHists(toptag_central_hist[3],"Central","PE");
  comparison.loadHists(toptag_forward_hist[3],"Forward","hist");
  comparison.plotHists(2,false);
  comparison.clearAll();


  /*
  all[1]->SetTitle("hadronic Top");
  all[1]->SetMarkerStyle(1);
  test.loadHists(all[1],"B+b M(1500)","HIST");
  test.loadStackHists(top_unmatched_w_matched[1]," top_{unmatch} W_{match}");
  test.loadStackHists(unmatched[1],"unmatched");
  test.loadStackHists(top_matched_w_unmatched[1]," top_{match} W_{unmatch}");
  test.loadStackHists(matched[1],"matched");
  test.plotHists(2,false);
  test.clearAll();
  */

  
  //treehists.Draw("slimmedJets.m_eta","weight","100,-5,5","Ak4 jets #eta");
  
  //treehists.Draw("slimmedJets.m_pt","weight","50,50,1500","Ak4 jet p_{T} [GeV]");
  //treehists.Draw("numberofjets","weight","21,-0.5,20.5","Number of Ak4 jets");
  //treehists.Draw("Sum$(slimmedJets.m_pt > 25)","weight","21,-0.5,20.5","Number of Ak4 jets test lenght");

  //treehists.Draw("slimmedMuonsUSER.m_pt","weight","50,0,500","Muon p_{T} [GeV]");
  

  //to do, get muon plots, jet plots, electrons, MET, HTLep

  return 0;

  folder = "jersmear_direction_up_Sel_Mu";

  TreeHists treehistsJer_up("plots/Jer_up.ps");
  treehistsJer_up.SetTree("AnalysisTree");
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v31/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}",0.1);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets",0.1);

  treehistsJer_up.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehistsJer_up.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehistsJer_up.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehistsJer_up.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta >= 2.4)","50,50,3000","Mass B [GeV]");
  
  treehistsJer_up.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehistsJer_up.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehistsJer_up.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehistsJer_up.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta < 2.4)","50,50,3000","Mass B [GeV]");

  folder = "jersmear_direction_down_Sel_Mu";

  TreeHists treehistsJer_down("plots/Jer_down.ps");
  treehistsJer_down.SetTree("AnalysisTree");
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/Selection_v31/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");

  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTJets.root","",2,-1,true,"t#bar{t}",0.1);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets.root","",3,-1,true,"W+Jets",0.1);

  treehistsJer_down.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehistsJer_down.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehistsJer_down.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehistsJer_down.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta >= 2.4)","50,50,3000","Mass B [GeV]");
  
  treehistsJer_down.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==0)","50,50,3000","Mass B [GeV]");
  treehistsJer_down.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber ==1)","50,50,3000","Mass B [GeV]");
  treehistsJer_down.Draw("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta < 2.4 && Chi2Dis.btagEventNumber > 1)","50,50,3000","Mass B [GeV]");
  treehistsJer_down.Draw("TopTagDis.mass","weight*(TopTagDis.forwardJetAbsEta < 2.4)","50,50,3000","Mass B [GeV]");
  



  return 0;
}

// && slimmedJets.m_btag_combinedSecondaryVertex[] > 0.79
