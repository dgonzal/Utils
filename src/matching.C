#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"

vector<double> signal_rebin(TH1F* hist, double percent);

int main(){
  string version = "wtag_topjetcorr";
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSel_"+version;//"jecsmear_direction_up_Sel";//"Selection_"+version;
  bool electron = true;
  if (electron)folder = "EleSel_cross";
  bool single = true;
  string output = "plots/matching/treeEle";
 
  //if(single) output = plots/treehists/ 
  
  TreeHists treehists(output,single);
  treehists.SetLegend(0.5, 0.4, 0.7, 0.86);
  TLatex *cms_text = new TLatex(3.5, 24," 35.8 fb^{-1} (13 TeV)"); //CMS Preliminary
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetX(0.9);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);
  cms_text->SetY(.95);
  treehists.addText(cms_text);
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-800_RH.root","hist",1,-1,false,"B+b M(800) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-800_RH.root","hist",2,-1,false,"B+t M(800) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1000_RH.root","hist",3,-1,false,"B+b M(1000) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1000_RH.root","hist",4,-1,false,"B+t M(1000) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeB-1500_RH.root","hist",5,-1,false,"B+b M(1500) RH");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.BprimeT-1500_RH.root","hist",6,-1,false,"B+t M(1500) RH");
  /*/
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.QCD.root","",4,-1,true,"QCD");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.SingleT.root","",41,-1,true,"Single Top");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.ZJets.root","",5,-1,true,"Z+Jets");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Tune.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt700to1000.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt1000toInf.root","",2,-1,true,"t#bar{t}");
  treehists.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.WJets_Pt.root","",3,-1,true,"W+Jets");
  /*/
  treehists.SetTree("AnalysisTree");
  treehists.switch_ratio(false);
  string binning = "50,0.2,3";
  vector<string> sample_nick ={"B+b M(800)","B+t M(800)","B+b M(1000)","B+t M(1000)","B+b M(1500)","B+t M(1500)","QCD","Singel Top","Z+Jets","ttbar","ttbar mid","ttbar high","W+Jets"};//"Data
  treehists.Draw(deltaRstring("Chi2Dis.bprime","BprimeGen.bprime"),"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400))",binning,"#Delta R(B_{gen},B_{reco})");
  treehists.Draw(deltaRstring("TopTagDis.bprime","BprimeGen.bprime"),"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400)",binning,"#Delta R(B_{gen},B_{reco})");

  
  //Chi2 categories
  std::vector<TH1F*> chi2_hist = treehists.return_hists(deltaRstring("Chi2Dis.bprime","BprimeGen.bprime"),"weight*((TopTagDis.mass==-1 || TopTagDis.topHad.pt()<400))",binning,"#Delta R(B_{gen},B_{reco}) ");
  
  //t-tag cat.
  std::vector<TH1F*> toptag_hist = treehists.return_hists(deltaRstring("TopTagDis.bprime","BprimeGen.bprime"),"weight*(TopTagDis.mass >0 && TopTagDis.topHad.pt()>400)",binning,"#Delta R(B_{gen},B_{reco}) ");

  
  simplePlots matching("plots/matching/effiEle",single);
  matching.switch_ratio(false);
  matching.setLegend(0.2, 0.6, 0.45, 0.86);
  
  matching.set_histYTitle("matched B [%]");
  matching.set_XTitle("#Delta R(B_{gen},B_{reco})");
  
  for(unsigned int i =0; i<chi2_hist.size();i++){
    TH1F* chi2 = (TH1F*) chi2_hist[i]->Clone();
    double chi2_sum = 0;
    for(int m=1;m<chi2->GetNcells()-1;++m){
      chi2_sum += chi2->GetBinContent(m);
      chi2->SetBinContent(m,chi2_sum);
      chi2->SetBinError(m,0);
    }
    chi2->SetTitle("");
    chi2->Scale(100/chi2_sum);
    matching.loadHists(chi2,sample_nick[i]);    
  }
  matching.plotHists(2,false);      
  matching.clearAll();
  for(unsigned int i =0; i<toptag_hist.size();i++){
    TH1F* top = (TH1F*) toptag_hist[i]->Clone();
    double top_sum =0;
    for(int m=1;m<top->GetNcells()-1;++m){
      top_sum += top->GetBinContent(m);
      top->SetBinContent(m,top_sum);
      top->SetBinError(m,0);
    }
    top->SetTitle("");
    top->Scale(100/top_sum);
    matching.loadHists(top,sample_nick[i],"H");

  } 
  matching.plotHists(2,false);      
  matching.clearAll();

  
  return 0;
}



