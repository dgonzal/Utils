#include "simplePlots.h"



using namespace std;


simplePlots::simplePlots(string saveName, bool single): HistsBase(saveName, single){
  normArea=false;
  legx1=0.45; legx2= 0.7; legy1=0.5; legy2=0.8;
  max=-1;
  stack = new THStack("hs","stacked histograms");
}
void simplePlots::loadHists(TH1F * hist, string legend_entry,string plotting_style,bool ratio){
  histos.push_back(hist);
  if(!legend_entry.empty())legend.push_back(legend_entry);
  plotting_styles.push_back(plotting_style);
  plotInratio.push_back(ratio);
}
void simplePlots::loadTF1(TF1* func, string legend_entry, string plotting_style){
  func_leg.push_back(legend_entry);
  func_plotting.push_back(plotting_style);
  functions.push_back(func);
}

void simplePlots::loadTGraph(TGraph* graph, string legend_entry, string plotting_style){
  graph_leg.push_back(legend_entry);
  graph_plotting.push_back(plotting_style);
  graphs.push_back(graph);
}

void simplePlots::loadHists(string histname, string title,string plotting_style,bool ratio){
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
  plotting_styles.push_back(plotting_style);
  plotInratio.push_back(ratio);
}
void simplePlots::loadStackHists(string histname, int color){
  using_stack =true;
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

void simplePlots::loadStackHists(TH1F * hist, string legend_entry){
  using_stack =true;
  hist->SetFillStyle(1001);
  stack->Add(hist);
  if(!legend_entry.empty()) stack_legend.push_back(legend_entry);
}

void simplePlots::plotHists(int options, bool logy){
  get_can()->cd();
  bool single_plots = get_single();
  TString singlefile_result;
  if(single_plots){
    page_number++;
    TString filestring = get_resultFile()+to_string(page_number).c_str()+".pdf";
    filestring.ReplaceAll("/","_");
    singlefile_result = get_resultFile()+filestring;
    cout<<"Single File name "<<singlefile_result<<endl;
    //get_can()->Print(singlefile_result+"[");
  }

  leg = new TLegend(legx1, legy1, legx2, legy2);
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.05);

  //  coordinates:
  //  			 
  //  y3 +-------------+	
  //     |             |	
  //     |     pad1    |	
  //  y2 |-------------|	
  //     |     rp1     |	
  //  y1 +-------------+	
  //     x1            x2	
  // 			                                       
  Float_t y1, y2, y3, x1, x2;
  x1=0;y1=0.05;y2=0.25;x2=1;y3=0.75;

  TPad* pad1 = new TPad("histograms","histograms",x1, y2, x2, y3);
  TPad* pad2 = new TPad("ratio_errors","ratio and error", x1, y1, x2, y2);
  
  get_can()->SetCanvasSize(800,800);
  if(!draw_ratio  || !(histos.size()>1) ){
    get_can()->SetCanvasSize(800,600);
  }
  get_can()->Update();
  if(draw_ratio){
    pad1->SetTopMargin(0.1);pad1->SetBottomMargin(0.01);pad1->SetLeftMargin(0.15);pad1->SetRightMargin(0.1);
    pad2->SetTopMargin(0.01); pad2->SetBottomMargin(0.35); pad2->SetLeftMargin(0.15); pad2->SetRightMargin(0.1);
    get_can()->Divide(2);
    pad1->Draw();
    pad2->Draw();
  }
  else{
    pad1 = new TPad("histograms","histograms",0, 0, 1, 1);
    pad1->Draw();
  }

  if(logy)
    pad1->SetLogy(1);
  else
    pad1->SetLogy(0);

  double maximum =0;
  for(unsigned int m = 0; m < histos.size(); ++m ){
    if(histos[m]->GetMaximum() > maximum) maximum = histos[m]->GetMaximum();
  }
  if(histos.size()>1 && draw_ratio){
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
    
    if(!hist_title.empty()) histos[m]->SetTitle(hist_title.c_str());
    if(changecolors){
      int b = m+1;
      if(b>9) b++; 
      histos[m]->SetLineColor(b);
    }
    if(histos.size()>1 && draw_ratio)pad1->cd();
    if(options==1 && legend.size()>m) 
      leg->AddEntry( histos[m], legend[m].c_str(), "lpe");
    else if(options==2 && legend.size()>m){ 
      leg->AddEntry( histos[m], legend[m].c_str(), "lpe");
      //leg->AddEntry((TObject*)0,("Entries: "+to_string( int(histos[m]->GetEntries()))).c_str(),"");
    }
    if(m==0){
      
      if(histos[m]->GetMaximum()<0.09)TGaxis::SetMaxDigits(2);
      else TGaxis::SetMaxDigits(5);
      //histos[m]->SetMinimum(hist_minimum);    
      histos[m]->SetMinimum(0.0001);    
      histos[m]->GetYaxis()->SetTitle(hist_ytitle.c_str());
      //histos[m]->GetYaxis()->SetLabelSize(0.04/bottomSize); 
      histos[m]->GetYaxis()->SetTitleSize(0.06);
      histos[m]->GetYaxis()->SetTitleOffset(0.8);
      //cout<<histos[m]->GetYaxis()->GetXmin()<<endl;
      
      if(!draw_ratio||histos.size()==1){
	histos[m]->GetYaxis()->SetTitleSize(0.05);
	histos[m]->GetYaxis()->SetTitleOffset(1.);
	histos[m]->GetXaxis()->SetTitle(xtitle.c_str());
      }
      
      if(normArea){
	if(changecolors)histos[m]->SetMarkerStyle(20);
	if(plotting_styles[m].empty())
	  histos[m]->DrawNormalized("p")->SetMaximum(maximum);
	else
	  histos[m]->DrawNormalized(plotting_styles[m].c_str())->SetMaximum(maximum);
      }
      else if(perGeV){
	histos[m]->SetMaximum(maximum*1.3);
	TH1F* h_hist = normtoGeV((TH1F*)histos[m]->Clone());
	if(changecolors)h_hist->SetMarkerStyle(20);
	if(plotting_styles[m].empty())
	  h_hist->Draw("p");
	else
	  h_hist->Draw(plotting_styles[m].c_str());
      }
      else{
	histos[m]->SetMaximum(maximum*1.3);
	if(changecolors)histos[m]->SetMarkerStyle(20);
	if(plotting_styles[m].empty())
	  histos[m]->Draw("p");
	else
	  histos[m]->Draw(plotting_styles[m].c_str());
      }
    }
    else{
      if(normArea){
	if(!plotting_styles[m].empty())
	  histos[m]->DrawNormalized(("same"+plotting_styles[m]).c_str());
	else
	  histos[m]->DrawNormalized("same hist");
      }
      else if(perGeV){
	TH1F* h_hist = normtoGeV((TH1F*)histos[m]->Clone());
	if(!plotting_styles[m].empty())
	   h_hist->Draw(plotting_styles[m].c_str());
	else  h_hist->Draw("same hist");
      }
      else if(!plotting_styles[m].empty())
	histos[m]->Draw(plotting_styles[m].c_str());
      else histos[m]->Draw("same hist"); 
    }
  }
  if(histos.size()>1 && draw_ratio){ 
    pad2->cd();
    for(unsigned int m = 0; m < histos.size(); ++m ){  
      if(!plotInratio[m])continue;

      TH1F* h_ratio = ratio(histos[m],(TH1F*)histos[0]->Clone(),normArea,zeroratiobin);
      //control over size for label size and offset ...
      double bottomSize = 0.273;
      h_ratio->SetTitle("");
      h_ratio->GetXaxis()->SetTitle(xtitle.c_str());
      h_ratio->GetYaxis()->SetTitle(ratio_ytitle.c_str());
      h_ratio->GetXaxis()->SetLabelSize(0.03/bottomSize); //h_ratio->GetXaxis()->SetNdivisions(100);
      h_ratio->GetXaxis()->SetTitleOffset(0.2/bottomSize);
      h_ratio->GetYaxis()->SetLabelSize(0.03/bottomSize); 
      h_ratio->GetYaxis()->SetNdivisions(505);
      h_ratio->GetXaxis()->SetTitleSize( 0.04/bottomSize);
      h_ratio->GetYaxis()->SetTitleSize( 0.13);
      h_ratio->GetYaxis()->SetTitleOffset(0.4);
      h_ratio->GetYaxis()->CenterTitle();
      h_ratio->SetTickLength( 0.03 / bottomSize );
      
      if(fit_ratio ==true && m==1 &&  h_ratio->GetEntries()>10 ){
         TF1* linear_fit = new TF1("linfit","[0]*x+[1]");
	 TH1F* fitratio = (TH1F*) h_ratio->Clone();
	 fitratio->SetFillColor(TColor::GetColor("#ffa500")); 
         TFitResultPtr fitres = fitratio->Fit(linear_fit,"S0EQ");
	 TMatrixDSym cov = fitres->GetCovarianceMatrix();
	 fitres->Print("V");
	 TF1* fiterrup = (TF1*) linear_fit->Clone();
	 TF1* fiterrdown = (TF1*) linear_fit->Clone();
	 fiterrup->SetParameter(1,linear_fit->GetParameter(1)+linear_fit->GetParError(1));
	 //fiterrup->SetParameter(0,linear_fit->GetParameter(0)+linear_fit->GetParError(0));
	 fiterrdown->SetParameter(1,linear_fit->GetParameter(1)-linear_fit->GetParError(1));
	 //fiterrdown->SetParameter(0,linear_fit->GetParameter(0)-linear_fit->GetParError(0));
	 for(int bin=0;bin<fitratio->GetNcells();++bin){
	    double bincenter = fitratio->GetBinCenter(bin);
	    fitratio->SetBinContent(bin,linear_fit->Eval(bincenter));
            //fitratio->SetBinError(bin,0);
            fitratio->SetBinError(bin,fabs(fiterrup->Eval(bincenter)-fiterrdown->Eval(bincenter)));
	    //cout<<linear_fit->Eval(bincenter)<<" "<<offset+slope*bincenter<<endl;
         } 
	 //assert(1==0);
	 fitratio->Draw("E3 same");
         //linear_fit->Draw("same");
      }
      if(changecolors)h_ratio->SetLineColor(1+m);
      h_ratio->SetMaximum(ratio_max);
      h_ratio->SetMinimum(ratio_min);
      if(m==0) {
	h_ratio->SetFillColor(TColor::GetColor( "#aaaaaa" ));
	h_ratio->Draw(ratio_drawopt_nom.c_str());
      }
      else
	h_ratio->Draw(ratio_drawopt_next.c_str());
    }
  }
    //if(histos.size()) pad2->Update();
  if(using_stack){
    TList * stack_list = stack->GetHists();
    for(unsigned int i=0; i<stack_legend.size();i++){
      leg->AddEntry(stack_list->At(i), stack_legend[i].c_str(), "f");
    }
    stack->Draw("same hist");
    gPad->RedrawAxis();  
    //stack->GetHistogram()->Draw("same");	
  }
  /*/
  //go back to first pad!
  if(draw_ratio)pad1->cd();
  if(using_stack || histos.size()>0){
    if(plotting_styles[0].empty())
      if(normArea)
	histos[0]->DrawNormalized("same p");
      else
	histos[0]->Draw("same p");
    else
      if(normArea)
	histos[0]->DrawNormalized(("same "+plotting_styles[0]).c_str());
      else
	histos[0]->Draw(("same "+plotting_styles[0]).c_str());
  }
  /*/
  for(unsigned int i=histos.size();i<legend.size();++i)
     if(!legend[i].empty())leg->AddEntry((TObject*)0, legend[i].c_str(), "");

  for(unsigned int i=0; i<graphs.size();++i){
    graphs[i]->Draw(graph_plotting[i].c_str());
    if(!graph_leg[i].empty()) leg->AddEntry(graphs[i],graph_leg[i].c_str(),"");
  }

  for(unsigned int i=0; i<functions.size();++i){
    functions[i]->Draw(func_plotting[i].c_str());
    if(!func_leg[i].empty()) leg->AddEntry(functions[i],func_leg[i].c_str(),"");
  }
  
  get_can()->Update();
  if(!leg_title.empty())leg->SetHeader(leg_title.c_str());
  leg->Draw();
  if(single_plots){
    get_can()->Print(singlefile_result);
  }
  else{
    get_can()->Print(get_resultFile());
  }
  
  if(logy) get_can()->SetLogy(0);
  //if(draw_ratio)pad1->Close();
  //if(draw_ratio)pad2->Close();
  get_can()->Clear();
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
      twoDhists[m]->Scale(1/twoDhists[m]->GetSumOfWeights()); 
      twoDhists[m]->Draw("colz"); 
    }
    get_can()->Print(get_resultFile());
  }
} 

