#include "effiPlots.h"


using namespace std;

effiPlots::effiPlots(string saveName): HistsBase(saveName){
  rangeMax = 0;
  rangeMin = 0;
  imposeDistri=false;
  legend_bool=true;
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


void effiPlots::loadHists(string s_denominator, string s_numerator, string leg_entry_graph){
  std::vector<std::string> sample_nicks = get_nicknames();
  int count  =-1;
  for(const auto & fileDir : get_filedirs()){
    count ++;
    if(debug)std::cout<< "leg entry " << leg_entry_graph<<" nick sample "<<sample_nicks[count]<<std::endl;
    if(!leg_entry_graph.empty() || !sample_nicks[count].empty())legend_entries.push_back(leg_entry_graph+" "+sample_nicks[count]); 
    else legend_entries.push_back("");
    if(debug)cout<<"Denom "<<s_denominator<<" Num "<<s_numerator<<endl;
    TFile* file = new TFile(fileDir.c_str());
    if(debug)cout<<"loaded file"<<endl;
    TH1F * numerator;
    if(file->GetListOfKeys()->Contains(s_numerator.c_str()))cout<< s_numerator<<" does not exist"<<endl;
    if(file->GetListOfKeys()->Contains(s_denominator.c_str()))cout<< s_denominator<<" does not exist"<<endl;
    if(!s_numerator.empty())
      numerator = (TH1F*) file->Get(s_numerator.c_str());
      if(debug) cout<<numerator->GetTitle()<<endl;
    else{
      cout<<"numerator string empty taking denominator string "<<s_denominator<<endl;
      numerator = (TH1F*) file->Get(s_denominator.c_str());
    }		     
    if(debug)cout<<"loaded numerator"<<endl;
    TH1F * denominator = (TH1F*) file->Get(s_denominator.c_str());
    if(debug)cout<<"loaded denominator"<<endl;
    size_t pos_denom = s_denominator.find("/");
    size_t pos_num   = s_numerator.find("/");
    if(debug){
      cout<<"Found seperators"<<endl;
      cout<<"Numerator Title "<<numerator->GetTitle()<<endl;
      cout<<"numerator string "<<s_numerator<<" position of / "<< pos_num <<" new titel "<<(s_numerator.substr(0,pos_num)).c_str() <<endl;
    }
    numerator->SetTitle((s_numerator.substr(0,pos_num)).c_str());
    numerator->GetXaxis()->SetTitle((s_numerator.substr(pos_num+1,-1)).c_str());
    if(debug){
      cout<<"Denominator Title "<<denominator->GetTitle()<<endl; 
      cout<<"denominator string "<<s_denominator<<" position of / "<< pos_denom <<" new titel "<<(s_denominator.substr(0,pos_denom)).c_str() <<endl;
    }
    denominator->SetTitle((s_denominator.substr(0,pos_denom)).c_str());
    denominator->GetXaxis()->SetTitle((s_denominator.substr(pos_denom+1,-1)).c_str());
    if(debug)cout<<"Renaming done"<<endl;
    if(rebin_int > 0){
      numerator->Rebin(rebin_int);
      denominator->Rebin(rebin_int);
    }
    histos.push_back(effiContainer(numerator,denominator));
    if(debug)cout<<"push_back done"<<endl;
    file->Close();
  }
}

void effiPlots::plotEffi(int options){
  double maximum = 0;
  TMultiGraph * resultGraphs = new TMultiGraph();
  //get_can()->SetLogy();
  TLegend* multigraphLeg = new TLegend(0.3,0.2,0.5,0.4); 
  multigraphLeg->SetBorderSize(0);
  for(unsigned int i = 0; i < histos.size(); ++i ){
    leg = new TLegend(0.65,0.65,0.85,0.85);
    histos[i].numerator->GetMaximum() <= histos[i].denominator->GetMaximum() ? maximum = histos[i].denominator->GetMaximum() : maximum = histos[i].numerator->GetMaximum();  
    histos[i].numerator->SetMaximum(maximum*1.2);
    histos[i].numerator->SetLineColor(kRed);
    histos[i].denominator->SetLineColor(kBlack);
    if(rangeMax!=rangeMin)histos[i].numerator->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    if(rangeMax!=rangeMin)histos[i].denominator->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    //if(!x_axis.empty()){
    //	histos[i].numerator->GetXaxis()->SetTitle(x_axis.c_str());
    //	histos[i].denominator->GetXaxis()->SetTitle(x_axis.c_str());
    //}
    histos[i].numerator->Draw();
    histos[i].denominator->Draw("same");
    Double_t error_num = -1;
    Double_t error_denom = -1;
    double integral_num = histos[i].numerator->IntegralAndError(1,-1,error_num);
    double integram_denom = histos[i].denominator->IntegralAndError(1,-1,error_denom);
    //if(!legend_entries[i].empty())leg->AddEntry((TObject*)0,(legend_entries[i]).c_str(),"");
    leg->AddEntry(histos[i].numerator, ("Numerator Events: "+to_string(int (integral_num))+"+-"+to_string(int(ceil(error_num)))).c_str(), "l");
    leg->AddEntry(histos[i].denominator,("Denominator Events: "+to_string(int (integram_denom))+"+-"+to_string(int(ceil(error_denom)))).c_str(), "l");
    leg->AddEntry((TObject*)0,("Total Efficiency: "+to_string(int (integral_num/integram_denom *100))).c_str(),"");
    leg->Draw();
    get_can()->Print(get_resultFile());
    double current_num = 0;
    double current_den = 0;
    double num_error =0;
    double den_error=0;
    TH1F* hist_denom = (TH1F*)histos[i].denominator->Clone();
    int NBins = histos[i].numerator->GetXaxis()->GetNbins();
    //cout<<"Number of bins "<<NBins<<endl;
    if(options==1||options==0){
      for(int m = 0; m < NBins; ++m){
	if(options==0){
	  current_num = histos[i].numerator->IntegralAndError(m+1,NBins+1,num_error);
	  current_den = histos[i].denominator->IntegralAndError(m+1,NBins+1,den_error);
	}
	else if(options==1) {
	  current_den = histos[i].denominator->IntegralAndError(1,m+1,den_error);
	  current_num = histos[i].numerator->IntegralAndError(1,m+1,num_error);
	}
	histos[i].numerator->SetBinContent(m,current_num);histos[i].numerator->SetBinError(m,num_error);
	histos[i].denominator->SetBinContent(m,current_den); histos[i].denominator->SetBinError(m,den_error);
      }
    }
    TGraphAsymmErrors* graph = new TGraphAsymmErrors(histos[i].numerator,histos[i].denominator,"cl=0.683 b(1,1) mode");
    if(rangeMax!=rangeMin)graph->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    //graph->SetTitle((string(histos[i].numerator->GetTitle())+" Efficiency").c_str());//+"/"+string(histos[i].denominator->GetTitle())).c_str());
    if(debug) std::cout<<"legend size "<<legend_entries.size()<<" entry "<< legend_entries[i]<<std::endl;
    if(!legend_entries[i].empty())multigraphLeg->AddEntry(graph,(legend_entries[i]).c_str(),"lp");
    //multigraphLeg->AddEntry(graph,"test","ap");
    graph->Draw("sameap");
    graph->GetYaxis()->SetTitle(y_axis.c_str());
    graph->GetXaxis()->SetTitle((string(histos[i].numerator->GetXaxis()->GetTitle())).c_str());
    if(imposeDistri)drawDistri(hist_denom,graph->GetMaximum());
    get_can()->Print(get_resultFile());
    //graph->SetMarkerStyle(21+i);
    graph->SetLineColor(1+i);
    resultGraphs->Add(graph);
    //leg->AddEntry(graph,histos.at(i).numerator->GetTitle,"l");
  }    
  resultGraphs->Draw("ap");//should be ap have to change marker style
  resultGraphs->GetYaxis()->SetTitle("Efficiency");
  resultGraphs->GetXaxis()->SetTitle(x_axis.c_str());
  if(rangeMax!=rangeMin){
    get_can()->Modified();  
    resultGraphs->GetXaxis()->SetLimits(rangeMin,rangeMax);
  }
  histos[0].denominator->DrawNormalized("histsame"); 

  multigraphLeg->Draw();
  //resultGraphs->Draw("ap");//should be ap have to change marker style
  //get_can()->BuildLegend();
  //resultGraphs->SetMinimum(0.9);

  //if(imposeDistri)drawDistri(0);
  if(legend_bool)get_can()->BuildLegend(0.15, 0.1, 0.9, 0.4);
  get_can()->Print(get_resultFile());
}

void effiPlots::drawDistri(TH1F* hist, double max){
  gStyle->SetPadTickY(0);
  get_can()->UseCurrentStyle();
  get_can()->Update();
  cout<<hist->GetMaximum()<<endl;;
  Float_t rightmax = hist->GetMaximum();
  //Float_t scale = 1.1/rightmax;
  //hist->SetFillColor(kBlack);
  hist->Scale(max);
  hist->Draw("same");
  //draw an axis on the right side
  TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
			    gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,50510,"+L");
  axis->SetLineColor(kBlack);
  axis->SetLabelColor(kBlack);
  axis->Draw();
}


void effiPlots::imposeHist(bool impose_switch){
  imposeDistri=impose_switch;
}
