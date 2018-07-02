#include <iostream>
#include <fstream>

#include "TreeHists.h"
#include "simplePlots.h"

#include <boost/algorithm/string/replace.hpp>
#include "boost/algorithm/string.hpp"

#include "TArrayD.h"
#include "TFile.h"

#include "eletriggerresult.h"
#include "forwardjetfitresult.h"
#include "mutrkfactors.h"

vector<double> signal_rebin(TH1F* hist, double percent);




class category_container{
public:  
  double ratio_c(){return signal_central/background_central;}
  double ratio_f(){return signal_forward/background_forward;}
  double ratio_estimate(){return ratio_c()/ratio_f()*background_forward/background_central;}

  void print_to_file(std::string filename){
    ofstream printfile;
    printfile.open (filename, std::ios::app);
    printfile << category<<" & "<< signal_forward <<" & "<<background_forward<<" & "<<ratio_f()<<" & "<<category<<" & "<< signal_central<<" & "<<background_central<< " & "<<ratio_c()<< " & "<< ratio_c()/ratio_f()*background_forward/background_central<<" \\\\ % "<<" signal cross section "<< signal_cross_section<<" "<<nick <<" "<<nick2 <<" "<<category2<<endl;
    printfile.close();
  }
  
  double signal_central;
  double background_central;
  double signal_forward;
  double background_forward;

  double signal_cross_section;
  std::string category, nick;
  std::string nick2,category2;//testing if everything went right
  vector<double>  rebin;
};



