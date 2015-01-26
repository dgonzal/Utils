#include "simplePlots.h"

using namespace std;




simplePlots::simplePlots(string saveName){
  resultFile=saveName;
  TH1::AddDirectory(kFALSE);

  can = new TCanvas("can", "can", 600, 700); 
  can->cd();
  can->Print(resultFile+"[");
  
  legx1=0.6; legx2= 0.8; legy1=0.7; legy2=0.8;

  // general appearance and style
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->UseCurrentStyle();



}
simplePlots::~simplePlots(){
  can->Print(resultFile+"]");
  delete can;
}



void simplePlots::loadHists(string histname){
  
  for(auto fileDir : filedirs){
    TFile* file = new TFile(fileDir.c_str());

    TH1F * hist = (TH1F*) file->Get(histname.c_str());
    
    histos.push_back(hist);
    
    delete file;
  }
}


void simplePlots::plotHists(int options, bool logy){
   
  leg = new TLegend(legx1, legy1, legx2, legy2);
  
  if(logy) can->SetLogy();

  double maximum =0;

  for(unsigned int m = 0; m < histos.size(); ++m )
    if(histos[m]->GetMaximum() > maximum) maximum = histos[m]->GetMaximum();

  for(unsigned int m = 0; m < histos.size(); ++m ){
    histos[m]->SetLineColor(1+m);

    if(options==1 && legend.size()>m) leg->AddEntry( histos[m], legend[m].c_str(), "l");
    if(m==0){
      histos[m]->SetMaximum(maximum*1.2);
      histos[m]->Draw();
    }
    else histos[m]->Draw("same"); 
    
    if(m+1 >= histos.size()){
      if(legend.size()>0)leg->Draw();
      can->Print(resultFile);
    }
  }
  
  if(logy) can->SetLogy(0);

  
}

