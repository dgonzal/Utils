#include "TreeHists.h"

using namespace std;

TreeHists::TreeHists(string saveName): HistsBase(saveName){
  normArea=false;
  max=-1;

  pad1 = NULL;
  pad2 = NULL;
  legx1=0.6; legx2= 0.92; legy1=0.6; legy2=0.86;
  
}

bool TreeHists::Draw(string variable, string draw_option, string binning, std::string x_axis, st::string y_axis){
  vector<vector<TH1F*>> empty_swapvec;
  error_histos.swap(empty_swapvec);
  histos.clear();

  if(treeName.empty()){
      cerr<<"There is no name for the Tree given"<<endl;
      return false;
  }
  if(variable.empty()){
    cerr<<"There is no name for the variable given, which should be plotted"<<endl;
    return false;
  }
  vector<string> hist_draw_options = get_draw_options();
  std::vector<bool> stackInfo = get_stackInfo();
  std::vector<int> histColors = get_histColors();
  std::vector<int> histMarker = get_histMarker();
  std::vector<std::string> nicknames = get_nicknames();
  
  for(unsigned int i=0; i< error_weights.size();i++){
    vector<TH1F*> tmp_vec;
    error_histos.push_back(tmp_vec); 
  }
  Long_t id, size, flag, modtime;
  int counter= -1;
  for(const auto & fileDir : get_filedirs()){
    counter++;
    if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
      cerr<<"File "<<fileDir.c_str()<<" does not exist"<<endl;
      return false;
    }
    TFile* file = new TFile(fileDir.c_str());
    if(file->IsZombie()){
      cerr<<"File "<<fileDir<<" is a Zombie"<<endl;
      return false;
    }
    TTree* mytree = (TTree*)file->Get(treeName.c_str());
    if(binning.empty()){
      mytree->Draw((variable+">>myTmpHist").c_str(),draw_option.c_str());
    }
    else{
      mytree->Draw((variable+">>myTmpHist("+binning+")").c_str(),draw_option.c_str());
    }
    TH1F* myTmpHist = (TH1F*)gPad->GetPrimitive("myTmpHist");
    histos.push_back(myTmpHist);

    if(!stackInfo.at(counter)) continue;

    for(unsigned int i = 0; i < error_weights.size();i++){
      for(auto number : replace_strings[i]){
	string error = error_weights[i];
	if(boost::algorithm::contains(error,"REPLACE"))
	  boost::replace_all(error,"REPLACE",number);
	if(binning.empty()){
	  mytree->Draw((variable+">>myTmpHist").c_str(),(error+"*"+draw_option).c_str());
	}
	else{
	  mytree->Draw((variable+">>myTmpHist("+binning+")").c_str(),(error+"*"+draw_option).c_str());
	}
	TH1F* myTmpHist = (TH1F*)gPad->GetPrimitive("myTmpHist");
	if(myTmpHist->GetSumOfWeights()==0){
	  if(binning.empty()){
	    mytree->Draw((variable+">>myTmpHist").c_str(),(draw_option).c_str());
	  }
	  else{
	    mytree->Draw((variable+">>myTmpHist("+binning+")").c_str(),(draw_option).c_str());
	  }
	  myTmpHist = (TH1F*)gPad->GetPrimitive("myTmpHist");
	}
	error_histos[i].push_back(myTmpHist);
      }
    }
    delete file;
  }
  bool first_draw =false;
  bool stack_exists = false;
  double histo_max =-9999999;

  leg = new TLegend(legx1, legy1, legx2, legy2);
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  
  THStack *hs = new THStack("hs","");
  for(unsigned int i =0; i<histos.size(); i++){
    TH1F* hist = histos[i];
    if(histo_max < hist->GetMaximum()) histo_max = hist->GetMaximum(); 
    if(histColors.at(i)!= -1){
      if(stackInfo.at(i))hist->SetFillColor(histColors.at(i));
      hist->SetLineColor(histColors.at(i));
    }
    if(histMarker.at(i)!= -1){
      hist->SetMarkerStyle(histMarker.at(i));
      hist->SetLineColor(histColors.at(i));
      if(histColors.at(i)!= -1) hist->SetMarkerColor(histColors.at(i));
    }
    if(stackInfo.at(i)){
      if(!nicknames.at(i).empty())
	leg->AddEntry( hist, nicknames.at(i).c_str(), "f");
      hs->Add(hist);
      stack_exists = true;
    }
    else
      if(!nicknames.at(i).empty()){
	if(histColors.at(i) ==1)
	  leg->AddEntry( hist, nicknames.at(i).c_str(), "lpe");
	else
	  leg->AddEntry( hist, nicknames.at(i).c_str(), "l");
      }
  }
  
  pad1 = new TPad("histograms","histograms",0, 0.2, 1, 0.8);
  pad2 = new TPad("ratio_errors","ratio and error",0, 0.05, 1,0.2);
  

  
  get_can()->SetCanvasSize(800,800);

  pad1->SetBottomMargin(0.04);
  pad2->SetTopMargin(0.04); 

  pad1->SetFillColor(0);
  pad2->SetFillColor(0);

  get_can()->Divide(2);
  pad1->Draw();
  pad2->Draw();


  pad1->cd();
  if(stack_exists){
    if(histo_max > hs->GetMaximum()){
      hs->SetMaximum(histo_max); 	
    }
    hs->Draw("hist");
    first_draw = true;
  }

  for(unsigned int i =0; i<histos.size(); i++){
    TH1F* hist = histos[i];
    string draw_option = hist_draw_options[i];
    if(!stackInfo.at(i)){
      if(!first_draw){
	if(histo_max > hist->GetMaximum())
	  hist->SetMaximum(histo_max); 
	first_draw =true;
	hist->Draw(draw_option.c_str());
      }
      else
	hist->Draw(("same "+draw_option).c_str());
    }
  }
  leg->Draw();
  pad2->cd();
  calc_ratio(hs,histos[0])->Draw("");
  
  TH1F* mc_stat = calc_MCstat(hs);
  TH1F* total_error = (TH1F*)(hs->GetStack()->Last())->Clone("");
  for(unsigned int i=0; i<error_weights.size();i++)
    calc_weightErr(i,methods_forerrors[i],total_error);
  if(error_weights.size()>0){
    total_error->Divide(total_error);
    total_error->SetFillColor(17);
    total_error->SetLineColor(0);
    total_error->Draw("same e2");
  }
  mc_stat->Draw("same e2");
  calc_ratio(hs,histos[0])->Draw("same");
  gPad->RedrawAxis();
  get_can()->Print(get_resultFile());

  pad1->Delete();
  pad2->Delete();

  
  return true;
}

