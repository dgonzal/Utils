#include "effiPlots.h"


effiPlots::effiPlots(string saveName): HistsBase(saveName){
  rangeMax = 0;
  rangeMin = 0;
}

void effiPlots::loadTHHists(string s_denominator, string s_numerator){

  for(const auto & fileDir : get_filedirs()){
    TFile* file = new TFile(fileDir.c_str());
    TH1 * numerator;
    if(!s_numerator.empty())numerator = (TH1*) file->Get(s_numerator.c_str());
    else  numerator = (TH1*) file->Get(s_denominator.c_str());
    TH1 * denominator = (TH1*) file->Get(s_denominator.c_str());
   

    histsTH.push_back(TH1EffiCont(numerator,denominator));
  }
}

void effiPlots::plotTH(){

  for(unsigned int i=0; i<histsTH.size(); ++i){
    TEfficiency effi(*histsTH.at(i).num, *histsTH.at(i).denom);
    //if(title.size()>=i) effi.SetTitle(title.at(i).c_str());
    effi.Draw("colz");
    get_can()->Print(get_resultFile());
  }
}


void effiPlots::loadHists(string s_denominator, string s_numerator){

  for(const auto & fileDir : get_filedirs()){
    TFile* file = new TFile(fileDir.c_str());
    TH1F * numerator;
    if(!s_numerator.empty())numerator = (TH1F*) file->Get(s_numerator.c_str());
    else  numerator = (TH1F*) file->Get(s_denominator.c_str());
    TH1F * denominator = (TH1F*) file->Get(s_denominator.c_str());

    size_t pos_denom = s_denominator.find("/");
    size_t pos_num   =  s_numerator.find("/");
    
    numerator->SetTitle((s_numerator.substr(0,pos_num)).c_str());
    denominator->SetTitle( (s_denominator.substr(0,pos_denom)).c_str());

    

    cout<<numerator->GetTitle()<<" "<<s_numerator<<" "<< pos_num <<endl;
    histos.push_back(effiContainer(numerator,denominator));
  }
}

void effiPlots::plotEffi(int options){

  double maximum = 0;
  TMultiGraph * resultGraphs = new TMultiGraph();
  //leg = new TLegend(0.6,0.7,0.8,0.8);


  for(unsigned int i = 0; i < histos.size(); ++i ){
    histos[i].numerator->GetMaximum() <= histos[i].denominator->GetMaximum() ? maximum = histos[i].denominator->GetMaximum() : maximum = histos[i].numerator->GetMaximum();
    
    histos[i].numerator->SetMaximum(maximum*1.2);
    histos[i].numerator->SetLineColor(kRed);
    histos[i].denominator->SetLineColor(kBlack);

    if(rangeMax!=rangeMin)histos[i].numerator->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    if(rangeMax!=rangeMin)histos[i].denominator->GetXaxis()->SetRangeUser(rangeMin,rangeMax);

    histos[i].numerator->Draw();
    histos[i].denominator->Draw("same");

    get_can()->Print(get_resultFile());

    double current_num = 0;
    double current_den = 0;

    double num_error =0;
    double den_error=0;

    int NBins = histos[i].numerator->GetXaxis()->GetNbins();
    
    if(options==1) current_den = histos[i].denominator->IntegralAndError(0,NBins,den_error);

    for(int m = 0; m < NBins; ++m){
      current_num = histos[i].numerator->IntegralAndError(m,NBins,num_error);
      if(options==0)current_den = histos[i].denominator->IntegralAndError(m,NBins,den_error);

      histos[i].numerator->SetBinContent(m,current_num);histos[i].numerator->SetBinError(m,num_error);
      histos[i].denominator->SetBinContent(m,current_den); histos[i].denominator->SetBinError(m,den_error);
      
    }

    TGraphAsymmErrors* graph = new TGraphAsymmErrors(histos[i].numerator,histos[i].denominator,"cl=0.683 b(1,1) mode");

    if(rangeMax!=rangeMin)graph->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    graph->SetTitle((string(histos[i].numerator->GetTitle())+"/"+string(histos[i].denominator->GetTitle())).c_str());
    graph->Draw("ap");
    get_can()->Print(get_resultFile());

    //graph->SetMarkerStyle(21+i);
    graph->SetLineColor(1+i);
    
    resultGraphs->Add(graph);
    //leg->AddEntry(graph,histos.at(i).numerator->GetTitle,"l");
  }    

  
  resultGraphs->SetTitle("Efficiency");
  resultGraphs->Draw("ap");//should be ap have to change marker style
  //leg->Draw();
  get_can()->BuildLegend(0.15, 0.1, 0.9, 0.4);
  if(rangeMax!=rangeMin){
    get_can()->Modified();
    resultGraphs->GetXaxis()->SetLimits(rangeMin,rangeMax);
  }
  get_can()->Print(get_resultFile());

}
