

#include "simplePlots.h"

using namespace std;




simplePlots::simplePlots(string saveName): HistsBase(saveName){
  normArea=false;
  legx1=0.6; legx2= 0.8; legy1=0.7; legy2=0.8;
  max=-1;
}

void simplePlots::loadHists(string histname){
  Long_t id, size, flag, modtime;
  for(const auto & fileDir : get_filedirs()){
    if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
      cerr<<fileDir.c_str()<<" does not exist"<<endl;
      return;
    }
    TFile* file = new TFile(fileDir.c_str());
    if(file->GetListOfKeys()->Contains(histname.c_str())!=0){
      cerr<<histname.c_str()<<" does not exist"<<endl;
      return;
    }
    TH1F * hist = (TH1F*) file->Get(histname.c_str());
    histos.push_back(hist);
    //cout<<"read Histo "<<histname.c_str()<<" from File "<<fileDir.c_str()<<endl;;
    delete file;
  }
  //cout<<"======================"<<endl;
}

void simplePlots::plotHists(int options, bool logy){
   
  leg = new TLegend(legx1, legy1, legx2, legy2);
  if(logy) get_can()->SetLogy();
  double maximum =0;
  for(unsigned int m = 0; m < histos.size(); ++m ){
    if(histos[m]->GetMaximum() > maximum) maximum = histos[m]->GetMaximum();
  }  
  if(max!=-1)maximum=max;
  //cout<<maximum<<" "<<max<<" "<<normArea<<" "<<histos.size()<<endl;
  for(unsigned int m = 0; m < histos.size(); ++m ){  
    histos[m]->SetLineColor(1+m);
    if(options==1 && legend.size()>m) leg->AddEntry( histos[m], legend[m].c_str(), "l");
    if(m==0){
      if(normArea){
	histos[m]->DrawNormalized()->SetMaximum(maximum);
      }
      else{
	histos[m]->SetMaximum(maximum*1.2);
	histos[m]->Draw();
      }
    }
    else{
      if(normArea)histos[m]->DrawNormalized("same"); 
      else histos[m]->Draw("same"); 
    }
    if(m+1== histos.size()){
      if(legend.size()>0&&options==1 )leg->Draw();
      get_can()->Print(get_resultFile());
    }
  }
  if(logy) get_can()->SetLogy(0);  
}

void simplePlots::loadTH2(string histname){
  for(const auto & fileDir : get_filedirs()){
    TFile* file = new TFile(fileDir.c_str());
    TH2F * hist = (TH2F*) file->Get(histname.c_str());
    twoDhists.push_back(hist);
  }
}

void simplePlots::plotTH2(int options){
  for(unsigned int m = 0; m < twoDhists.size(); ++m ){
    twoDhists[m]->SetLineColor(1+m);
    //twoDhists[m]->GetXaxis()->SetRangeUser(20,450);
    twoDhists[m]->Rebin2D(20,10);
    if(options==0){
      twoDhists[m]->Scale(1/twoDhists[m]->Integral()); 
      twoDhists[m]->Draw("colz"); 
    }
    get_can()->Print(get_resultFile());
  }
} 