TH1F* TreeHists::calc_ratio(THStack* stack, TH1F* hist){
  TH1F* ratio = (TH1F*) hist->Clone();
  TH1F * stack_hist = (TH1F*)(stack->GetStack()->Last())->Clone("");
  if(ratio->GetSumw2N()==0)ratio->Sumw2();
  if(stack_hist->GetSumw2N() ==0)hist->Sumw2();
  ratio->SetTitle("");
  ratio->Divide(stack_hist);
  //ratio->SetMarkerStyle(0);
  //ratio->SetMarkerSize(1);
  ratio->SetMaximum(1.8);
  ratio->SetMinimum(0);
  ratio->SetMarkerColor(1);
  ratio->SetFillColor(15);
  ratio->SetLineColor(1);
  double bottomSize = 0.3;
  ratio->GetXaxis()->SetLabelSize(0.); //ratio->GetXaxis()->SetNdivisions(100);
  ratio->GetYaxis()->SetLabelSize(0.04 / bottomSize); ratio->GetYaxis()->SetNdivisions(5);
  //ratio->GetXaxis()->SetTitleSize( 0.04 / bottomSize );
  ratio->GetYaxis()->SetTitleSize( 0.04 / bottomSize );
  ratio->GetYaxis()->SetTitleOffset( 0.34 );
  ratio->GetYaxis()->CenterTitle();
  ratio->SetTickLength( 0.01 / bottomSize );
  //ratio->GetXaxis()->SetLabelSize(0);
  //ratio->GetXaxis()->SetLabelOffset(999);
  return ratio;
}

