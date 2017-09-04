#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(){
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_wtag_topjetcorr";
  bool single =false;
  string output = "plots/"+folder+".ps";
   
  TreeHists treehists(output);
  
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root","",3,-1,true,"W+Jets");

  folder = "EleSel_cross";
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root","",3,-1,true,"W+Jets");

  
  treehists.SetTree("AnalysisTree");

  string eta = "2.";
  string binning = "40,500,3000";

  string forward_chi2 ="abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string central_chi2 ="abs(Chi2Dis.forwardJet.eta()) < "+eta;


  
  //W-tag cat.
  std::vector<TH1F*> wtag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) >="+eta+")",binning,"Mass B [GeV]");
  std::vector<TH1F*> wtag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta())  <"+eta+")",binning,"Mass B [GeV]");

  //Chi2 categories
  std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");
  
  //t-tag cat.
  std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && abs(TopTagDis.forwardJet.eta())>="+eta+")",binning,"Mass B [GeV]");
  std::vector<TH1F*> toptag_central_hist = treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && abs(TopTagDis.forwardJet.eta())< "+eta+")",binning,"Mass B [GeV]");


  string fitoption = "UUNORM P";


  //vector<string> sample_nick ={"Data","B+b M(800)","B+t M(800)","B+b M(1000)","B+t M(1000)","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","ttbar mid","ttbar high","W+Jets"};//"Data",
  vector<string> sample_nick ={"Muon QCD","Muon Singel Top","Muon Z+Jets","Muon ttbar","Muon ttbar mid","Muon ttbar high","Muon W+Jets","Electron QCD","Electron Singel Top","Electron Z+Jets","Electron ttbar","Electron ttbar mid","Electron ttbar high","Electron W+Jets"};//"Data",

  vector<string> category = {"Anti-b-tag","1 b-tag","2 b-tags","t-tag","W-tag"};
  vector<vector<TH1F*>> central_hists = {chi2_0btag_central_hist,chi2_1btag_central_hist,chi2_2btag_central_hist,toptag_central_hist,wtag_central_hist};
  vector<vector<TH1F*>> forward_hists = {chi2_0btag_forward_hist,chi2_1btag_forward_hist,chi2_2btag_forward_hist,toptag_forward_hist,wtag_forward_hist};

  vector<TH1F*> sum_central;
  vector<TH1F*> sum_forward;

  for(auto item : central_hists)
    cout<<"central size "<<item.size()<<endl;
  for(auto item : forward_hists)
    cout<<"forward size "<<item.size()<<endl;

  output = "plots/combined_comparision_forward_central.ps";
  if(single) output = "plots/combined_signal_comp/comparision_forward_central";
  
  simplePlots comparison(output,single);
  comparison.set_ratioYTitle("Cen./For.");
  comparison.set_histYTitle("NA");
  comparison.set_XTitle("B mass [GeV]");
  comparison.normToArea(true,0.4);
  for(unsigned int i =0; i<central_hists.size();i++){
    string recotype = " X^2";
    if(i==3 || i==4) recotype = "";
    for(unsigned int m=0;m<central_hists.at(i).size();m++){
      if(central_hists.at(i).at(m)->GetSumw2N()==0)central_hists.at(i).at(m)->Sumw2();
      if(forward_hists.at(i).at(m)->GetSumw2N()==0)forward_hists.at(i).at(m)->Sumw2();
      if(m==0){
	sum_forward.push_back((TH1F*)forward_hists.at(i).at(m)->Clone(("Sum Forward"+category[i]).c_str()));
	sum_central.push_back((TH1F*)central_hists.at(i).at(m)->Clone(("Sum Central"+category[i]).c_str()));
      }
      else{
	sum_central[sum_central.size()-1]->Add(central_hists.at(i).at(m));
	sum_forward[sum_forward.size()-1]->Add(forward_hists.at(i).at(m));
      }
      cout<<"1"<<endl;
      cout<<sample_nick[m]<<endl;
      cout<<category[i]<<endl;
      string histTitel = sample_nick[m]+" "+category[i]+recotype;
      cout<<histTitel<<" ratio "<<forward_hists.at(i).at(m)->GetSumOfWeights()<< " / "<<central_hists.at(i).at(m)->GetSumOfWeights()<<" = " <<forward_hists.at(i).at(m)->GetSumOfWeights()/central_hists.at(i).at(m)->GetSumOfWeights()<<endl;
      //central_hists.at(i).at(m)->Chi2Test(forward_hists.at(i).at(m),fitoption.c_str());
      central_hists.at(i).at(m)->SetTitle(histTitel.c_str());
      central_hists.at(i).at(m)->SetMarkerStyle(20);
      //central_hists.at(i).at(m)->Scale(forward_hists.at(i).at(m)->GetSumOfWeights()/central_hists.at(i).at(m)->GetSumOfWeights());
      if(central_hists.at(i).at(m)->GetSumOfWeights()==0 || forward_hists.at(i).at(m)==0)continue;
      comparison.loadHists((TH1F*)central_hists.at(i).at(m)->Clone(),"Central","PE");
      comparison.loadHists((TH1F*)forward_hists.at(i).at(m)->Clone(),"Forward","PE");
      comparison.plotHists(2,false);      
      comparison.clearAll();
    }
  } 
  
  for(unsigned int i =0; i<sum_central.size();i++){
    sum_central[i]->SetTitle(category[i].c_str());
    sum_forward[i]->SetTitle(category[i].c_str());
   
    comparison.loadHists((TH1F*)sum_central[i]->Clone(),"Central","PE");
    comparison.loadHists((TH1F*)sum_forward[i]->Clone(),"Forward","PE");      
    comparison.plotHists(2,false);
    comparison.clearAll();
  
  }

 simplePlots check_add("checksum.ps");
 for(unsigned int i =0; i<sum_central.size();i++){  
    check_add.loadHists((TH1F*)sum_forward[i]->Clone(),"Forward","PE");
    check_add.loadHists((TH1F*)sum_central[i]->Clone(),"Central","PE");
    check_add.plotHists(2,false);
    check_add.clearAll();
  }

  for(unsigned int i =0; i<sum_central.size();i++){
    TH1F* forward_integral = (TH1F*)sum_forward[i]->Clone();
    TH1F* central_integral = (TH1F*)sum_central[i]->Clone();
    int cells = forward_integral->GetNcells();
    for(int m=0;m<cells;m++){
	double error=0;
        forward_integral->SetBinContent(m,forward_integral->IntegralAndError(m,cells,error));
        forward_integral->SetBinError(m,error);
        central_integral->SetBinContent(m,central_integral->IntegralAndError(m,cells,error));
        central_integral->SetBinError(m,error);
    }
    check_add.loadHists(forward_integral,"Forward Integral","PE");
    check_add.loadHists(central_integral,"Central Integral","PE");
    check_add.plotHists(2,false);
    check_add.clearAll();
   }
  for(unsigned int i =0; i<sum_central.size();i++){
    TH1F* forward = (TH1F*)sum_forward[i]->Clone();
    TH1F* central = (TH1F*)sum_central[i]->Clone();
    forward->Chi2Test(central,"P");

  }
    
  return 0;
}