TH1F* simplePlots::ratio(TH1F* num, TH1F* denom, bool norm, bool zeroBinAsOne){
  TH1F* ratio = (TH1F*)num->Clone();
  TH1F* denom_zero_error = (TH1F*)denom->Clone();
   for(int m=0;m<denom_zero_error->GetNcells();m++)
    denom_zero_error->SetBinError(m,0);
  //cout<<ratio->GetSumw2N()<<endl;
  if(ratio->GetSumw2N()==0)ratio->Sumw2();
  if(denom_zero_error->GetSumw2N()==0)denom_zero_error->Sumw2();
  if(norm){
    ratio->Scale(1/num->GetSumOfWeights());
    denom_zero_error->Scale(1/denom->GetSumOfWeights());
  }
  ratio->Divide(denom_zero_error);
  ratio->SetMarkerStyle(0);
  ratio->SetMarkerSize(0);
  if(zeroBinAsOne){
    for(int m=0;m<ratio->GetNcells();m++)
      if(ratio->GetBinContent(m)==0.){
	ratio->SetBinContent(m,1);
	ratio->SetBinError(m,0);
      }
  }
  
  //ratio->SetLineColor(TColor::GetColor( "#aaaaaa" ));
  //ratio->SetFillColor(TColor::GetColor( "#aaaaaa" ));
  //eAsym->SetFillColor(TColor::GetColor( "#aaaaaa" ));
  //eAsym->SetLineWidth(1);
  //eAsym->SetFillStyle(3245);
  return ratio;
}

TH1F* simplePlots::normtoGeV(TH1F* hist){
  double cont = -1, err = -1, width= -1;
  for(unsigned int i =0; i< hist->GetNcells(); i++){
    cont   = hist->GetBinCenter(i);
    err    = hist->GetBinError(i);
    width  = hist->GetBinWidth(i);
    hist->SetBinContent(i,cont/width);
    hist->SetBinError(i,err/width);
  }
  return hist;
}