TH1F* TreeHists::calc_MCstat(THStack* stack){
  TH1F* ratio = (TH1F*)(stack->GetStack()->Last())->Clone("ratio");
  if(ratio->GetSumw2N()==0)ratio->Sumw2();
  ratio->SetTitle("");
  ratio->Divide(ratio);
  ratio->SetMarkerColor(15);
  ratio->SetFillColorAlpha(15,0.35);
  //int numberOfBins = ratio->GetNcells();
  //for(int i = 0; i<numberOfBins; i++)
  //  cout<<ratio->GetBinContent(i)<<" "<<ratio->GetBinError(i)<<endl;
  return ratio;
}

void TreeHists::calc_weightErr(unsigned int i_error, error_method method, TH1F* result){
  TH1F* nominal = (TH1F*) result->Clone("nominal");
  int numberOfBins = error_histos[i_error][0]->GetNcells();   
  vector<TH1F*> histo_sum;
  unsigned int number_of_weights = replace_strings[i_error].size();
  //cout<<"number of error histos "<<error_histos[i_error].size()<<" number of weights "<<number_of_weights<<endl;
  for(unsigned int m =0; m < number_of_weights; m++){
    TH1F* error_histo_sum = (TH1F*)error_histos[i_error][m]->Clone();
    for(unsigned int p = number_of_weights; p < error_histos[i_error].size(); p+= number_of_weights){ 
      error_histo_sum->Add(error_histos[i_error][p+m]);
    }
    histo_sum.push_back(error_histo_sum);
  }
  if(method == envelop){
    //find envelop
    for(int i = 0; i<numberOfBins; i++){
      double max =-9999999999999;
      double min = 9999999999999;
      for(auto & hist : histo_sum){
	double content = hist->GetBinContent(i);
	//cout<<content<<endl;
	if(content > max ) max = content;
	if(content < min ) min = content;
      }
      //symmetrize and set
      double nomBin = nominal->GetBinContent(i);
      double error = 0;
      if(nomBin>0){
	//cout<<"Max "<<max<<" min "<<min<<" nom "<<nomBin<<endl;
	error = (fabs(max-nomBin)+fabs(nomBin-min))/2;
	double sum_error_w2 =  error*error + nominal->GetBinError(i)* nominal->GetBinError(i);
	result->SetBinError(i,sqrt(sum_error_w2));
      }
    }
  }
  else if(method == rms){
    for(int i = 0; i<numberOfBins; i++){
      double mean_square = 0;
      double nomBin = nominal->GetBinContent(i);
      //double histo_number = error_histos[i_error].size();
      for(auto & hist : histo_sum){
	double content = hist->GetBinContent(i);
	//cout<<"rms value "<<content<<" nominal "<<nomBin<<endl;
	mean_square+= (content-nomBin)*(content-nomBin);
      }
      //set
      if(nomBin>0){
	double sum_error_w2 = mean_square + nominal->GetBinError(i)* nominal->GetBinError(i); 
	result->SetBinError(i,sqrt(sum_error_w2));
      }
    }
  }
  else{
    cerr<<"There was a problem with the error calculation method choosen please check!"<<endl;    
  }
  
  //result->SetMarkerColor(16);
  //return result;

}

void TreeHists::AddErrorWeight(string error_string, error_method method, string replace){
  error_weights.push_back(error_string);
  methods_forerrors.push_back(method);
  if(boost::algorithm::contains(replace, ",")){ 
    vector<string> splitted_string;
    boost::split(splitted_string,replace,boost::is_any_of(","));
    replace_strings.push_back(splitted_string);
  }
  else if(boost::algorithm::contains(replace, "-")){
    vector<string> splitted_string;
    boost::split(splitted_string,replace,boost::is_any_of("-"));
    vector<string> my_ints;
    for(int i = stoi(splitted_string[0]); i < stoi(splitted_string[1])+1; i++)
      my_ints.push_back(to_string(i));
    replace_strings.push_back(my_ints);
  }
  else if(replace.empty()){
    vector<string> int_vec={to_string(-1)};  
    replace_strings.push_back(int_vec);
  }
}
