#include "effiPlots.h"


using namespace std;

effiPlots::effiPlots(string saveName, bool single): HistsBase(saveName,single){
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
    file->Close();
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
  leg_title=leg_entry_graph;
  int count  =-1;
  for(const auto & fileDir : get_filedirs()){
    count ++;
    if(debug)std::cout<< "leg entry " << leg_entry_graph<<" nick sample "<<sample_nicks[count]<<std::endl;
    if(!leg_entry_graph.empty() || !sample_nicks[count].empty())legend_entries.push_back(sample_nicks[count]); 
    else legend_entries.push_back("");
    if(debug)cout<<"Denom "<<s_denominator<<" Num "<<s_numerator<<endl;
    TFile* file = new TFile(fileDir.c_str());
    if(debug)cout<<"loaded file"<<endl;
    TH1F * numerator;
    // you have to check first the dir and than the hist!!
    //if(file->GetListOfKeys()->Contains(s_numerator.c_str())==1)cout<< s_numerator<<" does not exist"<<endl;
    //if(file->GetListOfKeys()->Contains(s_denominator.c_str())==1)cout<< s_denominator<<" does not exist"<<endl;
    if(!s_numerator.empty()){
      numerator = (TH1F*) file->Get(s_numerator.c_str());
      if(debug) cout<<numerator->GetTitle()<<endl;
    }
    else{
      cout<<"numerator string "<<s_numerator<<" empty taking denominator string "<<s_denominator<<endl;
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
    if(rebin_bins.size()>0){  
      cout<<"============================="<<endl;
      cout<<"*****************************"<<endl;
      cout<<"rebin with list"<<endl; 
      numerator->Rebin(rebin_bins.size()-1,"", (double*) &rebin_bins[0]);
      denominator->Rebin(rebin_bins.size()-1,"", (double*) &rebin_bins[0]);
    }
    histos.push_back(effiContainer(numerator,denominator));
    if(debug)cout<<"push_back done"<<endl;
    file->Close();
  }
}


TString effiPlots::create_resultfilename(int & number, bool summary){
  TString filestring = get_resultFile()+to_string(number).c_str()+".pdf";
  if(summary) filestring = get_resultFile()+"summary"+to_string(number).c_str()+".pdf";
  filestring.ReplaceAll("/","_");
  number++;
  return get_resultFile()+filestring;  
}

void effiPlots::plotEffi(int options, vector<pair<int,int>> scalefactor){
  get_can()->cd();
  bool single_plots = get_single();
  double maximum = 0;
  TMultiGraph * resultGraphs = new TMultiGraph();
  //get_can()->SetLogy();
  TLegend* multigraphLeg = new TLegend(0.3,0.2,0.5,0.4); 
  multigraphLeg->SetBorderSize(0);
  multigraphLeg->SetTextSize(0.03);
  for(unsigned int i = 0; i < histos.size(); ++i ){
    /*
    gStyle->SetPadTickY(1);
    get_can()->UseCurrentStyle();
    get_can()->Update();
    */
    resultGraphs->SetMinimum(0.7);

    leg = new TLegend(0.65,0.65,0.85,0.85);
    histos[i].numerator->GetMaximum() <= histos[i].denominator->GetMaximum() ? maximum = histos[i].denominator->GetMaximum() : maximum = histos[i].numerator->GetMaximum();  
    histos[i].numerator->SetMaximum(maximum*1.2);
    histos[i].numerator->SetLineColor(kRed);
    histos[i].denominator->SetLineColor(kBlack);
    if(rangeMax!=rangeMin)histos[i].numerator->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    if(rangeMax!=rangeMin)histos[i].denominator->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    if(yRangeMax!=yRangeMin)histos[i].numerator->GetYaxis()->SetRangeUser(yRangeMin,yRangeMax);
    if(yRangeMax!=yRangeMin)histos[i].denominator->GetYaxis()->SetRangeUser(yRangeMin,yRangeMax);
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

    if(single_plots){
      get_can()->Print(create_resultfilename(page_number));
    }
    else
      get_can()->Print(get_resultFile());
    
    double current_num = 0;
    double current_den = 0;
    double num_error =0;
    double den_error=0;
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
    for(int bini =0; bini < histos[i].numerator->GetNcells();++bini){
      if(histos[i].numerator->GetBinContent(bini) <=0){
	histos[i].numerator->SetBinContent(bini,0.);
	histos[i].numerator->SetBinError(bini,0.);
      }
      if(histos[i].denominator->GetBinContent(bini) <=0){
	histos[i].denominator->SetBinContent(bini,0.);
	histos[i].denominator->SetBinError(bini,0.);
      }
    }
    TGraphAsymmErrors* graph = new TGraphAsymmErrors((TH1F*)histos[i].numerator->Clone(),(TH1F*)histos[i].denominator->Clone(),"cl=0.683 b(1,1) mode");
    if(rangeMax!=rangeMin)graph->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
    //graph->SetTitle((string(histos[i].numerator->GetTitle())+" Efficiency").c_str());//+"/"+string(histos[i].denominator->GetTitle())).c_str());
    if(debug) std::cout<<"legend size "<<legend_entries.size()<<" entry "<< legend_entries[i]<<std::endl;
    if(!legend_entries[i].empty()){
      multigraphLeg->SetHeader(leg_title.c_str());
      multigraphLeg->AddEntry(graph,legend_entries[i].c_str(),"l");
      graph->SetTitle(legend_entries[i].c_str());    }
    graph->SetMinimum(0.0);
    graph->Draw("sameap");
    graph->GetYaxis()->SetTitle(y_axis.c_str());
    graph->GetXaxis()->SetTitle((string(histos[i].numerator->GetXaxis()->GetTitle())).c_str());
    if(!x_axis.empty())graph->GetXaxis()->SetTitle(x_axis.c_str());
    if(imposeDistri)drawDistri((TH1F*)histos[i].denominator->Clone(),graph->GetMaximum());
    get_can()->Print(get_resultFile());
    graph->SetLineColor(1+i);
    resultGraphs->Add(graph);
    //delete graph;
    //graph->Close();
  }
  /*
  gStyle->SetPadTickY(2);
  //get_can()->UseCurrentStyle();
  get_can()->Update();
  */
  resultGraphs->SetMinimum(0.5);
  resultGraphs->SetMaximum(1.01);
  resultGraphs->Draw("ap");//should be ap have to change marker style
  resultGraphs->GetYaxis()->SetTitle("Efficiency");
  resultGraphs->GetXaxis()->SetTitle(x_axis.c_str());
  if(rangeMax!=rangeMin){
    get_can()->Modified();  
    resultGraphs->GetXaxis()->SetLimits(rangeMin,rangeMax);
  }
  
  //if(imposeDistri)histos[0].denominator->DrawNormalized("hist same"); 
  multigraphLeg->Draw(); 
  //resultGraphs->Draw("ap");//should be ap have to change marker style
  //get_can()->BuildLegend();
  //resultGraphs->SetMinimum(0.9);
  //if(imposeDistri)drawDistri(0);
  if(legend_bool)get_can()->BuildLegend(0.15, 0.15, 0.8, 0.5);
  if(single_plots){
    get_can()->Print(create_resultfilename(summary_number,true));
  }
  else
    get_can()->Print(get_resultFile());

  if(scalefactor.size()>0){
    TList * graphlist = (TList*)resultGraphs->GetListOfGraphs ();
    TMultiGraph* scale_ratios = new TMultiGraph();
    TLegend* multiratioLeg = new TLegend(0.2,0.2,0.7,0.4);
    multiratioLeg->SetBorderSize(0); 
    int counter =-1;
    for(auto tuple : scalefactor){
      counter++;
      TH1F* num_h = (TH1F*) histos[tuple.first].numerator->Clone();
      num_h->Divide(histos[tuple.first].denominator);
      TH1F* den_h = (TH1F*) histos[tuple.second].numerator->Clone();
      den_h->Divide(histos[tuple.second].denominator);
      
      TGraphAsymmErrors* scaleresult = compute_scalefactorerrors(num_h,den_h, (TGraphAsymmErrors*) resultGraphs->GetListOfGraphs()->At(tuple.first),(TGraphAsymmErrors*) resultGraphs->GetListOfGraphs()->At(tuple.second));
      TGraphAsymmErrors* helper_graph = (TGraphAsymmErrors*)scaleresult->Clone();
      helper_graph->SetLineColor(counter+1);
      multiratioLeg->AddEntry(helper_graph,(std::string(resultGraphs->GetListOfGraphs()->At(tuple.first)->GetTitle())+"/"+string(resultGraphs->GetListOfGraphs()->At(tuple.second)->GetTitle())).c_str(),"lp");
      scale_ratios->Add(helper_graph);
      //scaleresult->GetXaxis()->SetRangeUser(45,800);
      scaleresult->SetTitle("");
      scaleresult->GetXaxis()->SetTitle(x_axis.c_str());
      //scaleresult->GetYaxis()->SetRangeUser(.85,1.04);
      scaleresult->Draw("P");
      TH1F* hist = (TH1F*) scaleresult->GetHistogram();
      //hist->SetMaximum(1.04);
      hist->SetMinimum(0.8);
      hist->Draw();
      scaleresult->Draw("P");
      get_can()->Update();
      if(single_plots){
	get_can()->Print(create_resultfilename(summary_number,true));
      }
      else
	get_can()->Print(get_resultFile());
      
      // Calculate pull between two
      TGraphAsymmErrors* first = (TGraphAsymmErrors*) graphlist->At(tuple.first);
      TGraphAsymmErrors* second = (TGraphAsymmErrors*) graphlist->At(tuple.second);
      TH1F* result = (TH1F*) first->GetHistogram();
      result->SetMaximum(4);
      result->SetMinimum(-4);
      result->Rebin(result->GetNcells()/first->GetN()); 
      double x_f, y_f, x_s, y_s;
      for(int i=0; i<first->GetN();i++){
	first->GetPoint(i,x_f,y_f);
	int m=0;
	while(second->GetPoint(m,x_s,y_s) >=0){
	  if(x_s==x_f) break;
	  m++;
	}
	double bin_result = y_f-y_s;
	if(bin_result>0) bin_result /= sqrt(pow(first->GetErrorYlow(i),2)+pow(second->GetErrorYhigh(m),2));
	else bin_result /= sqrt(pow(first->GetErrorYhigh(i),2)+pow(second->GetErrorYlow(m),2));
	result->SetBinContent(i,bin_result);
	cout<<"pull "<<bin_result<<" first bin "<<i<<" second bin "<<m<< " x "<< x_f<<" y first "<<y_f<<" y second "<<y_s <<endl;
	
      }
      result->Draw();
      if(single_plots){
	get_can()->Print(create_resultfilename(summary_number,true));
      }
      else
	get_can()->Print(get_resultFile());
    }
    get_can()->Update();
    //scale_ratios->GetHistogram()->Draw();
    scale_ratios->SetMinimum(0.7);
    scale_ratios->Draw("AP");
    scale_ratios->GetXaxis()->SetTitle(x_axis.c_str());
    multiratioLeg->Draw();
    get_can()->Update();
    if(single_plots){
        get_can()->Print(create_resultfilename(summary_number,true));
    }
    else
       get_can()->Print(get_resultFile());

    //delete resultGraphs;
    //multigraphLeg->Close(); 
  }
}


TGraphAsymmErrors* effiPlots::compute_scalefactorerrors(TH1F* num_h, TH1F* den_h, TGraphAsymmErrors* scaledenom, TGraphAsymmErrors* scalenum){
  TH1F* scalehist = (TH1F*) num_h->Clone();
  scalehist->Divide(den_h);
  int bins = scalehist->GetNcells()-2;
  //scalehist->Draw();
  nominal ="";up_var="";down_var="";
  double x[bins], xlow[bins], xhigh[bins], y[bins], ylow[bins], yhigh[bins];
  TAxis *xaxis = scalehist->GetXaxis();
  int firstbin = 0, lastbin = bins;
  bool first = false;
  for(int i=0; i<bins;++i){
    double num_con = num_h->GetBinContent(i+1); 
    double den_con = den_h->GetBinContent(i+1);
    if(den_con != 0 && num_con != 0){
      if(!first){
	firstbin=i;
	first = true;
      }
      lastbin=i;
    }
  }
  cout<<"TGraphAsymmErrors denominator"<<endl;
  for(int i=0;i<scaledenom->GetN();++i){
    double graph_x=0,graph_y=0;
    scaledenom->GetPoint(i,graph_x,graph_y);
    cout<<"denom graph content "<<graph_y<<" x "<<graph_x <<" + "<<scaledenom->GetErrorYhigh(i)<<" - "<<scaledenom->GetErrorYlow(i) <<endl;
  }
  cout<<"TGraphAsymmErrors numerator"<<endl;
  for(int i=0;i<scalenum->GetN();++i){
    double graph_x=0,graph_y=0;
    scalenum->GetPoint(i,graph_x,graph_y);
    cout<<"numerator graph content "<<graph_y<<" x "<<graph_x<<" + "<<scalenum->GetErrorYhigh(i)<<" - "<<scalenum->GetErrorYlow(i) <<endl;
  }
  cout<<"hist denom"<<endl;
  for(int i=1;i<den_h->GetNcells()-1;++i){
    cout<<"denominator hist content "<<num_h->GetBinContent(i)<<" x "<< xaxis->GetBinCenter(i) <<endl;
  }
  cout<<"hist num"<<endl;
  for(int i=1;i<den_h->GetNcells()-1;++i){
    cout<<"numerator hist content "<<den_h->GetBinContent(i)<<" x "<< xaxis->GetBinCenter(i) <<endl;
  }


  int graphcount = 0;
  cout<<"first bin with entries "<<firstbin<<" last bin with entries "<<lastbin<<endl;
  for(int i=0; i<bins;++i){
    double num_con = num_h->GetBinContent(i+1); 
    double den_con = den_h->GetBinContent(i+1);
    if(den_con == 0 || num_con == 0) continue;
    if(scalenum->GetErrorYhigh(i)!=scalenum->GetErrorYhigh(i) ||  scalenum->GetErrorYlow(i)!=scalenum->GetErrorYlow(i)|| scaledenom->GetErrorYhigh(i)!=scaledenom->GetErrorYhigh(i) ||  scaledenom->GetErrorYlow(i)!=scaledenom->GetErrorYlow(i))continue;
    cout<<"numerator "<<num_con<<" + "<<scalenum->GetErrorYhigh(i)<<" - "<<scalenum->GetErrorYlow(i) <<" denominator "<<den_con<<" + "<< scaledenom->GetErrorYhigh(i) <<" - "<< scaledenom->GetErrorYlow(i)<<endl;    
    x[i]= xaxis->GetBinCenter(i+1);    
    y[i]= scalehist->GetBinContent(i+1);
    ylow[i]  = sqrt(pow(1/den_con*scalenum->GetErrorYlow(graphcount) ,2)+pow(num_con/(den_con*den_con)*scaledenom->GetErrorYhigh(graphcount),2));
    yhigh[i] = sqrt(pow(1/den_con*scalenum->GetErrorYhigh(graphcount),2)+pow(num_con/(den_con*den_con)*scaledenom->GetErrorYlow(graphcount) ,2));
    //ylow[i]  = fabs(num_con/den_con - (num_con+scalenum->GetErrorYhigh(graphcount)/(den_con-scaledenom->GetErrorYlow(graphcount))));
    //yhigh[i] = fabs(num_con/den_con - (num_con-scalenum->GetErrorYlow(graphcount)/(den_con+scaledenom->GetErrorYhigh(graphcount))));
    //ylow[i]  += sqrt(pow(1/den_con*scalenum->GetErrorYhigh(graphcount),2)+pow(num_con/(den_con*den_con)*scaledenom->GetErrorYlow(graphcount) ,2));
    //yhigh[i] += sqrt(pow(1/den_con*scalenum->GetErrorYlow(graphcount) ,2)+pow(num_con/(den_con*den_con)*scaledenom->GetErrorYhigh(graphcount),2));
    graphcount++;
    xlow[i]  = fabs(scalehist->GetBinLowEdge(i+1)-xaxis->GetBinCenter(i+1));
    xhigh[i] = fabs(scalehist->GetBinLowEdge(i+1) +scalehist->GetBinWidth(i+1)-xaxis->GetBinCenter(i+1));
    /*/
    double graph_x=0,graph_y=0;
    scalenum->GetPoint(i+1,graph_x,graph_y);
    cout<<"num hist "<<num_con<<" num graph content "<<graph_y<<" x "<<graph_x <<endl;
    /*/
    
    cout<<"iter "<<i<<" x "<<x[i]<<" x low "<<xlow[i]<<" x high "<<xhigh[i] <<" y "<<y[i]<<" low "<<ylow[i]<<" high "<<yhigh[i]<<endl;

    if(i==firstbin){
      nominal += "(("+val+"<="+to_string(boost::math::round(xhigh[i]+x[i]))+")*"+to_string(y[i])+")";
      up_var  += "(("+val+"<="+to_string(boost::math::round(xhigh[i]+x[i]))+")*"+to_string(y[i]+yhigh[i])+")";
      down_var+= "(("+val+"<="+to_string(boost::math::round(xhigh[i]+x[i]))+")*"+to_string(y[i]-ylow[i])+")"; 
    }
    else if(i<lastbin && i>firstbin){
      nominal += "+(("+val+">"+to_string(boost::math::round(x[i]-xlow[i]))+"&& "+val+"<="+to_string(boost::math::round(x[i]+xhigh[i]))+")*"+to_string(y[i])+")";
      up_var  += "+(("+val+">"+to_string(boost::math::round(x[i]-xlow[i]))+"&& "+val+"<="+to_string(boost::math::round(x[i]+xhigh[i]))+")*"+to_string(y[i]+yhigh[i])+")";
      down_var+= "+(("+val+">"+to_string(boost::math::round(x[i]-xlow[i]))+"&& "+val+"<="+to_string(boost::math::round(x[i]+xhigh[i]))+")*"+to_string(y[i]-ylow[i])+")";
    }
    else if(i==lastbin){
      nominal += "+(("+val+">"+to_string(boost::math::round(x[i]-xlow[i]))+")*"+to_string(y[i])+")";
      up_var  += "+(("+val+">"+to_string(boost::math::round(x[i]-xlow[i]))+")*"+to_string(y[i]+yhigh[i])+")";
      down_var+= "+(("+val+">"+to_string(boost::math::round(x[i]-xlow[i]))+")*"+to_string(y[i]-ylow[i])+")";

    }
    boost::replace_all(nominal , ".000000", ".");
    boost::replace_all(up_var  , ".000000", ".");
    boost::replace_all(down_var, ".000000", ".");

  }
  nominal = "("+nominal+")";
  up_var= "("+up_var+")";
  down_var= "("+down_var+")"; 
  
  //nominal= nominal.substr(1);
  //up_var=up_var.substr(1);
  //down_var=down_var.substr(1);
  //cout<<nominal<<endl;
  //cout<<up_var<<endl;
  //cout<<down_var<<endl;
  
  return new TGraphAsymmErrors(bins,x,y, xlow, xhigh,ylow,yhigh);
}



void effiPlots::drawDistri(TH1F* hist, double max){
  /*
  gStyle->SetPadTickY(0);
  get_can()->UseCurrentStyle();
  get_can()->Update();
  */
  if(debug)cout<<hist->GetMaximum()<<endl;;
  Float_t rightmax = hist->GetMaximum();rightmax*=1.3;
  //Float_t scale = 1.1/rightmax;
  //hist->SetFillColor(kBlack);
  hist->Scale(1/rightmax);
  hist->SetLineColor(kBlack);
  //hist->DrawNormalized("histsame");
  hist->Draw("sameY+");
  //draw an axis on the right side

  /*  
  TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
			    gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,50510,"+L");
  //axis->Set

  axis->SetLineColor(kBlack);
  axis->SetTitle("Events");
  axis->SetLabelColor(kBlack);
  axis->Draw();
  */
}


void effiPlots::imposeHist(bool impose_switch){
  imposeDistri=impose_switch;
}
