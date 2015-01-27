#include "effiPlots.h"


effiPlots::effiPlots(string saveName): HistsBase(saveName){}



void effiPlots::loadHists(string s_denominator, string s_numerator){

  for(auto fileDir : get_filedirs()){
    TFile* file = new TFile(fileDir.c_str());
    TH1F * numerator = (TH1F*) file->Get(s_numerator.c_str());
    TH1F * denominator = (TH1F*) file->Get(s_denominator.c_str());

    histos.push_back(effiContainer(numerator,denominator));
  }
}

void effiPlots::plotEffi(){

  double maximum = 0;

  for(unsigned int i = 0; i < histos.size(); ++i ){
    histos[i].numerator->GetMaximum() < histos[i].denominator->GetMaximum() ? maximum = histos[i].denominator->GetMaximum() : maximum = histos[i].numerator->GetMaximum();
    
    histos[i].numerator->SetMaximum(maximum*1.2);
    histos[i].numerator->SetLineColor(kRed);
    histos[i].denominator->SetLineColor(kBlack);

    histos[i].numerator->Draw();
    histos[i].denominator->Draw("same");

    get_can()->Print(get_resultFile());

    double current_num = 0;
    double current_den = 0;

    int NBins = histos[i].numerator->GetXaxis()->GetNbins();
    
    for(int m = 0; m < NBins; ++m){
      current_num = histos[i].numerator->Integral(m,NBins);;
      current_den = histos[i].denominator->Integral(m,NBins);

      histos[i].numerator->SetBinContent(m,current_num);
      histos[i].denominator->SetBinContent(m,current_den);
    }

    TGraphAsymmErrors* graph = new TGraphAsymmErrors(histos[i].numerator,histos[i].denominator);
    graph->SetTitle("Efficiency");
    graph->Draw("ap");
    get_can()->Print(get_resultFile());
  }    
}
