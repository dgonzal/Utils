#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(){
  string version = "v5";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_"+version;//"jecsmear_direction_up_Sel";//"Selection_"+version;
  bool electron = false;
  if (electron)folder = "EleSel_v1";


  TreeHists treehists("plots/"+folder+".ps");
  if(!electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root","PE",1,20,false,"Data");
  if(electron)treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleEleData.root","PE",1,20,false,"Data");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",4,-1,false,"B+b M(800)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",7,-1,false,"B+t M(800)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",4,-1,false,"B+b M(1000)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",7,-1,false,"B+t M(1000)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",4,-1,false,"B+b M(1500)");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",7,-1,false,"B+t M(1500)");

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}",0.1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root","",3,-1,true,"W+Jets",0.1);
 
  treehists.SetTree("AnalysisTree");
  /*
  treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
  treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_sfmu_mediumID_REPLACE/weight_sfmu_mediumID",TreeHists::error_method::envelop,"up,down");
  treehists.AddErrorWeight("weight_toptag_REPLACE",TreeHists::error_method::envelop,"up,down");
  
  treehists.AddErrorFolder({"/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jersmear_direction_up_Sel_Mu/","/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/jecsmear_direction_down_Sel_Mu/"});
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

  string eta = "2.";
  string energy = "250.0";
  string jetiso = "10000";
  string binning = "50,500,3000";


  string no_forward_chi2 = "";//"&& Chi2Dis.forwardJet.pt()>0";
  string no_forward_tag = "";//"&& TopTagDis.forwardJet.pt()>0";
  
  std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+")&&Chi2Dis.forwardJet.E()>="+energy+"&&Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((abs(Chi2Dis.forwardJet.eta())<"+eta+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((abs(Chi2Dis.forwardJet.eta())<"+eta+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1&&((abs(Chi2Dis.forwardJet.eta())<"+eta+")||Chi2Dis.forwardJet.E()<"+energy+")&&Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");

  std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0&&(abs(TopTagDis.forwardJet.eta())>="+eta+")&&TopTagDis.forwardJet.E()>="+energy+")",binning,"Mass B [GeV]");
  std::vector<TH1F*> toptag_central_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0 &&((abs(TopTagDis.forwardJet.eta())<"+eta+")||TopTagDis.forwardJet.E()<"+energy+"))",binning,"Mass B [GeV]");                            

  string fitoption = "UUNORM P";


  vector<string> sample_nick ={"Data","B+b M(800)","B+t M(800)","B+b M(1000)","B+t M(1000)","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","W+Jets"};
  vector<string> category = {"Anti-b-tag","1 b-tag","2 b-tags","TopTag" };
  vector<vector<TH1F*>> central_hists = {chi2_0btag_central_hist,chi2_1btag_central_hist,chi2_2btag_central_hist,toptag_central_hist};
  vector<vector<TH1F*>> forward_hists = {chi2_0btag_forward_hist,chi2_1btag_forward_hist,chi2_2btag_forward_hist,toptag_forward_hist};
  vector<TH1F*> sum_central;
  vector<TH1F*> sum_forward;


  string lepton_name = "Mu_";
  if(electron) lepton_name = "Ele_";
	
  simplePlots comparison("plots/"+lepton_name+"comparision_forward_central.ps");
  comparison.set_ratioYTitle("Cen./For.");
  comparison.set_histYTitle("NA");
  comparison.set_XTitle("B mass [GeV]");
  comparison.normToArea(true,0.4);
  for(unsigned int i =0; i<central_hists.size();i++){
    string recotype = " X^2";
    if(i==3) recotype = "";
    for(unsigned int m=0;m<central_hists.at(i).size();m++){
      if(central_hists.at(i).at(m)->GetSumw2N()==0)central_hists.at(i).at(m)->Sumw2();
      if(forward_hists.at(i).at(m)->GetSumw2N()==0)forward_hists.at(i).at(m)->Sumw2();
      if(m==7){
	sum_forward.push_back((TH1F*)forward_hists.at(i).at(m)->Clone(("Sum Forward"+category[i]).c_str()));
	sum_central.push_back((TH1F*)central_hists.at(i).at(m)->Clone(("Sum Central"+category[i]).c_str()));
      }
      else if(m>7){
	sum_central[sum_central.size()-1]->Add(central_hists.at(i).at(m));
	sum_forward[sum_forward.size()-1]->Add(forward_hists.at(i).at(m));
      }
      string histTitel = sample_nick[m]+" "+category[i]+recotype;
      cout<<histTitel<<" ratio "<<forward_hists.at(i).at(m)->GetEntries()<< " / "<<central_hists.at(i).at(m)->GetEntries()<<" = " <<forward_hists.at(i).at(m)->GetEntries()/central_hists.at(i).at(m)->GetEntries()<<endl;
      //central_hists.at(i).at(m)->Chi2Test(forward_hists.at(i).at(m),fitoption.c_str());
      central_hists.at(i).at(m)->SetTitle(histTitel.c_str());
      central_hists.at(i).at(m)->SetMarkerStyle(20);
      //central_hists.at(i).at(m)->Scale(forward_hists.at(i).at(m)->GetEntries()/central_hists.at(i).at(m)->GetEntries());
      if(central_hists.at(i).at(m)->GetEntries()==0 || forward_hists.at(i).at(m)==0)continue;
      comparison.loadHists((TH1F*)central_hists.at(i).at(m)->Clone(),"Central","PE");
      comparison.loadHists((TH1F*)forward_hists.at(i).at(m)->Clone(),"Forward","PE");
      comparison.plotHists(2,false);
      comparison.clearAll();
    }
  }

  for(unsigned int i =0; i<sum_central.size();i++){
    sum_central[i]->SetTitle(category[i].c_str());
    sum_forward[i]->SetTitle(category[i].c_str());
    cout<<"Forward/Central "<<sum_central[i]->GetTitle()<<" ratio "<<sum_forward[i]->GetEntries() <<" / "<<sum_central[i]->GetEntries()<<" "<<sum_forward[i]->GetEntries()/sum_central[i]->GetEntries()<<endl;
    comparison.loadHists((TH1F*)sum_central[i]->Clone(),"Central","PE");
    comparison.loadHists((TH1F*)sum_forward[i]->Clone(),"Forward","PE");
    comparison.plotHists(2,false);
    comparison.clearAll();
  }

  //do rebining since from 2000-3000 stats are rather low!
  unsigned int number_bins = chi2_0btag_forward_hist[0]->GetNbinsX();
  double low_edge = chi2_0btag_forward_hist[0]->GetBinLowEdge(1);
  double bin_width = chi2_0btag_forward_hist[0]->GetBinWidth(1);
  double high_edge = chi2_0btag_forward_hist[0]->GetBinLowEdge(number_bins)+chi2_0btag_forward_hist[0]->GetBinWidth(1);
  vector<double> bins;
  for(unsigned int i = 0; i<number_bins;++i){
    if(low_edge+i*bin_width < 1800)
      bins.push_back(low_edge+i*bin_width);
    else{
      bins.push_back(low_edge+i*bin_width);
      break;
    }
  }
  bins.push_back(high_edge);
  cout<<"NBins "<<number_bins<<" low "<<low_edge<<" high "<<high_edge<<" width "<<bin_width<<endl;
  cout<<"[";
  for(auto i : bins)
    cout<<" "<<i<<",";
  cout<<'\b'<<"]"<<endl;

  // [800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
  // B+b: 3.016,2.219,1.653,1.192,0.896,0.679, 0.529, 0.415, 0.319,0.249,0.195
  // B+t: 0.365,0.271,0.203,0.152,0.116,0.0894,0.0692,0.0540
  vector<double> signal_crosssection =  {3.016,0.365,1.653,0.203,0.415,0.0540};
  simplePlots signal_contamination("plots/"+lepton_name+"signal_contamination.ps");
  signal_contamination.set_histYTitle("Signal/Background [%]");
  signal_contamination.set_XTitle("B mass [GeV]");
  signal_contamination.switch_ratio(false);
  for(unsigned int m=0; m<central_hists.size();m++){
    for(unsigned int i=1;i<7;i++){
      central_hists.at(m).at(i)->SetTitle(("Central "+category[m]).c_str());
      central_hists.at(m).at(i)->Scale(signal_crosssection[i]*0.5*100);
      sum_central[m] = (TH1F*) sum_central[m]->Rebin(bins.size()-1,"rebin_sum_central", (double*) &bins[0]);
      central_hists.at(m).at(i) =(TH1F*) central_hists.at(m).at(i)->Rebin(bins.size()-1,"rebin_sum_central", (double*) &bins[0]);
      cout<<"=========================================="<<endl;
      cout<<central_hists.at(m).at(i)->GetTitle()<<endl;
      cout<<"Total S/B: "<<central_hists.at(m).at(i)->GetEntries()/sum_central[m]->GetEntries()<<endl;
      central_hists.at(m).at(i)->Divide(sum_central[m]);
      central_hists.at(m).at(i)->GetXaxis()->SetTitle("B mass [GeV]");
      central_hists.at(m).at(i)->GetXaxis()->SetTitleOffset(1.2);
      //central_hists.at(m).at(i)->GetYaxis()->SetTitleOffset(1.2);
      //central_hists.at(m).at(i)->GetYaxis()->SetTitle("Signal/Background [%]");
      string hist_option = "hist same";
      if(i==1) hist_option ="hist";
      signal_contamination.loadHists((TH1F*)central_hists.at(m).at(i)->Clone(),sample_nick[i],hist_option);
    }
    signal_contamination.plotHists(2,false);
    signal_contamination.clearAll();
  }
  for(unsigned int m=0; m<forward_hists.size();m++){
    for(unsigned int i=1;i<7;i++){
      forward_hists.at(m).at(i)->SetTitle(("Forward "+category[m]).c_str());
      forward_hists.at(m).at(i)->Scale(signal_crosssection[i]*0.5*100);
      cout<<"=========================================="<<endl;
      cout<<forward_hists.at(m).at(i)->GetTitle()<<endl;
      cout<<"Total S/B: "<<forward_hists.at(m).at(i)->GetEntries()/sum_forward[m]->GetEntries()<<endl;
      sum_forward[m] = (TH1F*) sum_forward[m]->Rebin(bins.size()-1,"rebin_sum_central", (double*) &bins[0]);
      forward_hists.at(m).at(i) =(TH1F*) forward_hists.at(m).at(i)->Rebin(bins.size()-1,"rebin_sum_central", (double*) &bins[0]);
      forward_hists.at(m).at(i)->Divide(sum_forward[m]);
      //forward_hists.at(m).at(i)->GetYaxis()->SetTitle("Signal/Background [%]");
      forward_hists.at(m).at(i)->GetXaxis()->SetTitle("B mass [GeV]");
      forward_hists.at(m).at(i)->GetXaxis()->SetTitleOffset(1.2);
      //forward_hists.at(m).at(i)->GetYaxis()->SetTitleOffset(1.2);
      string hist_option = "hist same";
      if(i==1) hist_option ="hist";
      signal_contamination.loadHists((TH1F*)forward_hists.at(m).at(i)->Clone(),sample_nick[i],hist_option);
    }
    signal_contamination.plotHists(2,false);
    signal_contamination.clearAll();
  }

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
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/Selection_v31/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",4,-1,false,"B+b M(1500)");
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",7,-1,false,"B+t M(1500)");

  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}",0.1);
  treehistsJer_up.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root","",3,-1,true,"W+Jets",0.1);

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
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/Selection_v31/uhh2.AnalysisModuleRunner.DATA.SingleMuData.root","PE",1,20,false,"Data");
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",4,-1,false,"B+b M(1500)");
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",7,-1,false,"B+t M(1500)");

  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD",0.5);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top",0.1);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets",0.1);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}",0.1);
  treehistsJer_down.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT.root","",3,-1,true,"W+Jets",0.1);

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
