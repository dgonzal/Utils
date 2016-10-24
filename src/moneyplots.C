#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"


int main(){
   string version = "v38";
   string CMSSW = "7_6_3";
   string folder = "Selection_"+version;
   double scaling_factor = 0.363;
   double uncer_scaling = 1;
   string binning = "80,100,2500";

   TreeHists treehists("plots/"+folder+".ps");

   treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.DATA.SingleMuDATA.root","PE",1,20,false,"Data");
   treehists.SetTree("AnalysisTree");

   treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_800_RH_25ns.root","hist",4,-1,false,"B+b M(800)");
   treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_800_RH_25ns.root","hist",7,-1,false,"B+t M(800)");
   treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpb_TW_1500_RH_25ns.root","hist",4,-1,false,"B+b M(1500)");
   treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.Bpt_TW_1500_RH_25ns.root","hist",7,-1,false,"B+t M(1500)");


   std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta <  2.4 && Chi2Dis.btagEventNumber ==0)",binning,"Mass B [GeV]");
   std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==0)",binning,"Mass B [GeV]");
   std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta <  2.4 && Chi2Dis.btagEventNumber ==1)",binning,"Mass B [GeV]");
   std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==1)",binning,"Mass B [GeV]");
   std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta <  2.4 && Chi2Dis.btagEventNumber ==2)",binning,"Mass B [GeV]");
   std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass","weight*(TopTagDis.mass==-1 && Chi2Dis.forwardJetAbsEta >= 2.4 && Chi2Dis.btagEventNumber ==2)",binning,"Mass B [GeV]");
   std::vector<TH1F*> toptag_forward_hist =     treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass > -1 && Chi2Dis.forwardJetAbsEta >=  2.4)",binning,"Mass B [GeV]");
   std::vector<TH1F*> toptag_central_hist =     treehists.return_hists("TopTagDis.mass","weight*(TopTagDis.mass > -1 && Chi2Dis.forwardJetAbsEta <   2.4)",binning,"Mass B [GeV]");

  
   vector<string> signals = {"B+b M(800)","B+t M(800)","B+b M(1500)","B+t M(1500)"};
   vector<string> category = {"Anti-b-tag","1 b-tag","2 b-tags","TopTag" };
   vector<vector<TH1F*>> central_hists = {chi2_0btag_central_hist,chi2_1btag_central_hist,chi2_2btag_central_hist,toptag_central_hist};
   vector<vector<TH1F*>> forward_hists = {chi2_0btag_forward_hist,chi2_1btag_forward_hist,chi2_2btag_forward_hist,toptag_forward_hist};

   simplePlots comparison("plots/moneyplots.ps");
   comparison.change_colors(false);
   for(unsigned int i =0; i<central_hists.size();i++){
    for(unsigned int m=0;m<forward_hists.at(i).size();m++){
      if(m==0){
	central_hists.at(i).at(m)->SetTitle(category[i].c_str());
	central_hists.at(i).at(m)->Scale(scaling_factor);
	central_hists.at(i).at(m)->SetFillColor(29);
	central_hists.at(i).at(m)->SetLineColor(29);
	for(int p = 0; p< central_hists.at(i).at(m)->GetSize();p++){
	  double nom_binerror = central_hists.at(i).at(m)->GetBinError(p);
	  double new_error = nom_binerror*(1+uncer_scaling);
	  central_hists.at(i).at(m)->SetBinError(p,new_error);
	}
	comparison.loadHists(central_hists.at(i).at(m),"Background","hist");
	forward_hists.at(i).at(m)->SetMarkerStyle(20);
	forward_hists.at(i).at(m)->SetLineColor(1);
	comparison.loadHists(forward_hists.at(i).at(m),"DATA","same PE");
	
	TH1F* errorbands = (TH1F*)central_hists.at(i).at(m)->Clone("error_bands");
	errorbands->SetFillColor(TColor::GetColor("#aaaaaa"));
	errorbands->SetLineWidth(1);
	errorbands->SetFillStyle(3245);
	comparison.loadHists(errorbands,"","same e2",false);
      }
      else{
	forward_hists.at(i).at(m)->SetLineColor(m);
	comparison.loadHists(forward_hists.at(i).at(m),signals[m-1],"same hist",false);
      }
    }
    comparison.plotHists(2,false);
    comparison.clearAll();
  }

  return 0;
}
