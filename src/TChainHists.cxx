#include "TChainHists.h"

using namespace std;
/*/
//From -pi<phi<pi to 0<phi<2pi convention 
std::string deltaPhistring(std::string var1, std::string var2){
  return "sqrt(std::fmod((("+var1+".Phi()+3.1415)-("+var2+".Phi()+3.1415)),3.1415)*std::fmod((("+var1+".Phi()+3.1415)-("+var2+".Phi()+3.1415)),3.1415))";
  //return "(("+var1+".Eta()-"+var2+".Eta())*("+var1+".Eta()-"+var2+".Eta())+(("+var1+".Phi()-"+var2+".Phi())*("+var1+".Phi()-"+var2+".Phi())))";
}
std::string deltaEtastring(std::string var1, std::string var2){
  return "sqrt(("+var1+".Eta()-"+var2+".Eta())*("+var1+".Eta()-"+var2+".Eta()))";
}
std::string deltaRstring(std::string var1, std::string var2){
 return "sqrt(("+var1+".Eta()-"+var2+".Eta())*("+var1+".Eta()-"+var2+".Eta())+(std::fmod("+var1+".Phi()-"+var2+".Phi(),3.1415)*std::fmod("+var1+".Phi()-"+var2+".Phi(),3.1415)))";
 //return "(("+var1+".Eta()-"+var2+".Eta())*("+var1+".Eta()-"+var2+".Eta())+(("+var1+".Phi()-"+var2+".Phi())*("+var1+".Phi()-"+var2+".Phi())))";
}
/*/


std::vector<std::string> glob(const std::string& pat){
  using namespace std;
  glob_t glob_result;
  cout<<"pattern "<<pat<<endl;
  
  glob(pat.c_str(),0,NULL,&glob_result);
  vector<string> ret;
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
    ret.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  return ret; 
}



ChainHists::ChainHists(string saveName, bool single): HistsBase(saveName, single){
  normArea=false;
  max=-1;
  pad1 = NULL;
  pad2 = NULL;
  legx1=0.6; legx2= 0.92; legy1=0.6; legy2=0.9;
  
}

TChain* ChainHists::load_chain(std::string fileDir){
  //Long_t id=0, size=0, flag=0, modtime=0; 
  //cout<<(fileDir+"_"+to_string(page_number)).c_str()<<endl;
  TChain* chain = new TChain(fileDir.c_str());//(fileDir+"_"+to_string(page_number)).c_str());
  chain->Add((fileDir+"/"+treeName).c_str());
  /*/
  for(auto file_iter: glob(fileDir)){
    cout<<"add file to chain "<<file_iter<<endl;
    
    if(gSystem->GetPathInfo(file_iter.c_str(),&id, &size, &flag, &modtime)!=0){
      std::cerr<<"File "<<file_iter.c_str()<<" does not exist"<<std::endl;
      std::cerr<<"id "<<id<<" size "<<size<<" flag "<<flag<<" modtime "<<modtime<<std::endl;
      exit(EXIT_FAILURE);
    }
    chain->AddFile(file_iter.c_str(),TTree::kMaxEntries,treeName.c_str());
    //chain->Add((file_iter+"/"+treeName).c_str());
    
    }
  /*/
  //exit(0);
  //assert(1==0);
  return chain;
}

vector<TH1F*> ChainHists::return_hists(string variable, string draw_option, string binning, std::string x_axis, std::string y_axis){
  vector<TH1F*> result;  
  for(const auto & fileDir : get_filedirs()){
    TChain* mychain = load_chain(fileDir);
    string file_name = "";
    result.push_back(make_hist(mychain, variable, binning, draw_option));
  }
  return result;
}


