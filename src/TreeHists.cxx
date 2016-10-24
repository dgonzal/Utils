#include "TreeHists.h"

using namespace std;

TreeHists::TreeHists(string saveName): HistsBase(saveName){
  normArea=false;
  max=-1;

  pad1 = NULL;
  pad2 = NULL;
  legx1=0.6; legx2= 0.92; legy1=0.6; legy2=0.86;
  
}

TTree* TreeHists::load_tree(std::string fileDir){
  Long_t id=0, size=0, flag=0, modtime=0;
  if(gSystem->GetPathInfo(fileDir.c_str(),&id, &size, &flag, &modtime)!=0){
    std::cerr<<"File "<<fileDir.c_str()<<" does not exist"<<std::endl;
    std::cerr<<"id "<<id<<" size "<<size<<" flag "<<flag<<" modtime "<<modtime<<std::endl;
    exit(EXIT_FAILURE);
  }
  TFile* file = new TFile(fileDir.c_str());
  if(file->IsZombie()){
    cerr<<"File "<<fileDir<<" is a Zombie"<<endl;
    exit(EXIT_FAILURE);
  }
  //std::cout<<" Load Tree "<<treeName<<" from "<<fileDir<<std::endl;
  return (TTree*)file->Get(treeName.c_str());
}

vector<TH1F*> TreeHists::return_hists(string variable, string draw_option, string binning, std::string x_axis, std::string y_axis){
  vector<TH1F*> result;  
  for(const auto & fileDir : get_filedirs()){
    TTree* mytree = load_tree(fileDir);
    string file_name = "";
    result.push_back(make_hist(mytree, variable, binning, draw_option));
  }
  return result;
}


