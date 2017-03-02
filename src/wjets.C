#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"
#include "TH1F.h"
#include "TGraphErrors.h"


int main(){
  string CMSSW = "8_0_24_patch1";
  string folder = "WJetsFitGenHT";
  //folder = "MuSel_v5";
  string resultfile = "plots/wjets_pt.ps";

  TreeHists treehists(resultfile);
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_comp.root","PE0",    1,20,false,"W+Jets"           ,0,1);

  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt600toInf.root","",  12,-1,true,"W+Jets p_{T}>600" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt400to600.root","",  9,-1,true,"W+Jets 400<p_{T}<600" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt250to400.root","",  8,-1,true,"W+Jets 250<p_{T}<400" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt100to250.root","",  6,-1,true,"W+Jets 100<p_{T}<250" ,0,1);
  /*
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT2500toInf.root","",  12,-1,true,"W+Jets HT2500toInf" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT1200to2500.root","",  9,-1,true,"W+Jets HT1200to2500",0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT800to1200.root","",   8,-1,true,"W+Jets HT800to1200" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT600to800.root","",    6,-1,true,"W+Jets HT600to800" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT400to600.root","",    5,-1,true,"W+Jets HT400to600" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT200to400.root","",    4,-1,true,"W+Jets HT200to400" ,0,1);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_HT100to200.root","",    3,-1,true,"W+Jets HT100to200" ,0,1);
  */
 
  treehists.SetTree("AnalysisTree");

  //treehists.Draw("GenHT","weight","500,100,5000","H_{T,gen} [GeV]");
  //treehists.Draw("GenHT","weight","500,500,5000","H_{T,gen} [GeV]");
  //treehists.Draw("GenHT","weight","500,1500,5000","H_{T,gen} [GeV]");
  //treehists.Draw("GenHT","weight","500,3000,5000","H_{T,gen} [GeV]");
  //treehists.Draw("GenParticles.m_pt","weight * (abs(GenParticles.m_pdgId)==24)","75,10,2000","p_{T,W} [GeV]");

  //treehists.Draw("TMath::Log(GenHT)","weight","500,0,5000","H_{T,gen} [GeV]"); 
  treehists.switch_logy(true);
  //treehists.Draw("GenHT","weight","500,100,5000","H_{T,gen} [GeV]");
  //treehists.Draw("GenHT","weight","500,0,5000","H_{T,gen} [GeV]");
  treehists.Draw("GenParticles.m_pt","weight * (abs(GenParticles.m_pdgId)==24)","75,10,2000","p_{T,W} [GeV]");
  //vector<TH1F*> hist = treehists.return_hists("GenHT","weight","800,100,6000","H_{T,gen} [GeV]","");

  return 0;

  vector<TH1F*> hist = treehists.return_hists("GenParticles.m_pt","weight * (abs(GenParticles.m_pdgId)==24)","75,150,2000","p_{T,W} [GeV]");

  unsigned int number_bins = hist[0]->GetNbinsX();
  double low_edge = hist[0]->GetBinLowEdge(1);
  double bin_width = hist[0]->GetBinWidth(1);
  double high_edge = hist[0]->GetBinLowEdge(number_bins)+hist[0]->GetBinWidth(1);
  vector<double> bins;
  for(unsigned int i = 0; i<number_bins;++i) bins.push_back(low_edge+i*bin_width);
  bins.push_back(high_edge);
  cout<<"NBins "<<number_bins<<" low "<<low_edge<<" high "<<high_edge<<" width "<<bin_width<<endl;
  cout<<"[";
  for(auto i : bins)
    cout<<" "<<i<<",";
  cout<<'\b'<<"]"<<endl;


  bool done = false;
  int counter =0;
  while(!done && counter < 1000 && bins.size()>3){
    counter ++;
    bool no_change = true;
    for(unsigned i=0;i<bins.size()-1;++i){
      unsigned int m = bins.size()-2-i;
      if(hist[0]->GetBinContent(m) == 0 || hist[0]->GetBinError(m) > hist[0]->GetBinContent(m)*0.4){
	if(i==0){
	  bins.erase(bins.end()-2);
	  hist[0]= (TH1F*) hist[0]->Rebin(bins.size()-1,"rebin", (double*) &bins[0]);
	  no_change = false;
	  break;
	}
	else if(i==bins.size()-2){
	  bins.erase(bins.begin()+1);
	  hist[0]= (TH1F*) hist[0]->Rebin(bins.size()-1,"rebin", (double*) &bins[0]);
	  no_change = false;
	  break;
	}
	else{
	  if(hist[0]->GetBinError(m-1) > hist[0]->GetBinError(m+1)){
	    bins.erase(bins.begin()+m);
	    hist[0]= (TH1F*) hist[0]->Rebin(bins.size()-1,"rebin", (double*) &bins[0]);
	    no_change = false;
	    break;
	  }
	  else{
	    hist[0]= (TH1F*) hist[0]->Rebin(bins.size()-1,"rebin", (double*) &bins[0]);
	    no_change = false;
	    break;
	  }
	} 
      }
    }
    if(no_change) done=true;
  }

  cout<<"NBins "<< hist[0]->GetNbinsX() << " low "<< hist[0]->GetBinLowEdge(1)  <<" high "<< hist[0]->GetBinLowEdge(hist[0]->GetNbinsX())+hist[0]->GetBinWidth(hist[0]->GetNbinsX())<<endl;
  cout<<"[";
  for(auto i : bins)
    cout<<" "<<i<<",";
  cout<<'\b'<<"]"<<endl;


  for(unsigned int i =2; i<hist.size();++i){
    hist[1]->Add(hist[i]);
  }
  hist[1]= (TH1F*) hist[1]->Rebin(bins.size()-1,"ht_rebin", (double*) &bins[0]);

  TH1F* ratio = (TH1F*) hist[0]->Clone("ratio");
  ratio->Divide(hist[1]);

  cout<<"======================================"<<endl;
  cout<<"ratio bin +- error values "<<endl;
  cout<<"[";
  for(int i=0;i<ratio->GetNbinsX();++i)
    cout<<" "<<ratio->GetBinContent(i+1)<<" +- "<< ratio->GetBinError(i+1)<<",";
  cout<<'\b'<<"]"<<endl;

  cout<<"======================================"<<endl;
  cout<<"ratio bin values "<<endl;
  cout<<"{";
  for(int i=0;i<ratio->GetNbinsX();++i)
    cout<<" "<<ratio->GetBinContent(i+1)<<",";
  cout<<'\b'<<"}"<<endl;
  cout<<"======================================"<<endl;
  cout<<"ratio error values "<<endl;
  cout<<"{";
  for(int i=0;i<ratio->GetNbinsX();++i)
    cout<<" "<< ratio->GetBinError(i+1)<<",";
  cout<<'\b'<<"}"<<endl;



  TGraphErrors* grerr = new TGraphErrors(ratio);
  TF1 * line = new TF1("line","pol1"); 
  grerr->Fit(line,"IMP","");

  
  simplePlots wjetsfit("plots/wjetsfit.ps");
  wjetsfit.set_ratioYTitle(""); 
  wjetsfit.set_title(" ");
  wjetsfit.loadHists(ratio,"Ratio","");
  wjetsfit.plotHists(2,false);
  wjetsfit.clearAll();
  
  //wjetsfit.loadTGraph(grerr,"ratio","");
  wjetsfit.loadTF1(line,"Fit","");
  wjetsfit.plotHists(2,false);
  wjetsfit.clearAll();
  wjetsfit.loadHists(hist[0],"W+Jets","Pe");
  wjetsfit.loadHists(hist[1],"W+Jets HT","samePE");
  wjetsfit.plotHists(2,true);
  wjetsfit.clearAll();

  return 0;

  // fit to a log normal 
  /*
  TF1 * f1 = new TF1("f1","exp([0]+[1]*x+[2]*x*x)" ); 
  //TF1 * f1 = new TF1("f1","expo"); 
  double p[3]; 
  //p[0] = hist[0]->GetBinCenter( hist[0]->GetMaximumBin());
  p[0] = 7.4;
  p[1] = -0.016;
  p[2] = 0;
  f1->SetParameters(p); 
  f1->SetParName(0,"const");
  f1->SetParName(1,"alpha");
  f1->SetParName(2,"beta");
  */
  
  //TF1 * f1 = new TF1("f1","[0]*ROOT::Math::lognormal_pdf(x,[1],[2])*exp([3]*TMath::Log(x))*exp([4]*TMath::Log(x*x*x))*exp([5]*TMath::Log(x*x*x*x)) " ); 
  //TF1 * f1 = new TF1("f1","[0]*ROOT::Math::lognormal_pdf(x,[1],[2])*exp([3]*TMath::Log(x))" ); 


  /*
  TF1 * f1 = new TF1("f1","[0]*ROOT::Math::lognormal_pdf(x,[1],[2],[3])" ); 
  // set initial parameters 
  double p[4]; 
  p[0] = hist[0]->GetEntries()*hist[0]->GetXaxis()->GetBinWidth(1);   // area of the histogram 
   
  // find median of histogram 
  double prob[] = {0.5}; 
  double q[1]; 
  hist[0]->GetQuantiles(1,q,prob);
  double median = q[0];
  // find mode of histogram 
  double  mode = hist[0]->GetBinCenter( hist[0]->GetMaximumBin());
  
  std::cout << "histogram mode is " << mode  << " median is " << median << std::endl;
  
  if (median < 0) { 
    cerr<<"lognormal: not valid histogram median"<<endl;
    return 1;
  }
  
  // m is log(median)
  p[2] = std::log(median); 

  // s2 is  log(median) - log(mode) 
  p[1] = std::sqrt( std::log(median/mode) ); 
  //additional free parameters
  p[3] = 1;
  //p[4] = 0;
  //p[5] = 0;

  f1->SetParameters(p); 
  f1->SetParName(0,"A");
  f1->SetParName(1,"sigma");
  f1->SetParName(2,"theta");
  f1->SetParName(3,"m");
  //f1->SetParName(4,"m");
  //f1->SetParName(5,"epsilon");

  f1->Print();
  

  simplePlots wjetsfit("plots/wjetsfit.ps");
  hist[0]->Fit(f1,"IMP","");
  hist[1]->Fit(f1,"IMP","");
  hist[0]->Fit(f1,"IMP","");
  wjetsfit.loadHists(hist[0],"W+Jets","");
  wjetsfit.plotHists(2,true);
  wjetsfit.clearAll();

  hist[1]->Fit(f1,"IMP","");
  wjetsfit.loadHists(hist[1],"W+Jets H_{T} Binned ","");
  wjetsfit.plotHists(2,true);
  wjetsfit.clearAll();

  return 0;
  */
  
}
