#include "simplePlots.h"

using namespace std;


simplePlots::simplePlots(string saveName): HistsBase(saveName){
  normArea=false;
  legx1=0.6; legx2= 0.8; legy1=0.7; legy2=0.8;
  max=-1;
  stack = new THStack("hs","stacked histograms");
}
void simplePlots::loadHists(TH1F * hist){
  histos.push_back(hist);
}

void simplePlots::loadHists(string histname, string title){
  Long_t id, size, flag, modtime;
  for(const auto & fileDir : get_filedirs()){
    if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
      cerr<<"File "<<fileDir.c_str()<<" does not exist"<<endl;
      return;
    }
    TFile* file = new TFile(fileDir.c_str());
    if(file->IsZombie()){
      cerr<<"File "<<fileDir<<" is a Zombie"<<endl;
      return;
    }
    vector<string> histkeyname;
    if(histname.find("/") != std::string::npos){
      boost::split(histkeyname,histname,boost::is_any_of("/"));
    }
    else
      histkeyname.push_back(histname);

    if(file->GetListOfKeys()->Contains(histkeyname[0].c_str())==0){
      cerr<<"Histogram "<<histkeyname[0].c_str()<<" does not exist"<<endl;
      cerr<<"List of Keys: "<<endl;
      for(int i = 0; i< file->GetListOfKeys()->GetSize();i++)
	cerr<<file->GetListOfKeys()->At(i)->GetName()<<endl;
      return;
    }
    TH1F * hist = (TH1F*) file->Get(histname.c_str());
    if(!title.empty())
      hist->SetTitle(title.c_str());
    histos.push_back(hist);
    //cout<<"read Histo "<<histname.c_str()<<" from File "<<fileDir.c_str()<<endl;;
    delete file;
  }
  //cout<<"======================"<<endl;
}
void simplePlots::loadStackHists(string histname, int color){
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
    hist->SetFillStyle(1001);
    hist->SetFillColor(color);
    stack->Add(hist);
    //cout<<"read Histo "<<histname.c_str()<<" from File "<<fileDir.c_str()<<endl;;
    delete file;
  }
  //cout<<"======================"<<endl;
}

void simplePlots::loadStackHists(TH1F * hist){
    hist->SetFillStyle(1001);
    stack->Add(hist);
}

void simplePlots::plotHists(int options, bool logy){
  leg = new TLegend(legx1, legy1, legx2, legy2);
  TPad *pad1 = new TPad("pad1","This is pad1",0.02,0.02,0.48,0.83,33);
  TPad *pad2 = new TPad("pad2","This is pad2",0.52,0.02,0.98,0.83,33);
  pad1->SetFillColor(0);
  pad2->SetFillColor(0);

  if(logy) get_can()->SetLogy();
  double maximum =0;
  for(unsigned int m = 0; m < histos.size(); ++m ){
    if(histos[m]->GetMaximum() > maximum) maximum = histos[m]->GetMaximum();
  }
  if(histos.size()>1){
    get_can()->Divide(2);
    pad1->Draw();
    if(logy){
      get_can()->SetLogy(0);
      pad1->SetLogy();
    }
    pad2->Draw();
  }    
  if(max!=-1)maximum=max;
  //cout<<maximum<<" "<<max<<" "<<normArea<<" "<<histos.size()<<endl;
  for(unsigned int m = 0; m < histos.size(); ++m ){  
    histos[m]->SetLineColor(1+m);
    if(histos.size()>1)pad1->cd();
    if(options==1 && legend.size()>m) 
      leg->AddEntry( histos[m], legend[m].c_str(), "l");
    else if(options==2 && legend.size()>m){ 
      leg->AddEntry( histos[m], legend[m].c_str(), "l");
      leg->AddEntry((TObject*)0,("Entries: "+to_string( int(histos[m]->GetEntries()))).c_str(),"");
   }
    if(m==0){
      if(normArea){
	histos[m]->DrawNormalized("E")->SetMaximum(maximum);
      }
      else{
	histos[m]->SetMaximum(maximum*1.2);
	histos[m]->Draw();
      }
    }
    else{
      if(normArea)histos[m]->DrawNormalized("same hist"); 
      else histos[m]->Draw("same hist"); 
    }
    if(m+1== histos.size()){
      if(legend.size()>0 && options>0 )leg->Draw();
      //get_can()->Print(get_resultFile());
    }
  }
  if(histos.size()>1 && stack->Sizeof()==0){ 
    pad2->cd();
    for(unsigned int m = 0; m < histos.size(); ++m ){  
      TH1F* h_ratio = ratio(histos[m],histos[0],normArea);
      h_ratio->SetTitle("");
      h_ratio->SetLineColor(1+m);
      h_ratio->SetMaximum(3);
      h_ratio->SetMinimum(0);
      if(m==0) {
	h_ratio->SetFillColor(TColor::GetColor( "#aaaaaa" ));
	h_ratio->Draw("e2");
      }
      else
	h_ratio->Draw("same E");
    }
  }
  else if(stack->Sizeof()>0){
    pad2->cd();


  }




    //if(histos.size()) pad2->Update();

  if(stack->Sizeof()>0)stack->Draw("same hist");
  get_can()->Print(get_resultFile());
  if(logy) get_can()->SetLogy(0);  
  pad1->Close();
  pad2->Close();

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

TH1F* simplePlots::ratio(TH1F* num, TH1F* denom, bool norm){
  TH1F* ratio = (TH1F*)num->Clone();
  //cout<<ratio->GetSumw2N()<<endl;
  if(ratio->GetSumw2N()==0)ratio->Sumw2();
  if(denom->GetSumw2N() ==0)denom->Sumw2();
  if(norm){
    ratio->Scale(1/num->Integral());
    denom->Scale(1/denom->Integral());
  }
  ratio->Divide(denom);
  //double err =0;
  /*
  for(int i=0; i<num->GetNbinsX();i++){
    //if(num->GetBin(i) <0.1 || denom->GetBin(i)<0.1) continue;
    //Double_t ex_low = (num->GetXaxis()->GetBinCenter(i)) - (num->GetXaxis()->GetBinLowEdge(i));
    //Double_t ex_up =  (num->GetXaxis()->GetBinUpEdge(i)) - (num->GetXaxis()->GetBinCenter(i));
    //err = sqrt(num->GetBinError(i)*num->GetBinError(i)+denom->GetBinError(i)*denom->GetBinError(i));
    if(ratio->GetBinContent(i)!=0)cout<<"GetBinError "<<ratio->GetBinError(i)<<" GetBinContent "<< ratio->GetBinContent(i)<<endl;//,num->GetBinError(i));
    //eAsym -> SetPoint(i, num->GetXaxis()->GetBinCenter(i), num->GetBinContent(i)/denom->GetBinContent(i)); 
    //eAsym -> SetPointError(i, ex_low, ex_up, err, err); 
  }
  */
  ratio->SetMarkerStyle(0);
  ratio->SetMarkerSize(0);
  //ratio->SetLineColor(TColor::GetColor( "#aaaaaa" ));
  //ratio->SetFillColor(TColor::GetColor( "#aaaaaa" ));
  //eAsym->SetFillColor(TColor::GetColor( "#aaaaaa" ));
  //eAsym->SetLineWidth(1);
  //eAsym->SetFillStyle(3245);
  return ratio;
}