bool TreeHists::Draw(string variable, string draw_option, string binning, std::string x_axis, std::string y_axis){
  /*
  ** draw_option is in this case for the TTreeDraw command
  ** hist_draw_options governs how the histograms are drawn!!
  **
  */

  vector<vector<TH1F*>> empty_swapvec;
  error_histos.swap(empty_swapvec);
  std::vector<std::vector<TH1F*>> empty_error_folder_histos;
  error_folder_histos.swap(empty_error_folder_histos);
  histos.clear();

  if(treeName.empty()){
      cerr<<"There is no name for the Tree given"<<endl;
      return false;
  }
  if(variable.empty()){
    cerr<<"There is no name for the variable given, which should be plotted"<<endl;
    return false;
  }
  std::vector<std::string> hist_draw_options = get_draw_options();
  std::vector<bool> stackInfo = get_stackInfo();
  std::vector<int> histColors = get_histColors();
  std::vector<int> histMarker = get_histMarker();
  std::vector<std::string> nicknames = get_nicknames();
  std::vector<double> uncertainties = get_uncertainties();

  for(unsigned int i=0; i< error_weights.size();i++){
    vector<TH1F*> tmp_vec;
    error_histos.push_back(tmp_vec); 
  }
  Long_t id, size, flag, modtime;

  int counter= -1;
  std::vector<std::vector<TTree*>> error_trees;
  for(unsigned int i=0;i<error_folders.size();i++)
    for(unsigned int m=0;m<error_folders[i].size();m++)
      error_trees.push_back(std::vector<TTree*>());

  for(const auto & fileDir : get_filedirs()){
    counter++;
    TTree* mytree = load_tree(fileDir);
    string file_name = "";
    if(error_folders.size()> 0 && !boost::algorithm::contains(fileDir,".DATA.") && stackInfo[counter]){
      vector<string> splitted_string;
      boost::split(splitted_string,fileDir,boost::is_any_of("/"));
      file_name = splitted_string.at(splitted_string.size()-1);
      for(unsigned int i =0; i<error_folders.size();i++){
	for(unsigned int m=0;m<error_folders[i].size();m++){
	  if(debug)std::cout<<"storing tree "<<error_folders[i][m]+file_name<<" into tree "<<i+m <<std::endl;
	  error_trees[i+m].push_back(load_tree(error_folders[i][m]+file_name));
	  continue;
	}
      }
    }
    
    histos.push_back(make_hist(mytree, variable, binning, draw_option));
    if(!stackInfo.at(counter)) continue;
    //int count_per_error =0;
    for(unsigned int i = 0; i < error_weights.size();i++){
      TH1F* myTmpHist;
      for(auto number : replace_strings[i]){
	string error = error_weights[i];
	if(boost::algorithm::contains(error,"REPLACE"))
	  boost::replace_all(error,"REPLACE",number);
	myTmpHist = make_hist(mytree, variable, binning, error+"*"+draw_option);
	if(myTmpHist->GetSumOfWeights()<=0){
	  myTmpHist = make_hist(mytree, variable, binning, draw_option);
	}
	error_histos[i].push_back(myTmpHist);
      }
    }
  }
  
  for(unsigned int i=0; i<error_trees.size();i++){
    error_folder_histos.push_back(std::vector<TH1F*>());
    for(auto tree : error_trees[i])
      error_folder_histos[i].push_back(make_hist(tree, variable, binning, draw_option));  
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
  //TH1F* flat_uncertainties = histos[0]
  for(unsigned int i =0; i<histos.size(); i++){
    TH1F* hist = histos[i];
    //if(uncertainties[i]>0)
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

  pad1 = new TPad("histograms","histograms",x1, y2, x2, y3);
  pad2 = new TPad("ratio_errors","ratio and error", x1, y1, x2, y2);
  
  get_can()->SetCanvasSize(800,800);

  //pad1->SetTopMargin(0.1); pad1->SetLeftMargin(0.19);
  pad1->SetTopMargin(0.1);pad1->SetBottomMargin(0.0);pad1->SetLeftMargin(0.19);pad1->SetRightMargin(0.1);
  //pad2->SetTopMargin(0.04); pad2->SetLeftMargin(0.19);
  pad2->SetTopMargin(0.0); pad2->SetBottomMargin(0.25);  pad2->SetLeftMargin(0.19); pad2->SetRightMargin(0.1);
  
  pad1->SetFillColor(0);
  pad2->SetFillColor(0);

  get_can()->Divide(2);
  pad1->Draw();
  pad2->Draw();

  //calculate total error before plotting anything
  TH1F* total_error = (TH1F*)(hs->GetStack()->Last())->Clone("total_error");
  for(unsigned int i=0; i<error_weights.size();i++)
    calc_weightErr(i,methods_forerrors[i],total_error);  
  calc_fixedErr(uncertainties,stackInfo, total_error,hs);
  for(unsigned int i=0; i<error_folders.size();i+=2)
    calc_errorfolder({i,i+1},total_error,(TH1F*)(hs->GetStack()->Last())->Clone("tmp_nominal"));
  
  TH1F* error_stack_hist = (TH1F*)total_error->Clone("my_grey_errors");

  //start plotting
  pad1->cd();

  if(stack_exists){
    double max_bin_value_with_error = error_stack_hist->GetBinContent(error_stack_hist->GetMaximumBin())+error_stack_hist->GetBinError(error_stack_hist->GetMaximumBin());
    if(histo_max > max_bin_value_with_error){
      hs->SetMaximum(histo_max); 	
    }
    else{
      hs->SetMaximum(max_bin_value_with_error);
    }   
    hs->SetMinimum(0.0001);
    hs->Draw("hist");
    //plot grey error bands 
    static Int_t LightGray     = TColor::GetColor( "#aaaaaa" );
    error_stack_hist->SetFillColor(LightGray);
    error_stack_hist->SetLineWidth(1);
    error_stack_hist->SetFillStyle(3245);
    error_stack_hist->Draw("same e2");
    if(!y_axis.empty()){
      hs->GetYaxis()->SetTitleSize(0.06);
      hs->GetYaxis()->SetTitle(y_axis.c_str());
      get_can()->Modified();
    }
    first_draw = true;
  }
  //plot data and signal
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
  
  //calculate ratios and its error, use previous calculation
  TH1F* ratio = calc_ratio(hs,histos[0]);
  if(!x_axis.empty()){
    //ratio->GetXaxis()->SetLabelSize(15);
    ratio->GetXaxis()->SetTitle(x_axis.c_str());
  }
  //the two times draw of ratio is just bevause it sets the default values for the drawing! could be changed
  ratio->Draw("");

  TH1F* mc_stat = calc_MCstat(hs);
  if(error_weights.size()>0){
    total_error->Divide(total_error);
    total_error->SetFillColor(17);
    total_error->SetLineColor(0);
    total_error->Draw("same e2");
  }
  mc_stat->Draw("same e2");
  ratio->Draw("same e0");
  gPad->RedrawAxis();  
  get_can()->Update();
  get_can()->Print(get_resultFile());

  pad1->Delete();
  pad2->Delete();
  
  return true;
}

TH1F* TreeHists::calc_ratio(THStack* stack, TH1F* hist){
  TH1F* ratio = (TH1F*) hist->Clone("data_ratio");
  TH1F * stack_hist = (TH1F*)(stack->GetStack()->Last())->Clone("");
  if(ratio->GetSumw2N()==0)ratio->Sumw2();
  if(stack_hist->GetSumw2N() ==0)hist->Sumw2();
  ratio->SetTitle("");
  ratio->Divide(stack_hist);
  
  for(int i = 0; i< ratio->GetNcells();i++){
    if(ratio->GetBinContent(i) ==0)
      ratio->SetBinContent(i,-1);
  }
  ratio->GetYaxis()->SetRangeUser(0,1.99);
  ratio->SetMarkerColor(1);
  ratio->SetFillColor(15);
  ratio->SetLineColor(1);
  double bottomSize = 0.273;
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetXaxis()->SetLabelSize(0.03/bottomSize); //ratio->GetXaxis()->SetNdivisions(100);
  ratio->GetXaxis()->SetTitleOffset(0.2/bottomSize);
  ratio->GetYaxis()->SetLabelSize(0.03/bottomSize); ratio->GetYaxis()->SetNdivisions(505);
  ratio->GetXaxis()->SetTitleSize( 0.04/bottomSize);
  ratio->GetYaxis()->SetTitleSize( 0.13);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->CenterTitle();
  ratio->SetTickLength( 0.03 / bottomSize );
 
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
  unsigned int number_of_weights = replace_strings[i_error].size() > 0 ? replace_strings[i_error].size() : error_folders[i_error].size();
  //cout<<"number of error histos "<<error_histos[i_error].size()<<" number of weights "<<number_of_weights<<endl;
  for(unsigned int m =0; m < number_of_weights; m++){
    TH1F* error_histo_sum = (TH1F*)error_histos[i_error][m]->Clone();
    for(unsigned int p = number_of_weights; p < error_histos[i_error].size(); p+= number_of_weights){ 
      error_histo_sum->Add(error_histos[i_error][p+m]);
      if(debug)cout<<"number of hist "<<p<<" content i==10 "<<error_histos[i_error][p+m]->GetBinContent(10)<<endl;
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
      //cout<<"nominal "<<nomBin<<" max "<<max<< " min "<<min<< " error "<<(fabs(max-nomBin)+fabs(nomBin-min))/2 <<endl;
      //if(max==-1 || min ==-1) continue;
      if(nomBin>0){
	if(max!=-1 || min !=-1) error = (fabs(max-nomBin)+fabs(nomBin-min))/2;
	//cout<<error<<endl;
	//error = sqrt(((max-nomBin)*(max-nomBin)+(nomBin-min)*(nomBin-min))/2);
	double sum_error_w2 =  error*error + nominal->GetBinError(i)*nominal->GetBinError(i);
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
void TreeHists::AddErrorFolder(std::vector<std::string> folder_name){
  error_folders.push_back(folder_name); 
}

void TreeHists::calc_fixedErr(const std::vector<double> & uncertainties, const std::vector<bool> & stacked, TH1F* result, THStack* stack){
  int counter = 0;
  for(unsigned int i =0; i<uncertainties.size();i++){
    if(!stacked.at(i))continue;
    TH1F* histo_in_stack = (TH1F*) stack->GetStack()->At(counter);
    if(uncertainties.at(i)>0){
      double sys = uncertainties.at(i);
      for(int m = 0; m< histo_in_stack->GetNcells(); m++){
	double bincontent = histo_in_stack->GetBinContent(m);
	double binerror = result->GetBinError(m);
	//cout<<"new error "<<sys*bincontent <<" old "<<binerror<< " total "<<sqrt((sys*sys*bincontent*bincontent)+binerror*binerror)<<endl;
	result->SetBinError(m,sqrt((sys*sys*bincontent*bincontent)+binerror*binerror));
      }
    }
  }
}

TH1F* TreeHists::make_hist(TTree* mytree, std::string variable, std::string binning, std::string draw_option){
  if(binning.empty()){
    mytree->Draw((variable+">>myTmpHist").c_str(),draw_option.c_str());
  }
  else{
    mytree->Draw((variable+">>myTmpHist("+binning+")").c_str(),draw_option.c_str());
    //mytree->Scan(variable.c_str(),draw_option.c_str());
  }
  return (TH1F*)gPad->GetPrimitive("myTmpHist");
}


void TreeHists::calc_errorfolder(std::vector<unsigned int> is, TH1F* result, TH1F* nominal){
  debug = true;
  
  for(auto i : is)
    cout<<i<<" size "<<error_folder_histos.at(i).size()<<endl;

  for(auto i : is){
    if(debug)std::cout<<i<<std::endl;
    error_folder_histos.at(i).at(0)->SetName(("errorhelper_n-th_"+to_string(0)+"_of_"+to_string(i)).c_str());
    if(debug)std::cout<<error_folder_histos.at(i).at(0)->GetName()<<std::endl;
    for(unsigned int m=1;m<error_folder_histos.at(i).size();m++){
      TH1F* tmphist = (TH1F*) error_folder_histos.at(i).at(m)->Clone(("n-th_"+to_string(m)+"_of_"+to_string(i)).c_str());
      if(debug) std::cout<<tmphist->GetName()<<std::endl;
      error_folder_histos.at(i).at(0)->Add(tmphist);
    }
    for(int m=0;m<error_folder_histos.at(i).at(0)->GetNcells();m++){
      error_folder_histos.at(i).at(0)->SetBinError(m,error_folder_histos.at(i).at(0)->GetBinContent(m));
      error_folder_histos.at(i).at(0)->SetBinContent(m,nominal->GetBinContent(m));
      if(debug)std::cout<<"Bin Content nom "<<nominal->GetBinContent(m)<<" error trees sum "<< error_folder_histos.at(is[0]).at(0)->GetBinContent(m)<<std::endl;
    }
    //error_folder_histos.at(i).at(0)->Add(nominal,-1);
    error_folder_histos.at(i).at(0)->Divide(nominal);
  }
  unsigned int numberOfBins =  result->GetNcells();
  if(debug)std::cout<<"got number of cells "<<numberOfBins<<std::endl;
  for(unsigned int i=0; i< numberOfBins;i++){
    double err = 0;
    for(auto m : is)
      err += fabs(error_folder_histos.at(m).at(0)->GetBinError(i))/is.size();
    if(debug)std::cout<<"error for bin "<<i<<": "<<err<<" total error in bin "<<result->GetBinContent(i) <<std::endl;
    result->SetBinError(i,sqrt(result->GetBinContent(i)*result->GetBinContent(i)+err*err));
  }
  if(debug)std::cout<<"done with error tree"<<std::endl;
  debug = false;
}
