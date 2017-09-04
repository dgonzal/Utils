#include "TreeHists.h"
#include "effiPlots.h"
#include "simplePlots.h"

#include <iostream>
#include <fstream>

bool draw_effi_withtreehists(TreeHists & treehistclass, effiPlots & efficlass, string command, string pre_weight, string post_weight, string binning, string x_axis, vector<double>rebin={}){
  treehistclass.Draw(command,pre_weight,binning,x_axis);
  treehistclass.Draw(command,post_weight,binning,x_axis);
  vector<TH1F*> pre  =  treehistclass.return_hists(command,pre_weight,binning,x_axis);
  vector<TH1F*> post =  treehistclass.return_hists(command,post_weight,binning,x_axis);

  TH1F* data_pre  = pre[0];
  TH1F* data_post = post[0];
    
  TH1F* mc_pre = pre[1];mc_pre->Add(pre[2]);
  TH1F* mc_post = post[1];mc_post->Add(post[2]);

  if(rebin.size()>0){
     data_pre = (TH1F*) data_pre->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
     data_post = (TH1F*) data_post->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
     mc_pre = (TH1F*) mc_pre->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
     mc_post = (TH1F*) mc_post->Rebin(rebin.size()-1,"", (double*) &rebin[0]);
  }
  efficlass.set_axisTitle(x_axis,"Efficency");
  efficlass.loadHists((TH1F*)data_pre->Clone(),(TH1F*)data_post->Clone(),"Data");
  efficlass.loadHists((TH1F*)mc_pre->Clone(),(TH1F*)mc_post->Clone(),"Simulation");
  efficlass.plotEffi(4,{make_pair(0,1)});
  efficlass.clear_histos();

  return true;
}


using namespace std;

