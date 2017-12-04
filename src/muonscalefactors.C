#include "TreeHists.h"
#include "effiPlots.h"
#include "simplePlots.h"
#include "Utils.h"
#include "eletriggerresult.h"

#include <iostream>
#include <fstream>


using namespace std;

int main(){

  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/MuTriggerMeasurement";
  string resultfile = "plots/muonscalefactor/distribution.ps";
  string effiresult = "plots/muonscalefactor/effi.ps";

  bool single = false;
  string binning ="40,40,800";
  string eta_binning="10,-2.4,2.4";
  bool errors = false;
  string factors = "("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  //factors = "(((fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<0.2)*0.97+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=0.2 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<1)*0.98+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=1 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<1.6)*0.967+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=1.6 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<1.8)*0.983+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=1.8 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<2.2)*0.974+ (fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=2.2)*0.956)*(1-isRealData)+isRealData)*";
  cout<<"factors applied "<<factors<<endl;
  
  vector<double> rebin ={20,40,60,80,100,120,140,160,180,200,400,820};

  
  TreeHists treehists(resultfile,single);
  //treehists.addFile(dir+"/highpt.root","PE",1,20,false,"hight p_{T} muon ID");
  //treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/MuTriggerMeasurement/tight_id.root","",2,-1,true,"tight muon ID");
  treehists.addFile(dir+"_highpt_v2/uhh2.AnalysisModuleRunner.DATA.Data.root","PE",1,20,false,"hight p_{T} muon ID");
  treehists.addFile(dir+"_tight_v2/uhh2.AnalysisModuleRunner.DATA.Data.root","",2,-1,true,"tight muon ID");
  //treehists.addFile(dir+"_highpt_v2/MC.root","PE",1,20,false,"hight p_{T} muon ID");
  //treehists.addFile(dir+"_tight_v2/MC.root","",2,-1,true,"tight muon ID");
 /*/ 
  treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.DATA.Data.root","PE",1,20,false,"Data");
  treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.TTbar*.root","",2,-1,true,"t#bar{t}");
  treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.SingleT*.root","",41,-1,true,"single t");
  //treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.ZJets*.root","",5,-1,true,"Z+Jets");
  //treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.WJets*.root","",3,-1,true,"W+Jets");
  /*/

  treehists.set_ratiolimits(1.69,0.39);
  treehists.SetTree("AnalysisTree");
  
  TLatex *lumi_text = new TLatex(3.5, 24,"35.8 fb^{-1} (13 TeV)"); //CMS Preliminary
  lumi_text->SetNDC();
  lumi_text->SetTextAlign(33);
  lumi_text->SetX(0.9);
  lumi_text->SetTextFont(42);
  lumi_text->SetTextSize(0.04);
  lumi_text->SetY(.95);
  treehists.addText(lumi_text);
  TLatex *preliminary_text = new TLatex(3.5, 24,"CMS #it{Preliminary}");
  preliminary_text->SetNDC();
  preliminary_text->SetTextAlign(33);
  preliminary_text->SetX(0.5);
  preliminary_text->SetTextFont(42);
  preliminary_text->SetTextSize(0.045);
  preliminary_text->SetY(.95);
  treehists.addText(preliminary_text);

  treehists.Draw("slimmedMuonsUSER.slimmedElectronsUSER.m_eta[0]",factors+"weight","50,-2.4,2.4","electron #eta","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedElectronsUSER.m_pt[0]",factors+"weight","50,40,500"," electron p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_pt[0]",factors+"weight","50,30,900","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_eta[0]",factors+"weight","50,-5,5","ak4 #eta","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0]",factors+"weight","50,-2.4,2.4","muon #eta","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight","50,40,500"," muon p_{T} [GeV]","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0]",factors+"weight","50,-2.4,2.4","muon #eta","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight","50,30,800"," muon p_{T} [GeV]","Events");
 
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*(slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && muontrigger)","50,20,820","muon p_{T} [GeV]","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0]",factors+"weight*(slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && muontrigger)","50,-2.4,2.4"," muon #eta","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_pt[0]",factors+"weight*(slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && muontrigger)","50,0,800","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_eta[0]",factors+"weight*(slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && muontrigger)","50,-5,5","ak4 #eta","Events");
  
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*(slimmedMETs.m_pt>200 && muonCombitrigger)","50,20,820","muon p_{T} [GeV]","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*((slimmedMETs.m_pt>200 && muonCombitrigger) || (slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && muontrigger))","50,20,820","muon p_{T} [GeV]","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*((slimmedMETs.m_pt>200 && muonCombitrigger) && !muontrigger)","50,20,820","muon p_{T} [GeV]","Events");

  //treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]","weight*((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>120 && muontrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && jetMutrigger))",binning,"muon p_{T} [GeV]");
  //treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0]","weight*((slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>120 && singleMutrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]>55 && jetMutrigger))","50,-5,5","muon #eta");

 
  effiPlots MuTriggerScale(effiresult,single);
  MuTriggerScale.switch_legend();
  //MuTriggerScale.setRange(0,0,0.5,1.09);
  
  vector<TH1F*> mu_cut =  treehists.return_hists("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight",binning,"");
  
  vector<TH1F*> mu_tight =  treehists.return_hists("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*(tightmuon)",binning,"");//tightmuon
  vector<TH1F*> mu_highpt =  treehists.return_hists("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*(highptmuon)",binning,"");//tightmuon

  vector<TH1F*> mutrigger_tightid =  treehists.return_hists("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*(muontrigger && tightmuon)",binning,"");//tightmuon
  vector<TH1F*> mutrigger_highpt  =  treehists.return_hists("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]",factors+"weight*(muontrigger && highptmuon)",binning,"");//tightmuon
   
  TH1F* data_pre  = (TH1F*)mu_tight[1]->Clone();
  TH1F* data_post = (TH1F*)mutrigger_tightid[1]->Clone();
  
  
  TH1F* mc_pre  = (TH1F*)mu_highpt[0]->Clone();
  TH1F* mc_post = (TH1F*)mutrigger_highpt[0]->Clone();  //mutrigger_tightid[1];//mutrigger_tightid[1];


  data_pre = (TH1F*)data_pre->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
  data_post = (TH1F*)data_post->Rebin(rebin.size()-1,"", (double*) &rebin[0]);

  mc_pre = (TH1F*)mc_pre->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
  mc_post = (TH1F*)mc_post->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
  

  
  TH1F* ratio = (TH1F*)data_pre->Clone();
  ratio->Divide(data_post); ratio->Multiply(mc_post);ratio->Divide(mc_pre);
  
  
  simplePlots comparison("test.ps");
  comparison.set_XTitle("B mass [GeV]");
  //comparison.normToArea(true,0.4);
  comparison.loadHists((TH1F*)data_pre->Clone(),"Data","PE");
  comparison.loadHists((TH1F*)mc_pre->Clone(),"Background","");      
  comparison.plotHists(2);
  comparison.clearAll();
  comparison.loadHists((TH1F*)data_post->Clone(),"Data","PE");
  comparison.loadHists((TH1F*)mc_post->Clone(),"Background","");      
  comparison.plotHists(2);
  comparison.clearAll();
  comparison.loadHists((TH1F*)data_pre->Clone(),"Data pre","PE");
  comparison.loadHists((TH1F*)data_post->Clone(),"Data post","");      
  comparison.plotHists(2);
  comparison.clearAll();
  comparison.loadHists((TH1F*)mc_pre->Clone(),"MC pre","PE");
  comparison.loadHists((TH1F*)mc_post->Clone(),"MC post","");    
  comparison.plotHists(2);
  comparison.clearAll();
 
  MuTriggerScale.set_axisTitle("muon p_{T} [GeV]","Efficency");
  //MuTriggerScale.setRange(40,800,0.8,1.1);
  MuTriggerScale.set_val("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]");
  //MuTriggerScale.loadHists((TH1F*)data_pre->Clone(),(TH1F*)data_post->Clone(),"simulation muon hight p_{T} ID");
  //MuTriggerScale.loadHists((TH1F*)mc_pre->Clone(),(TH1F*)mc_post->Clone(),"simulation muon tight ID");
  //MuTriggerScale.loadHists((TH1F*)data_pre->Clone(),(TH1F*)data_post->Clone(),"data muon hight p_{T} ID");
  //MuTriggerScale.loadHists((TH1F*)mc_pre->Clone(),(TH1F*)mc_post->Clone(),"data muon hight p_{T} ID");
  MuTriggerScale.loadHists((TH1F*)data_pre->Clone(),(TH1F*)data_post->Clone(),"high p_{T} muon ID simulation");
  MuTriggerScale.loadHists((TH1F*)mc_pre->Clone(),(TH1F*)mc_post->Clone(),"tight muon ID simulation");
  MuTriggerScale.plotEffi(4,{make_pair(0,1)});
  string nominal = MuTriggerScale.get_nominal();

  cout<<"muon scale factors"<<endl;
  cout<<"up ===================="<<endl;
  cout<<MuTriggerScale.get_up()<<endl;
  cout<<"down =================="<<endl;
  cout<<MuTriggerScale.get_down()<<endl;


  ofstream myfile ("./include/muontriggerresult.h");
  myfile<<"#include <string>\n";
  myfile<<"\n";
  myfile<<"std::string eletriggerscale(int i=0){\n";
  myfile<<"if(i==0) return \""<<nominal << "\";\n";
  myfile<<"\n";
  myfile<<"else if(i==1) return\""<<MuTriggerScale.get_up() << "\";\n";
  myfile<<"\n";
  myfile<<"else if(i==2) return\""<<MuTriggerScale.get_down() << "\";\n";
  myfile<<"\n";
  myfile<<"else return \" Some error ocurred, this option is not valid for the ele scale factor\";\n";
  myfile<<"}";
  myfile.close();

  MuTriggerScale.clear_histos();
 
  
  return 0;
}