bool ChainHists::Draw(string variable, string draw_option, string binning, std::string x_axis, std::string y_axis, bool legend){
  /*
  ** draw_option is in this case for the TTreeDraw command
  ** hist_draw_options governs how the histograms are drawn!!
  **
  */  
  
  //Free memory and clean containers to fill
  error_histos =  std::vector<std::vector<TH1F*>> ();
  error_folder_histos = std::vector<std::vector<TH1F*>>();
  histos = std::vector<TH1F*>();


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
  std::vector<double> uncertainties_stack;
  std::vector<double> scale = get_scalefactors();
  bool single_plots = get_single();

  TString singlefile_result;
  page_number++;
  if(single_plots){
    if(page_number<ignore_pages || (page_number>igup_pages && igup_pages !=-1)){
      cout<<"Going to ignore page "<<page_number<<endl;
      return false;
    }
    TString filestring = get_resultFile()+to_string(page_number).c_str()+".pdf";
    filestring.ReplaceAll("/","_");
    //boost::replace_all(filestring,"/","_");
    //too complicated instead use page_1,page_2....
    /*
    string filestring =variable+"_"+draw_option+"_"+binning+".ps";
    boost::replace_all(filestring," ","");
    boost::replace_all(filestring,",","_");
    boost::replace_all(filestring,"+","");
    boost::replace_all(filestring,"-","minus");
    boost::replace_all(filestring,"&&","AND");
    boost::replace_all(filestring,"||","OR");
    boost::replace_all(filestring,"$","");
    */
    singlefile_result = get_resultFile()+filestring;
    cout<<"Single File name "<<singlefile_result<<endl;
    //get_can()->Print(singlefile_result+"[");
  }

  for(unsigned int i=0; i< error_weights.size();i++){
    vector<TH1F*> tmp_vec;
    error_histos.push_back(tmp_vec); 
  }
  int counter= -1;
  //std::vector<std::vector<TTree*>> error_trees;

  std::vector<chain_folderinfo> error_chains_info;
  for(unsigned int i=0;i<error_folders.size();i++)
    error_chains_info.push_back(chain_folderinfo());
  /*
  for(unsigned int i=0;i<error_folders.size();i++)
    for(unsigned int m=0;m<error_folders[i].size();m++)
      error_chains.push_back(std::vector<TChain*>());
  */

  for(const auto & fileDir : get_filedirs()){
    if(chainload_once == true && chainloaded ==true)
	break;
    //cout<<"var: "<<variable<<endl;
    counter++;
    TChain* mychain = load_chain(fileDir);
    string file_name = "";
    if(error_folders.size()> 0 && !boost::algorithm::contains(fileDir,".DATA.") && stackInfo[counter] && !mcratio){
      vector<string> splitted_string;
      boost::split(splitted_string,fileDir,boost::is_any_of("/"));
      file_name = splitted_string.at(splitted_string.size()-1);
      for(unsigned int i =0; i<error_folders.size();i++){
	for(unsigned int m=0;m<error_folders[i].size();m++){
	  if(debug)std::cout<<"storing chain "<<error_folders[i][m]+file_name<<std::endl;
	  //error_chains[i+m].push_back(load_chain(error_folders[i][m]+file_name));
	  //std::cout<<"storing chain "<<error_folders[i][m]+file_name<<" into chain "<<i+m <<std::endl;
	  //std::cout<<"i "<<i<<" m "<<m<<std::endl;
	  if(m==0){
	    if(debug)cout<<error_folders[i][m]+file_name<<endl;
	    error_chains_info[i].dir_up=error_folders[i][m];
	    error_chains_info[i].error_chains_up.push_back(load_chain(error_folders[i][m]+file_name));
	  }
	  else if(m==1){
	    if(debug)cout<<error_folders[i][m]+file_name<<endl;
	    error_chains_info[i].dir_down=error_folders[i][m];
	    error_chains_info[i].error_chains_down.push_back(load_chain(error_folders[i][m]+file_name));
	  }
	  else{
	    cout<<"more then up and down is not implemented for folder uncertainties!"<<endl;
	    assert(1==0);
	  }
	  continue;
	}
      }
    }
    string modified_draw_option = draw_option+"*"+to_string(scale[counter]);
    if(draw_option.empty()) modified_draw_option = to_string(scale[counter]);
    if(debug)cout<<"main draw options "<< modified_draw_option<< " binning "<<binning<<" draw options "<<modified_draw_option <<endl;
    histos.push_back(make_hist(mychain, variable, binning, modified_draw_option));
    if(debug)cout<<"main entries "<<histos[histos.size()-1]->GetEntries()<<endl;
    if(!stackInfo.at(counter) || mcratio) continue;
    //int count_per_error =0;
    for(unsigned int i = 0; i < error_weights.size();i++){
      TH1F* myTmpHist;
      for(auto number : replace_strings[i]){
	string error = error_weights[i];
	if(boost::algorithm::contains(error,"REPLACE"))
	  boost::replace_all(error,"REPLACE",number);
	myTmpHist = make_hist(mychain, variable, binning, error+"*"+draw_option);
	if(debug)cout<<variable<<" "<< binning<<" "<< error+"*"+modified_draw_option<<" "<<myTmpHist->GetSumOfWeights()<<endl;
	if(myTmpHist->GetSumOfWeights()<=0 || myTmpHist->GetSumOfWeights()!=myTmpHist->GetSumOfWeights()){
	  if(methods_forerrors[i]==rms) 
	    myTmpHist = make_hist(mychain, variable, binning, draw_option+"* 0");
	  else
	    myTmpHist = make_hist(mychain, variable, binning, draw_option);

	}
	error_histos[i].push_back(myTmpHist);
      }
      //myTmpHist->Delete();
    }
    //mychain->Reset();//Delete();
  }
  if(chainload_once==true)
	chainloaded=true;

  int tmp_count = -2;
  //cout<<"error_chains_info.size() "<<error_chains_info.size()<<endl;
  // for(unsigned int i=0; i<error_chains.size();i++){
  for(unsigned int i=0; i<error_chains_info.size();++i){
    tmp_count+=2;
    chain_folderinfo chain_infos = error_chains_info[i];
    error_folder_histos.push_back(std::vector<TH1F*>());//up
    for(auto up_chain : chain_infos.error_chains_up){
      std::string errorfolder_variable = variable;
      for(unsigned int m=0; m<alias.size();++m){
	if(conditions[m].empty())
	  boost::replace_all(errorfolder_variable,oldnames[m],alias[m]);
	else{
	  if(boost::algorithm::contains(chain_infos.dir_up,conditions[m])){
	    boost::replace_all(errorfolder_variable,oldnames[m],alias[m]);
	  }
	}
      }
      if(debug)cout<<"Draw command up: "<<errorfolder_variable<<" binning "<<binning<<" draw options "<<draw_option <<endl;
      error_folder_histos[tmp_count].push_back(make_hist(up_chain, errorfolder_variable, binning, draw_option));
      if(debug)cout<<"error folder entries "<<error_folder_histos[tmp_count].at(error_folder_histos[tmp_count].size()-1)->GetEntries()<<endl;
    }
    error_folder_histos.push_back(std::vector<TH1F*>());//down
    for(auto down_chain : chain_infos.error_chains_down){
      std::string errorfolder_variable = variable;
      for(unsigned int m=0; m<alias.size();++m){
	if(conditions[m].empty())
	  boost::replace_all(errorfolder_variable,oldnames[m],alias[m]);
	else{
	  if(boost::algorithm::contains(chain_infos.dir_down,conditions[m])){
	    boost::replace_all(errorfolder_variable,oldnames[m],alias[m]);
	  }
	}
      }
      //cout<<"Draw command down: "<<errorfolder_variable<<endl;
      //cout<<errorfolder_variable<<" "<<binning<<" "<<draw_option <<endl;
      error_folder_histos[tmp_count+1].push_back(make_hist(down_chain, errorfolder_variable, binning, draw_option)); 
    }
  }
  //cout<<"============"<<endl;
  

  //cleanup the tchain->draw stuff
  get_can()->Clear();
  //free error chains
  //error_chains = std::vector<std::vector<TChain*>> ();
  //free files and close them
  //for(int i =0; i<gROOT->GetListOfFiles()->GetSize();++i)
  //  gROOT->GetListOfFiles()->RemoveAt(i);

  bool first_draw =false;
  bool stack_exists = false;
  double histo_max =-9999999;

  leg = new TLegend(legx1, legy1, legx2, legy2);
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(.05);
  leg->SetFillStyle(0);
  
  THStack *hs = new THStack("hs","");
  //TH1F* flat_uncertainties = histos[0]
  for(unsigned int i =0; i<histos.size(); i++){
    TH1F* hist = histos[i];
    /*
    if(draw_ratio){ 
      hist->GetXaxis()->SetLabelOffset(999);
      hist->GetXaxis()->SetLabelSize(0);
    }
    */

    //if(uncertainties[i]>0)
    if(histo_max < hist->GetMaximum()+hist->GetBinError(hist->GetMaximumBin())) histo_max = hist->GetMaximum()+hist->GetBinError(hist->GetMaximumBin()); 
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
      uncertainties_stack.push_back(uncertainties[i]);
      stack_exists = true;
    }
    else
      if(!nicknames.at(i).empty()){
	if(histColors.at(i) ==1)
	  leg->AddEntry( hist, nicknames.at(i).c_str(), "lpe");
	else
	  leg->AddEntry( hist, nicknames.at(i).c_str(), "l");
      }
    //hist->Delete();
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
  if(!draw_ratio)get_can()->SetCanvasSize(800,600);
  get_can()->Update();
 
  pad1->SetFillColor(0);
  pad2->SetFillColor(0);

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
    

  //calculate total error before plotting anything
  TH1F* total_error;  
  if(hs->GetNhists()>0){
    total_error= (TH1F*)(hs->GetStack()->Last())->Clone("total_error");
  }
  else{
    total_error =  (TH1F*)histos[0]->Clone("total_error");
    for(int i=0;i<total_error->GetNbinsX()+2;++i){
      total_error->SetBinError(i,0);
      total_error->SetBinContent(i,0);
    } 
  }
  TH1F* zero_error = (TH1F*)total_error->Clone("zero_error");

  for(int m=0;m< zero_error->GetNcells();m++)
    zero_error->SetBinError(m,0);


  if(!mcratio){
    for(unsigned int i=0; i<error_weights.size();i++)
      calc_weightErr(i,methods_forerrors[i],total_error);  
    calc_fixedErr(uncertainties_stack,stackInfo, total_error,hs);  
    for(unsigned int i=0; i<error_folders.size();i+=2)
      calc_errorfolder({i,i+1},total_error,(TH1F*)(hs->GetStack()->Last())->Clone("tmp_nominal"));
  }
  TH1F* error_stack_hist = (TH1F*)total_error->Clone("my_grey_errors");
  

  //start plotting
  pad1->cd();

  if(stack_exists){
    double max_bin_value_with_error = error_stack_hist->GetBinContent(error_stack_hist->GetMaximumBin())+error_stack_hist->GetBinError(error_stack_hist->GetMaximumBin());
    if(histo_max > max_bin_value_with_error){
      hs->SetMaximum(histo_max*1.2); 	
    }
    else{
      hs->SetMaximum(max_bin_value_with_error*1.2);
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
    if(draw_ratio){ 
      hs->GetXaxis()->SetLabelOffset(999);
      hs->GetXaxis()->SetLabelSize(0);
      get_can()->Modified();
    }
    else{
      hs->GetXaxis()->SetTitle(x_axis.c_str());
      hs->GetYaxis()->SetTitleSize(0.13);
    }
    first_draw = true;
    gPad->Update();
    get_can()->Update();
  }
  //plot data and signal
  for(unsigned int i =0; i<histos.size(); i++){
    TH1F* hist = histos[i];
    hist->SetLineWidth(3);
    string h_draw_option = hist_draw_options[i];
    if(draw_ratio && !first_draw){ 
      hist->GetXaxis()->SetLabelOffset(999);
      hist->GetXaxis()->SetLabelSize(0);
    }
    if(!draw_ratio && !first_draw){
      hist->GetYaxis()->SetTitleSize(0.13);;
    }
    if(!stackInfo.at(i)){
      if(!first_draw){
	hist->SetTitle("");
	if(!y_axis.empty())
	  hist->GetYaxis()->SetTitle(y_axis.c_str());
	if(!x_axis.empty())
	  hist->GetXaxis()->SetTitle(x_axis.c_str());
	if(histo_max > hist->GetMaximum())
	  hist->SetMaximum(histo_max*1.2); 
	first_draw =true;
	hist->Draw(h_draw_option.c_str());
      }
      else
	hist->Draw(("same "+h_draw_option).c_str());
    }
    //hist->Delete();
  }

  histos[0]->Draw(("same "+hist_draw_options[0]).c_str());
  if(legend)leg->Draw();

  if(draw_ratio){ 
    pad2->cd();
    //calculate ratios data/mc and its error, use previous calculation
    TH1F* ratio = calc_ratio(zero_error,histos[0]);
    if(!x_axis.empty()){
      //ratio->GetXaxis()->SetLabelSize(15);
      ratio->GetXaxis()->SetTitle(x_axis.c_str());
    }
    ratio->SetMinimum(0.0001);
    
    if(mcratio){
      for(int i=0; i<ratio->GetNbinsX()+2;++i){
	ratio->SetBinError(i,0);
	ratio->SetBinContent(i,-2);
      }
    }

    //drawing two times the ratio is just because it sets the default values! could be changed
    ratio->Draw("");
    TH1F* mc_stat = calc_MCstat(hs);
    if(error_weights.size()>0 || error_folder_histos.size() >0){
      total_error->Divide(zero_error);//total_error);
      total_error->SetFillColor(17);
      total_error->SetLineColor(0);
      total_error->Draw("same e2");
    }
    mc_stat->Draw("same e2");
    ratio->Draw("same e0");
    pad2->RedrawAxis();  
    get_can()->Update();
    //ratio->Delete();
    //mc_stat->Delete();
  }

  pad1->cd();
  pad1->Update();
  pad1->RedrawAxis();
  printText();
  get_can()->Update();
 
  if(single_plots){
    get_can()->Print(singlefile_result);
  }
  else
    get_can()->Print(get_resultFile());
  
  //Delete histos and stacks that I don't need anymore
  total_error->Delete();
  zero_error->Delete();
  error_stack_hist->Delete();
  hs->Delete();
  leg->Delete();
  //Clear and delete pads and canvas
  pad1->Clear();
  pad2->Clear();
  pad1->Delete();
  pad2->Delete();
  gPad->Clear();
  get_can()->Clear();


  //for(auto item : gROOT->GetListOfSpecials())
  //  cout<<item->GetName()<<endl;
  

  if(chainload_once == false){
    gROOT->GetListOfSpecials()->Delete();
    gROOT->GetListOfSpecials()->Clear();
    //gROOT->EndOfProcessCleanups();
    //gROOT->CloseFiles();//EndOfProcessCleanups();
    gROOT->GetListOfFiles()->Delete();
    gROOT->GetListOfFiles()->Clear();
    gROOT->Reset();
    //TFile::Close("R");
  }

  return true;
}

TH1F* ChainHists::calc_ratio(TH1F* stack_hist, TH1F* hist){
  TH1F* ratio = (TH1F*) hist->Clone("data_ratio");
  //TH1F * stack_hist = (TH1F*)(stack->GetStack()->Last())->Clone("");
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
  ratio->GetXaxis()->SetLabelSize(0.03/bottomSize); //ratio->GetXaxis()->SetNdivisions(100);
  ratio->GetXaxis()->SetTitleOffset(1.);
  ratio->GetXaxis()->SetTitleSize(0.12);
  //ratio->GetXaxis()->SetTitleOffset();
  //ratio->GetXaxis()->SetLabelSize();

  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetLabelSize(0.03/bottomSize); 
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->GetYaxis()->SetTitleSize( 0.13);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->CenterTitle();
  ratio->SetTickLength( 0.03 / bottomSize );
 
  return ratio;
}

TH1F* ChainHists::calc_MCstat(THStack* stack){
  TH1F* ratio = (TH1F*)(stack->GetStack()->Last())->Clone("ratio");
  if(ratio->GetSumw2N()==0)ratio->Sumw2();
  ratio->SetTitle("");
  TH1F* zero_error_ratio = (TH1F*)(stack->GetStack()->Last())->Clone("ratio");
  for(int m=0;m< zero_error_ratio->GetNcells();m++)
    zero_error_ratio->SetBinError(m,0);
  ratio->Divide(zero_error_ratio);
  ratio->SetMarkerColor(15);
  ratio->SetFillColorAlpha(15,0.35);
  //int numberOfBins = ratio->GetNcells();
  //for(int i = 0; i<numberOfBins; i++)
  //  cout<<ratio->GetBinContent(i)<<" "<<ratio->GetBinError(i)<<endl;
  return ratio;
}

void ChainHists::calc_weightErr(unsigned int i_error, error_method method, TH1F* result){
  TH1F* nominal = (TH1F*) result->Clone("nominal");
  int numberOfBins = error_histos[i_error][0]->GetNcells();   
  vector<TH1F*> histo_sum;
  unsigned int number_of_weights = replace_strings[i_error].size() > 0 ? replace_strings[i_error].size() : error_folders[i_error].size();
  //cout<<"number of error histos "<<error_histos[i_error].size()<<" number of weights "<<number_of_weights<<endl;
  for(unsigned int m =0; m < number_of_weights; m++){
    TH1F* error_histo_sum = (TH1F*)error_histos[i_error][m]->Clone();
    for(unsigned int p = number_of_weights; p < error_histos[i_error].size(); p+= number_of_weights){ 
      //cout<<error_histos[i_error][p+m]->GetBinContent(5)<<endl;
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
      //cout<<"nombin "<<nomBin<<" number of sum histos "<<histo_sum.size()<<endl;
      //double histo_number = error_histos[i_error].size();
      for(auto & hist : histo_sum){
	double content = hist->GetBinContent(i);
	mean_square+= content*content;
      }
      //mean_square/=histo_sum.size();
      //set
      if(nomBin>0){
	//cout<<"PDF RMS Bin "<<i <<" error "<<sqrt(mean_square)<<endl;
	double sum_error_w2 = mean_square + nominal->GetBinError(i)* nominal->GetBinError(i); 
	result->SetBinError(i,sqrt(sum_error_w2));
      }
    }
  }
  else{
    cerr<<"There was a problem with the error calculation method choosen please check!"<<endl;    
  }
  histo_sum = vector<TH1F*>();
}

void ChainHists::AddErrorWeight(string error_string, error_method method, string replace){
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
void ChainHists::AddErrorFolder(std::vector<std::string> folder_name){
  error_folders.push_back(folder_name); 
}

void ChainHists::calc_fixedErr(const std::vector<double> & uncertainties, const std::vector<bool> & stacked, TH1F* result, THStack* stack){
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

TH1F* ChainHists::make_hist(TChain* mychain, std::string variable, std::string binning, std::string draw_option){
  //cout<<"Chain name "<<mychain->GetName()<<endl; 
  if(binning.empty()){
    mychain->Draw((variable+">>myTmpHist").c_str(),draw_option.c_str());
  }
  else{
    mychain->Draw((variable+">>myTmpHist("+binning+")").c_str(),draw_option.c_str());
    //mychain->Scan(variable.c_str(),draw_option.c_str());
  }
  return (TH1F*)gPad->GetPrimitive("myTmpHist");
}


void ChainHists::calc_errorfolder(std::vector<unsigned int> is, TH1F* result, TH1F* nominal){
  //debug = true;
  if(debug) 
    for(auto i : is){
      cout<<i<<" size "<<error_folder_histos.at(i).size()<<endl;
    }
  for(auto i : is){
    if(debug)std::cout<<"number of error "<<i<<std::endl;
    error_folder_histos.at(i).at(0)->SetName(("errorhelper_n-th_"+to_string(0)+"_of_"+to_string(i)).c_str());
    if(debug)std::cout<<error_folder_histos.at(i).at(0)->GetName()<<std::endl;
    for(unsigned int m=1;m<error_folder_histos.at(i).size();m++){
      TH1F* tmphist = (TH1F*) error_folder_histos.at(i).at(m)->Clone(("n-th_"+to_string(m)+"_of_"+to_string(i)).c_str());
      if(debug) std::cout<<tmphist->GetName()<<std::endl;
      error_folder_histos.at(i).at(0)->Add(tmphist);
    }
  }
  unsigned int numberOfBins =  result->GetNcells();
  if(debug)std::cout<<"got number of cells "<<numberOfBins<<std::endl;
  for(unsigned int i=0; i< numberOfBins;i++){
    double err = 0;
    for(auto m : is){
      //if(debug) cout<<"error folder histo bin i: "<<i<<" bin content "<<error_folder_histos.at(m).at(0)->GetBinContent(i)<<endl;
      err += fabs(error_folder_histos.at(m).at(0)->GetBinContent(i)-nominal->GetBinContent(i));//is.size();
    }
    if(debug)std::cout<<"Bin i: "<<i<<" content "<<nominal->GetBinContent(i)<<" error before "<<result->GetBinError(i) <<" folder error "<<err<<" new error "<<sqrt(result->GetBinError(i)*result->GetBinError(i)+err*err) <<std::endl;
    result->SetBinError(i,sqrt(result->GetBinError(i)*result->GetBinError(i)+err*err));
  }
  if(debug)std::cout<<"done with error chain"<<std::endl;
  debug = false;
}


void ChainHists::AddErrorFolderAlias(std::string var, std::string new_name, std::string condition){
  if(var.empty() || new_name.empty()){
    cout<<"ErrorFolderAlias variable is empty. something went wrong. Aborting";
    assert(1==0);
  }
  alias.push_back(new_name);
  oldnames.push_back(var);
  conditions.push_back(condition);
}