int main(){
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_new";
  bool electron = false;
  if (electron)folder = "EleSel_new";
  bool single = true;
  string output = "plots/"+folder+".ps";
  bool sqrt_back = false;
  
  //if(single) output = plots/treehists/ 
  string loose = "0.5426";

  
  TreeHists treehists(output); 
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/Merge_Data.root","PE",1,20,false,"Data");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",4,-1,false,"B+b M(800) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",8,-1,false,"B+t M(800) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",9,-1,false,"B+b M(1000) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",10,-1,false,"B+t M(1000) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",11,-1,false,"B+b M(1500) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",12,-1,false,"B+t M(1500) RH");
 
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD*.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_*.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt*.root","",3,-1,true,"W+Jets");
  
  treehists.SetTree("AnalysisTree");
  treehists.mcratio_only();
  string eta = "2.4";
  string binning = "20,0,3500";

  string forward_chi2 ="abs(Chi2Dis.forwardJet.eta()) >="+eta;//+" && Chi2Dis.chi<0.08";
  string central_chi2 ="abs(Chi2Dis.forwardJet.eta()) < "+eta;//+" && Chi2Dis.chi<0.08";

  string eletriggerfactors = "";
  if(electron)  eletriggerfactors ="("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  string muontrkfactors =""; 
  if(!electron) muontrkfactors = "("+muon_trk_factors()+"*(1-isRealData)+isRealData)*";
  string factors = muontrkfactors+eletriggerfactors+"(abs(Chi2Dis.forwardJet.eta())<=4)*";//+forwardfit("TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400");
  //factors += "(Chi2Dis.forwardJet.pt()>0)*";
  

  cout<<"applying factors: "<<factors<<endl;

  cout<<"Added all files. Ready to start working"<<endl; 
  /*/
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass>0 && "+forward_chi2+")",binning,"Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass>0 && "+central_chi2+")",binning,"Mass B [GeV]");
 
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");
  
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  treehists.Draw("Chi2Dis.mass",factors+"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400) && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");
  treehists.Draw("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && "+forward_chi2+")",binning,"Mass B [GeV]");
  treehists.Draw("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && "+central_chi2+")",binning,"Mass B [GeV]");
  /*/

  string exclude_toptag = "TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400"; // || TopTagDis.btag_discriminand < "+loose;
  
  //std::vector<TH1F*> wtag_forward_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass>0 && "+forward_chi2+")",binning,"Mass B [GeV]");
  std::vector<TH1F*> wtag_forward_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass>0 && "+forward_chi2+" && Chi2Dis.btagEventNumber>0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> wtag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass>0 && "+central_chi2+")",binning,"Mass B [GeV]");
  
  /*/
  //W-tag cat. WTag
  std::vector<TH1F*> wtag_forward_hist = treehists.return_hists("WTagDis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta()) >="+eta+")",binning,"Mass B [GeV]");
  std::vector<TH1F*> wtag_central_hist = treehists.return_hists("WTagDis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass>0 && abs(Chi2Dis.forwardJet.eta())  <"+eta+")",binning,"Mass B [GeV]");
  /*/
  //Chi2 categories
  std::vector<TH1F*> chi2_0btag_forward_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_forward_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_forward_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass==-1 && "+forward_chi2+" && Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");
  
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight*(("+exclude_toptag+") && WTagDis.mass==-1 && "+central_chi2+" && Chi2Dis.btagEventNumber> 1)",binning,"Mass B [GeV]");

  /*/
  std::vector<TH1F*> chi2_0btag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight/weight_btag*weight_btag_loose*(("+exclude_toptag+" ) && WTagDis.mass==-1 && "+central_chi2+" && numberofloosebjets==0)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_1btag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight/weight_btag*weight_btag_loose*(("+exclude_toptag+") && WTagDis.mass==-1 && "+central_chi2+" && numberofloosebjets==1)",binning,"Mass B [GeV]");
  std::vector<TH1F*> chi2_2btag_central_hist = treehists.return_hists("Chi2Dis.mass",factors+"weight/weight_btag*weight_btag_loose*(("+exclude_toptag+") && WTagDis.mass==-1 && "+central_chi2+" && numberofloosebjets> 1)",binning,"Mass B [GeV]");
  /*/
  //t-tag cat.
  std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && "+forward_chi2+")",binning,"Mass B [GeV]");
  //std::vector<TH1F*> toptag_forward_hist = treehists.return_hists("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && TopTagDis.btag_discriminand > "+loose +" && "+ forward_chi2+")",binning,"Mass B [GeV]");
  std::vector<TH1F*> toptag_central_hist = treehists.return_hists("TopTagDis.mass",factors+"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400 && "+central_chi2+")",binning,"Mass B [GeV]");

  cout<<"Done with the histogram production starting to add and check stuff"<<endl;

  string fitoption = "UUNORM P";

  //vector<string> sample_nick ={"Data","B+b M(800)","B+t M(800)","B+b M(1000)","B+t M(1000)","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","ttbar mid","ttbar high","W+Jets"};//"Data",
  //vector<string> sample_nick ={"Data","B+b M(800)","B+t M(800)","B+b M(1000)","B+t M(1000)","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","ttbar mid","ttbar high","W+Jets"};//"Data",
  vector<string> sample_nick = treehists.get_nicknames();
  cout<<"sample: ";
  int start_add_backgrounds = 0;
  bool found_start = false;
  vector<unsigned int> sample_switch ={};
  for(unsigned int i=0; i<sample_nick.size();i++){
    if(sample_nick[i].empty()) sample_nick[i]= sample_nick[i-1];
    else sample_switch.push_back(i);
    if(!found_start && !boost::algorithm::contains(sample_nick[i], "B+") && !boost::algorithm::contains(sample_nick[i], "Data")){
      start_add_backgrounds = i;
      found_start = true;
    }
   
    cout<<sample_nick[i]<<", ";
  }
  cout<<endl;
  sample_switch.push_back(sample_nick.size());
  for(auto item : sample_switch){
    cout<<item<<endl;
  }
  cout<<"start adding background at "<<start_add_backgrounds<<endl;
  
  //vector<string> category = {"Anti-b-tag","1 b-tag","2 b-tags","t-tag","W-tag"};
  vector<string> category = {"Anti_btag","1_btag","2_btags","ttag","Wtag"};

  vector<vector<TH1F*>> central_hists = {chi2_0btag_central_hist,chi2_1btag_central_hist,chi2_2btag_central_hist,toptag_central_hist,wtag_central_hist};
  vector<vector<TH1F*>> forward_hists = {chi2_0btag_forward_hist,chi2_1btag_forward_hist,chi2_2btag_forward_hist,toptag_forward_hist,wtag_forward_hist};

  vector<TH1F*> sum_central;
  vector<TH1F*> sum_forward;

  for(auto item : central_hists)
    cout<<"central size "<<item.size()<<endl;
  for(auto item : forward_hists)
    cout<<"forward size "<<item.size()<<endl;

  string lepton_name = "Mu_";
  if(electron) lepton_name = "Ele_";

  output = "plots/"+lepton_name+"comparision_forward_central.ps";
  if(single) output = "plots/"+lepton_name+"signal_comp/comparision_forward_central";
  
  simplePlots comparison(output,single);
  comparison.set_ratioYTitle("Cen./For.");
  comparison.set_histYTitle("normalized to area");
  comparison.set_XTitle("m_{reco} [GeV]");
  comparison.normToArea(true,0.4);
  comparison.set_ratioLimtis(0.4,1.9);
  TH1F* background_sum_all_categories;

  string root_file = "muon_hists.root";
  if(electron) root_file = "electron_hists.root";
  TFile* file = new TFile(root_file.c_str(),"RECREATE");
   
  for(unsigned int i =0; i<central_hists.size();i++){
    unsigned int sample_counter = 0;
    string recotype = " X^2";
    if(i==3 || i==4) recotype = "";
    TH1F* sample_sum_forward, *sample_sum_central;
    for(unsigned int m=0;m<central_hists.at(i).size();m++){
      if(boost::contains(sample_nick[m],"Data")){
	file->cd();
	TH1F* my_tmp_central =  (TH1F*)central_hists.at(i).at(m)->Clone((sample_nick[m]+"__"+category[i]+"_central").c_str());
	TH1F* my_tmp_forward =  (TH1F*)forward_hists.at(i).at(m)->Clone((sample_nick[m]+"__"+category[i]+"_forward").c_str());
	my_tmp_central->SetTitle("m_{reco} [GeV]");
	my_tmp_forward->SetTitle("m_{reco} [GeV]");	  
	my_tmp_central->Write();
	my_tmp_forward->Write();
      }
      if(central_hists.at(i).at(m)->GetSumw2N()==0)central_hists.at(i).at(m)->Sumw2();
      if(forward_hists.at(i).at(m)->GetSumw2N()==0)forward_hists.at(i).at(m)->Sumw2();
      if(i==0 && m==start_add_backgrounds){
	background_sum_all_categories = (TH1F*)central_hists.at(i).at(m)->Clone(("Sum Central"+category[i]).c_str());
	background_sum_all_categories->SetTitle("Sum all categories");
      }
      else if(i!=3 && m>start_add_backgrounds) background_sum_all_categories->Add((TH1F*)central_hists.at(i).at(m)->Clone(("Sum Central"+category[i]).c_str()));
      
      if(m==start_add_backgrounds){
	sum_forward.push_back((TH1F*)forward_hists.at(i).at(m)->Clone(("Sum Forward"+category[i]).c_str()));
	sum_central.push_back((TH1F*)central_hists.at(i).at(m)->Clone(("Sum Central"+category[i]).c_str()));
      }
      else if(m>start_add_backgrounds){
	sum_central[sum_central.size()-1]->Add(central_hists.at(i).at(m));
	sum_forward[sum_forward.size()-1]->Add(forward_hists.at(i).at(m));
      }
      if(m == sample_switch[sample_counter]){
	sample_sum_forward = (TH1F*)forward_hists.at(i).at(m)->Clone();
	sample_sum_central = (TH1F*)central_hists.at(i).at(m)->Clone();
	string histTitel = sample_nick[m]+" "+category[i]+recotype;
	sample_sum_central->SetTitle(histTitel.c_str());
	sample_sum_central->SetMarkerStyle(20);
      }
      else if(m> sample_switch[sample_counter]){
	sample_sum_forward->Add(forward_hists.at(i).at(m));
	sample_sum_central->Add(central_hists.at(i).at(m));
      }
      if(m+1==sample_switch[sample_counter+1]){
	sample_counter++;
	if(sample_sum_central->GetSumOfWeights()==0 || sample_sum_forward->GetSumOfWeights()==0)continue;
	comparison.loadHists((TH1F*) sample_sum_central->Clone(),"Central","PE");
	comparison.loadHists((TH1F*) sample_sum_forward->Clone(),"Forward","PE");
        cout<< sample_nick[m]+" "+category[i]+recotype+" central "<<sample_sum_central->GetSumOfWeights()<<" forward "<<sample_sum_forward->GetSumOfWeights()<<endl;
	comparison.plotHists(2,false);      
	comparison.clearAll();
      }
    }
  } 
  
  for(unsigned int i =0; i<sum_central.size();i++){
    file->cd();
    TH1F* my_tmp_central =  (TH1F*)sum_central.at(i)->Clone(("MC_merge__"+category[i]+"_central").c_str());
    TH1F* my_tmp_forward =  (TH1F*)sum_forward.at(i)->Clone(("MC_merge__"+category[i]+"_forward").c_str());
    my_tmp_central->SetTitle("m_{reco} [GeV]");
    my_tmp_forward->SetTitle("m_{reco} [GeV]");	  
    my_tmp_central->Write();
    my_tmp_forward->Write();
    
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

  //return 0;
  //do rebining since from ~2000-3000 stats are rather low!
  unsigned int number_bins = chi2_0btag_forward_hist[0]->GetNbinsX();
  double low_edge = chi2_0btag_forward_hist[0]->GetBinLowEdge(1);
  double bin_width = chi2_0btag_forward_hist[0]->GetBinWidth(1);
  double high_edge = chi2_0btag_forward_hist[0]->GetBinLowEdge(number_bins)+chi2_0btag_forward_hist[0]->GetBinWidth(1);
  vector<double> bins;
  for(unsigned int i = 0; i<number_bins;++i){
    if(low_edge+i*bin_width < 1700)
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

  bins.erase(bins.begin()+1);
  
  // [800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800]
  // B+b: 3.016,2.219,1.653,1.192,0.896,0.679, 0.529, 0.415, 0.319,0.249,0.195
  // B+t: 0.365,0.271,0.203,0.152,0.116,0.0894,0.0692,0.0540
  vector<double> signal_crosssection =  {3.016,0.365,1.653,0.203,0.415,0.0540};

  
  output = "plots/"+lepton_name+"signal_contamination.ps";
  if(sqrt_back)  output = "plots/"+lepton_name+"signal_contamination_sqrt.ps";
  if(single){
    output = "plots/"+lepton_name+"signal_comp/signal_contamination";
    //if(sqrt_back) output += "_sqrt";
    //output +="/";
  }
  double signalfraction = 0.8;

  
  simplePlots signal_contamination(output,single);
  signal_contamination.set_histYTitle("S/B");
  if(sqrt_back) signal_contamination.set_histYTitle("S/#sqrt{B}");
  signal_contamination.set_XTitle("m_{reco} [GeV]");
  signal_contamination.switch_ratio(false);


  ofstream events_mass_windows_file;
  events_mass_windows_file.open ("events_mass_windows.txt");
  events_mass_windows_file<< "File contains the number of signal and background events in a mass windows of "+to_string(signalfraction*100)+" and the signal cross section.\n";

  vector<category_container> event_numbers;
  
  events_mass_windows_file<<endl;
  events_mass_windows_file<<"Going to the Forward region"<<endl;
  events_mass_windows_file<<"=========================================================" <<endl;

  for(unsigned int m=0; m<forward_hists.size();m++){
    events_mass_windows_file <<endl;
    for(unsigned int i=1;i<7;i++){
      category_container event_cat;
      
      forward_hists.at(m).at(i)->SetTitle(("Forward "+category[m]).c_str());
      TH1F* tmp_forward = (TH1F*)forward_hists.at(m).at(i)->Clone();
      if(i%2==0){
	tmp_forward->Scale(signal_crosssection[i-1]);
	cout<<"cross section "<<sample_nick[i]<<" "<<signal_crosssection[i-1]<<endl;
      }
      else{
	tmp_forward->Scale(signal_crosssection[i-1]*0.5*0.5);
	cout<<"cross section "<<sample_nick[i]<<" "<<signal_crosssection[i-1]*0.5<<endl;
      }
      vector<double> signalbinning  = signal_rebin(tmp_forward,signalfraction);
      TH1F* tmp_sum = (TH1F*)sum_forward[m]->Clone();      
      //tmp_sum = (TH1F*) tmp_sum->Rebin(signalbinning.size()-1,"rebin_sum_forward", (double*) &signalbinning[0]);
      //tmp_forward =(TH1F*) tmp_forward->Rebin(signalbinning.size()-1,"rebin_sum_forward", (double*) &signalbinning[0]);
      //forward_hists.at(m).at(i)->GetYaxis()->SetTitle("Signal/Background [%]");
      TH1F* tmp_forward_rebinned = (TH1F*) tmp_forward->Rebin(signalbinning.size()-1,"rebin_sum_central", (double*) &signalbinning[0]);
      int maxbin = tmp_forward_rebinned->GetMaximumBin();
      double maxbinwidth = tmp_forward_rebinned->GetBinWidth(maxbin);
      double maxbinlowedge = tmp_forward_rebinned->GetBinLowEdge(maxbin);
      double back_sum = 0;
      double back_error = 0; 
      for(int np=0;np<tmp_forward->GetNcells();++np){
	if(tmp_forward->GetBinCenter(np)> maxbinlowedge && tmp_forward->GetBinCenter(np) <= maxbinlowedge+maxbinwidth){
	  tmp_forward->SetBinContent(np,tmp_forward_rebinned->GetBinContent(maxbin));
	  tmp_forward->SetBinContent(np,tmp_forward_rebinned->GetBinContent(maxbin));
	  back_sum += tmp_sum->GetBinContent(np);
	  back_error += tmp_sum->GetBinError(np)*tmp_sum->GetBinError(np);
	}
	else{
	  tmp_forward->SetBinContent(np,0);
	  tmp_forward->SetBinError(np,0);
	}
      }
      double cross_section = -1;
      if(i%2==0) cross_section=signal_crosssection[i-1];
      else cross_section=signal_crosssection[i-1]*0.5;
      events_mass_windows_file <<sample_nick[i]<<" signal cross section "<<cross_section<<" " << category[m]<<" Forward Signal: "<<tmp_forward_rebinned->GetBinContent(maxbin)<<" Background "<<back_sum<<" ratio S/B "<< tmp_forward_rebinned->GetBinContent(maxbin)/back_sum <<endl;;
      cout<<category[m]+" Forward Signal: "<<tmp_forward_rebinned->GetBinContent(maxbin)<<" Background "<<back_sum<<endl;
      back_error = sqrt(back_error);

     
      event_cat.signal_forward=tmp_forward_rebinned->GetBinContent(maxbin);
      event_cat.background_forward=back_sum;
      event_cat.category=category[m];
      event_cat.nick=sample_nick[i];
      event_cat.rebin = signalbinning;
      event_cat.signal_cross_section = cross_section;
      event_numbers.push_back(event_cat);
 
      for(int np=0;np<tmp_forward->GetNcells();++np){
	tmp_sum->SetBinContent(np,back_sum);
	tmp_sum->SetBinError(np,back_error);
      }
      tmp_forward->GetXaxis()->SetTitle("m_{reco} [GeV]");
      tmp_forward->GetXaxis()->SetTitleOffset(1.2);
      if(sqrt_back){
	for(int b=0; b< tmp_sum->GetNcells();++b)
	  tmp_sum->SetBinContent(b,sqrt(tmp_sum->GetBinContent(b)));
	tmp_forward->GetYaxis()->SetTitle("S/#sqrt{B}");
      }
      tmp_forward->Divide(tmp_sum);
      string hist_option = "hist same";
      if(i==1) hist_option ="hist";
      signal_contamination.loadHists(tmp_forward,sample_nick[i],hist_option);
      //signal_contamination.plotHists(2,false);
      //signal_contamination.clearAll();
    }
    signal_contamination.plotHists(2,false);
    signal_contamination.clearAll();
  }
  
  events_mass_windows_file<<endl;
  events_mass_windows_file<<"Going to the Central region"<<endl;
  events_mass_windows_file<<"=========================================================" <<endl;
  for(unsigned int m=0; m<central_hists.size();m++){
    events_mass_windows_file <<endl;
    for(unsigned int i=1;i<7;i++){
      central_hists.at(m).at(i)->SetTitle(("Central "+category[m]).c_str());
      TH1F* tmp_central = (TH1F*)central_hists.at(m).at(i)->Clone();
      if(i%2==0){
	tmp_central->Scale(signal_crosssection[i-1]);
	cout<<"cross section "<<sample_nick[i]<<" "<<signal_crosssection[i-1]<<endl;
      }
      else{
	tmp_central->Scale(signal_crosssection[i-1]*0.5*0.5);
	cout<<"cross section "<<sample_nick[i]<<" "<<signal_crosssection[i-1]*0.5<<endl;
      }
      int counter = 0;
      for(auto cat : event_numbers)
	if(cat.category ==category[m] && cat.nick==sample_nick[i]) break;
	else counter++;
      
      vector<double> signalbinning = event_numbers[counter].rebin;//signal_rebin(tmp_central,signalfraction);     
      TH1F* tmp_sum = (TH1F*)sum_central[m]->Clone();
      //tmp_sum = (TH1F*) tmp_sum->Rebin(signalbinning.size()-1,"rebin_sum_central", (double*) &signalbinning[0]);
      TH1F* tmp_central_rebinned = (TH1F*) tmp_central->Rebin(signalbinning.size()-1,"rebin_sum_central", (double*) &signalbinning[0]);
      int maxbin = tmp_central_rebinned->GetMaximumBin();
      double maxbinwidth = tmp_central_rebinned->GetBinWidth(maxbin);
      double maxbinlowedge = tmp_central_rebinned->GetBinLowEdge(maxbin);
      double back_sum = 0;
      double back_error = 0; 
      for(int np=0;np<tmp_central->GetNcells();++np){
	if(tmp_central->GetBinCenter(np)> maxbinlowedge && tmp_central->GetBinCenter(np) <= maxbinlowedge+maxbinwidth){
	  tmp_central->SetBinContent(np,tmp_central_rebinned->GetBinContent(maxbin));
	  tmp_central->SetBinContent(np,tmp_central_rebinned->GetBinContent(maxbin));
	  back_sum += tmp_sum->GetBinContent(np);
	  back_error += tmp_sum->GetBinError(np)*tmp_sum->GetBinError(np);
	}
	else{
	  tmp_central->SetBinContent(np,0);
	  tmp_central->SetBinError(np,0);
	}
      }
      double cross_section = -1;
      if(i%2==0) cross_section=signal_crosssection[i-1];
      else cross_section=signal_crosssection[i-1]*0.5;
      events_mass_windows_file <<sample_nick[i]<<" signal cross section "<<cross_section <<" "<< category[m]<<" Central Signal: "<<tmp_central_rebinned->GetBinContent(maxbin)<<" Background "<<back_sum<<" ratio S/B "<< tmp_central_rebinned->GetBinContent(maxbin)/back_sum <<endl; 
      cout<<category[m]+" Central Signal: "<<tmp_central_rebinned->GetBinContent(maxbin)<<" Background "<<back_sum<<endl;
      back_error = sqrt(back_error);
      
      event_numbers[counter].signal_central=tmp_central_rebinned->GetBinContent(maxbin);
      event_numbers[counter].background_central=back_sum;
      event_numbers[counter].category2=category[m];
      event_numbers[counter].nick2=sample_nick[i];
      
      for(int np=0;np<tmp_central->GetNcells();++np){
	tmp_sum->SetBinContent(np,back_sum);
	tmp_sum->SetBinError(np,back_error);
      }

      
      tmp_central->GetXaxis()->SetTitle("m_{reco} [GeV]");
      tmp_central->GetXaxis()->SetTitleOffset(1.2);
      tmp_central->GetYaxis()->SetTitleOffset(1.2);
      tmp_central->GetYaxis()->SetTitle("S/B");

      if(sqrt_back){
	for(int b=0; b< tmp_sum->GetNcells();++b)
	  tmp_sum->SetBinContent(b,sqrt(tmp_sum->GetBinContent(b)+tmp_central->GetBinContent(b) ));
	tmp_central->GetYaxis()->SetTitle("S/#sqrt{S+B}");
      }
      tmp_central->Divide(tmp_sum);
      string hist_option = "hist same";
      if(i==1){
	hist_option ="hist";
	//signal_contamination.loadHists((TH1F*)sum_central[m]->Clone(),"MC Sum",hist_option);
	//hist_option = "hist same";
      }
      signal_contamination.loadHists(tmp_central,sample_nick[i],hist_option);
      
      //signal_contamination.plotHists(2,false);
      //signal_contamination.clearAll();
    }
    signal_contamination.plotHists(2,false);
    signal_contamination.clearAll();
  }

  /*/
  for(unsigned int m=0; m<forward_hists.size();m++)
    for(unsigned int i=1;i<7;i++){

      cout<<forward_hists.at(m).at(i)->GetTitle()<<" Signal Central "<< sample_nick[i]<<": "<<central_hists.at(m).at(i)->GetSumOfWeights()<<" Forward "<<forward_hists.at(m).at(i)->GetSumOfWeights()<<endl;
      cout<<forward_hists.at(m).at(i)->GetTitle()<<" Background Central "<< sample_nick[i]<<": "<<sum_central[m]->GetSumOfWeights()<<" Forward "<<sum_forward[m]->GetSumOfWeights()<<endl;

      cout<<forward_hists.at(m).at(i)->GetTitle()<<" Signal ratio Central/Forward "<< sample_nick[i]<<": "<<central_hists.at(m).at(i)->GetSumOfWeights()/forward_hists.at(m).at(i)->GetSumOfWeights()<<endl;
      cout<<forward_hists.at(m).at(i)->GetTitle()<<" S/B ratio Central/Forward "<< sample_nick[i]<<": "<<(central_hists.at(m).at(i)->GetSumOfWeights()/sum_central[m]->GetSumOfWeights())/(forward_hists.at(m).at(i)->GetSumOfWeights()/sum_forward[m]->GetSumOfWeights())<<endl;
    }
  /*/
  events_mass_windows_file.close();
  
  ofstream events_mass_windows_texfile;
  std::string texfilename = "tex_tamplate_mass_window.txt";
  if(electron) texfilename = "Ele"+texfilename;
  else texfilename = "Mu"+texfilename;    
  events_mass_windows_texfile.open(texfilename);
  events_mass_windows_texfile<< "File contains the number of signal and background events in a mass windows of "+to_string(signalfraction*100)<<endl;;
  events_mass_windows_texfile<< "Forward & $S$ & $B$ & ratio $S/B$ & Central & $S$ & $B$ & ratio $S/B$ & $(S/B)_{central} / (S/B)_{forward} * B_{f}/B_{c})$"<<endl;
  events_mass_windows_texfile.close();
  for(auto cat :event_numbers)
    cat.print_to_file(texfilename);

  
  return 0;
}

vector<double> signal_rebin(TH1F* hist, double percent){
  int bins = hist->GetNcells()-1;
  int maxbin = -1;
  double total_sig=0, maxsignal=-1;;
  vector<double> binning;
  for(int i=1;i<bins;++i){
    double content = hist->GetBinContent(i);
    total_sig+=content;
    if(content> maxsignal){
      maxbin=i;
      maxsignal=content;
    }
    if(i==1)binning.push_back(hist->GetBinLowEdge(i));
    binning.push_back(binning[i-1]+hist->GetBinWidth(i));
  }
  bool keep_going=true;
  while(keep_going){
    maxsignal =-1;
    maxbin =-1;
    for(int i=1;i<(int)binning.size()+1;++i){
      double content = hist->GetBinContent(i);
      if(content/total_sig>=percent){
	cout<<"[";
	for(auto m : binning)
	  cout<<" "<<m<<",";
	cout<<'\b'<<"]"<<endl;

	cout<<"at least "<<percent<<" in bin "<<i<<" "<<content<<" / "<<total_sig<<endl;

	return binning;
      }
      if(content> maxsignal){
	maxbin=i;
	maxsignal=content;
      }
    }
    if(maxbin==1)
      binning.erase(binning.begin()+1);
    else if(maxbin==(int)binning.size())
      binning.erase(binning.begin()+binning.size()-1);
    else
      hist->GetBinContent(maxbin-1) > hist->GetBinContent(maxbin+1) ? binning.erase(binning.begin()+maxbin-1) : binning.erase(binning.begin()+maxbin);
    hist = (TH1F*) hist->Rebin(binning.size()-1,"rebin_signal", (double*) &binning[0]);
  }
  
    
  return vector<double>();
}