int main(){

  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/EleTriggerScale/";
  string resultfile = "plots/elescalefactor/distribution";
  string effiresult = "plots/elescalefactor/effi";

  bool single = true;
  string binning ="40,20,820";
  string eta_binning="10,-2.4,2.4";
  bool errors = false;
  
  vector<double> rebin ={0,20,40,60,80,100,120,140,160,180,200,400,820};

  
  TreeHists treehists(resultfile,single);
  treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.DATA.Data.root","PE",1,20,false,"Data");
  treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.TTbar.root","",2,-1,true,"t#bar{t}");
  treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"single t");

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

  
  if(errors){
    treehists.AddErrorWeight("pdfWeight",TreeHists::error_method::rms);
    treehists.AddErrorWeight("scaleWeight_REPLACE",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_btag_REPLACE/weight_btag",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_pu_REPLACE/weight_pu",TreeHists::error_method::envelop,"up,down");   
    treehists.AddErrorWeight("weight_sfmu_tight_REPLACE/weight_sfmu_tight",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_sfmu_muonTrigger_REPLACE/weight_sfmu_muonTrigger",TreeHists::error_method::envelop,"up,down");
    treehists.AddErrorWeight("weight_sfelec_eleid_REPLACE/weight_sfelec_eleid",TreeHists::error_method::envelop,"up,down"); 
  }
   


  
  treehists.Draw("slimmedJets.slimmedJets.m_pt[0]","weight","70,30,900","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_eta[0]","weight","70,-5,5","ak4 #eta","Events");
  //treehists.Draw("slimmedJets.slimmedJets.m_phi[0]","weight","50,-3.14,3.14","ak4 #phi","Events",false);
  //treehists.Draw("Sum$(slimmedJets.slimmedJets.m_pt>50)","1.","11,-0.5,10.5","Number of ak4","Events",false);
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta[0]","weight","50,-2.4,2.4","electron #eta","Events");
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]","weight","70,40,500"," electron p_{T} [GeV]","Events");
  //treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_phi[0]","weight","50,-3.14,3.14"," electron #phi","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0]","weight","50,-2.4,2.4","muon #eta","Events");
  treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_pt[0]","weight","70,30,800"," muon p_{T} [GeV]","Events");
  //treehists.Draw("slimmedMuonsUSER.slimmedMuonsUSER.m_phi[0]","weight","50,-3.14,3.14"," muon #phi","Events");
 
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]","weight*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 || slimmedJets.slimmedJets.m_pt[0]>185)","50,20,520"," electron p_{T} [GeV]","Events");
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta[0]","weight*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 || slimmedJets.slimmedJets.m_pt[0]>185)","50,-2.4,2.4"," electron #eta","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_pt[0]","weight*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","70,0,800","ak4 p_{T} [GeV]","Events");
  treehists.Draw("slimmedJets.slimmedJets.m_eta[0]","weight*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","70,-5,5","ak4 #eta","Events");
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta[0]","weight*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","70,-5,5","electron #eta","Events");
  
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))",binning,"muon p_{T} [GeV]");
  treehists.Draw("slimmedElectronsUSER.slimmedElectronsUSER.m_eta[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))","50,-5,5","muon #eta");
  
  effiPlots EleTriggerScale(effiresult,single);
  EleTriggerScale.switch_legend();
  //EleTriggerScale.setRange(0,0,0.5,1.09);
  
  vector<TH1F*> jet_ele_cut =  treehists.return_hists("slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]","weight*(slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))",binning,"");
  vector<TH1F*> eletrigger_jet_ele_cut =  treehists.return_hists("slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))",binning,"");


  TH1F* data_pre  = jet_ele_cut[0];
  for (int i = 0; i < data_pre->GetNcells(); i++) {
    cout<<data_pre->GetBinLowEdge(i)<<", ";
  }
  cout<<endl;
  
  TH1F* data_post = eletrigger_jet_ele_cut[0];
  
  
  TH1F* mc_pre = jet_ele_cut[1];mc_pre->Add(jet_ele_cut[2]);
  TH1F* mc_post = eletrigger_jet_ele_cut[1];mc_post->Add(eletrigger_jet_ele_cut[2]);


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
 
  EleTriggerScale.set_axisTitle("electron p_{T}","Efficency");
  //Combi Trigger
  EleTriggerScale.set_val("slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]");
  EleTriggerScale.loadHists((TH1F*)data_pre->Clone(),(TH1F*)data_post->Clone(),"Data Combined Ele Trigger");
  EleTriggerScale.loadHists((TH1F*)mc_pre->Clone(),(TH1F*)mc_post->Clone(),"MC Combined Ele Trigger");
  EleTriggerScale.plotEffi(4,{make_pair(0,1)});
  string nominal = EleTriggerScale.get_nominal();

  ofstream myfile ("./include/eletriggerresult.h");
  myfile<<"#include <string>\n";
  myfile<<"\n";
  myfile<<"std::string eletriggerscale(int i=0){\n";
  myfile<<"if(i==0) return \""<<nominal << "\";\n";
  myfile<<"\n";
  myfile<<"else if(i==1) return\""<<EleTriggerScale.get_up() << "\";\n";
  myfile<<"\n";
  myfile<<"else if(i==2) return\""<<EleTriggerScale.get_down() << "\";\n";
  myfile<<"\n";
  myfile<<"else return \" Some error ocurred, this option is not valid for the ele scale factor\";\n";
  myfile<<"}";
  myfile.close();

  EleTriggerScale.clear_histos();
  
  draw_effi_withtreehists(treehists, EleTriggerScale,"slimmedElectronsUSER.slimmedElectronsUSER.m_eta[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))",eta_binning,"electron #eta" );
    
  draw_effi_withtreehists(treehists, EleTriggerScale,"slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","("+nominal+"*(1-isRealData)+isRealData)*weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))",binning,"postscale electron p_{T}",rebin);
  
  draw_effi_withtreehists(treehists, EleTriggerScale,"slimmedElectronsUSER.slimmedElectronsUSER.m_eta[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","("+nominal+"*(1-isRealData)+isRealData)*weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))",eta_binning,"postscale electron #eta" );

  
  draw_effi_withtreehists(treehists, EleTriggerScale,"slimmedJets.slimmedJets.m_pt[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","("+nominal+"*(1-isRealData)+isRealData)*weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))","50,100,600","postscale jet p_{T}");
  
  draw_effi_withtreehists(treehists, EleTriggerScale,"slimmedJets.slimmedJets.m_eta[0]","weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55))","("+nominal+"*(1-isRealData)+isRealData)*weight*((slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>120 && singleEletrigger) || (slimmedJets.slimmedJets.m_pt[0]>185 && slimmedElectronsUSER.slimmedElectronsUSER.m_pt[0]>55 && jetEletrigger))",eta_binning,"postscale jet #eta" );
  
  
  
  return 0;
}
