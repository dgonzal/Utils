#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

void create_file(const char *dirname, TString filename, TString histoname, TString Prefix, TString ResultFile, string RenameDataHist = "");
pair<TH1F*,TH1F*> make_envelop_hist(TFile* file, TString folder, TString Prefix, TString type);
pair<TH1F*,TH1F*> make_rms_hist(TFile* file, TString folder, TString Prefix, TString type);
void limit(const char * signal,const char * resultfile , const char *dirname="/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_6_3/src/UHH2/VLQToTopAndLepton/config/"){
  //TString directories[] = {"Selection_v16/","BTag_variation_up/","BTag_variation_down/","PU_variation_up/","SF_muonID_up/","BTag_variation_up/","PU_variation_down/","SF_muonID_down/","BTag_variation_down/"};//,"EleSelection_v1/"
  TString directories[] = {"Selection_v25/"};//,"BTag_variation_up_Mu/","BTag_variation_down_Mu/","PU_variation_up_Mu/","SF_muonID_up_Mu/","PU_variation_down_Mu/","SF_muonID_down_Mu/","EleSelection_v1/","BTag_variation_up_Ele/","BTag_variation_down_Ele/","PU_variation_up_Ele/","PU_variation_down_Ele/"};
  TString dirPrefix[] = {"Mu","Ele"};

  //,"EleSelection_v1/","BTag_variation_up_Mu/","BTag_variation_down_Mu/","PU_variation_up_Mu/","SF_muonID_up_Mu/","BTag_variation_up_Mu/","PU_variation_down_Mu/","SF_muonID_down_Mu/","BTag_variation_down_Mu/","BTag_variation_up_Ele/","BTag_variation_down_Ele/","PU_variation_up_Ele/","SF_muonID_up_Ele/","BTag_variation_up_Ele/","PU_variation_down_Ele/","SF_muonID_down_Ele/","BTag_variation_down_Ele/"
  
  TString names[] = {"SingleTsChannel","SingleTtChannel","SingleTWAntitop","SingleTWTop","ZJets","TTJets","WJets","QCD","DATA",signal};

  vector<TString> histograms = {"Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp","TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp","TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Forward_Chi2_BprimeUncerHists","Chi2_1_BTag_Forward_Chi2_BprimeUncerHists","Chi2_2plus_BTags_Forward_Chi2_BprimeUncerHists","Chi2_AntiBTag_Central_Chi2_BprimeUncerHists","Chi2_1_BTag_Central_Chi2_BprimeUncerHists","Chi2_2plus_BTags_Central_Chi2_BprimeUncerHists","TopTagReco_Forward_TopTagReco_BprimeUncerHists","TopTagReco_Central_TopTagReco_BprimeUncerHists"};
  

  //boosted histograms
  //histograms = {"TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp","TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp","TopTagReco_Forward_TopTagReco_BprimeUncerHists","TopTagReco_Central_TopTagReco_BprimeUncerHists"};


  //histograms = {"Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_Mass_had","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_Mass_had","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_Mass_had","Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_Mass_lep","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_Mass_lep","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_Mass_lep","Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_Mass_had","Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_Mass_had","Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_Mass_had","Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_Mass_lep","Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_Mass_lep","Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_Mass_lep","TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp","TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp"};

  //TString histograms[] =   {"Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp"};
  vector<TString> Prefix;
  for(auto name : histograms){
    string s = name.Data();
    vector<string> my_names;
    string new_name;
    boost::split(my_names,s,boost::is_any_of("_"));
    for(auto part : my_names){
      if(!boost::algorithm::contains(part,"Bprime"))
	new_name += part;
      else
	break;
    }
    if(boost::algorithm::contains(s,"Mass_had"))
      new_name+="_had";
    else if(boost::algorithm::contains(s,"Mass_lep"))
       new_name+="_lep";
    Prefix.push_back(new_name);
  }
  //TString histograms[] = {"Chi2_AntiBTag_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Chi2_BprimeHypHists/mass_hyp","TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp","TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Chi2_BprimeUncerHists","Chi2_1_BTag_Chi2_BprimeUncerHists","Chi2_2plus_BTags_Chi2_BprimeUncerHists","TopTagReco_Forward_TopTagReco_BprimeUncerHists","TopTagReco_Central_TopTagReco_BprimeUncerHists"};
  //TString Prefix[] = {"AntiTopTagAntiBTag","AntiTopTag1BTag","AntiTopTag2BTags","TopTagForward","TopTagCentral","AntiTopTagAntiBTag","AntiTopTag1BTag","AntiTopTag2BTags","TopTagForward","TopTagCentral"};
 
  /*
  TString names[] = {"DATA",signal};
  TString histograms[] = {"Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp","TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp"};
  TString backgroundHist[] = {"Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp","TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp"};
  TString Prefix[] = {"AntiTopTagAntiBTagForward","AntiTopTag1BTagForward","AntiTopTag2BTagsForward","TopTagForward"};
  */
  //TString histograms[] = {"Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp","TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Forward_Chi2_BprimeUncerHists","Chi2_1_BTag_Forward_Chi2_BprimeUncerHists","Chi2_2plus_BTags_Forward_Chi2_BprimeUncerHists","TopTagReco_Forward_TopTagReco_BprimeUncerHists"};

 
  //TString Prefix[] = {"AntiTopTagAntiBTagForward","AntiTopTag1BTagForward","AntiTopTag2BTagsForward","TopTagForward","AntiTopTagAntiBTagForward","AntiTopTag1BTagForward","AntiTopTag2BTagsForward","TopTagForward"};

  //TString Prefix[] = {"AntiTopTagAntiBTagForward","AntiTopTag1BTagForward","AntiTopTag2BTagsForward","TopTagForward"};

  TFile* file2  = new TFile(TString(resultfile)+".root","RECREATE");
  file2->Close();
  
  for(int m =0; m< sizeof(dirPrefix)/sizeof(TString); ++m){
    TFile* filedirs  = new TFile(dirPrefix[m]+TString(resultfile)+".root","RECREATE");
    filedirs->Close();
  }


  for(int m =0; m< sizeof(directories)/sizeof(TString); ++m){
    //int prefix_it = m % 2; //sizeof(dirPrefix)/sizeof(TString);
    ///cout<<m<<" "<<prefix_it<<" "<< sizeof(dirPrefix)/sizeof(TString)<<endl;
    int prefix_it = 0;

    if(directories[m].Contains("Ele"))
      prefix_it = 1;

    for(int p =0; p < sizeof(names)/sizeof(TString); ++p){
      for(unsigned int i =0; i< histograms.size(); ++i){
	create_file(dirname+directories[m], names[p],histograms[i], "Bprime"+Prefix[i]+dirPrefix[m],resultfile);
	create_file(dirname+directories[m], names[p],histograms[i], "Bprime"+Prefix[i]+dirPrefix[m],dirPrefix[m]+resultfile);
	/*
	if(p==0)
	  create_file(dirname+directories[m], "DATA",backgroundHist[i], "Bprime"+Prefix[i]+dirPrefix[0],resultfile,"Background");
	*/
      }
      cout<<names[p]<<" --> Done"<<endl;
    }
  }
  gApplication->Terminate();
}
 
void create_file(const char *dirname, TString filename, TString histoname, TString Prefix, TString ResultFile, string RenameDataHist){
  TString filename_old = filename; 
  TString my_dir(dirname);
  TSystemDirectory dir(dirname, dirname);
  TList *fileslist = dir.GetListOfFiles();
  if (fileslist) {
    TSystemFile *systemfile;
    TString fname;
    TIter next(fileslist);
    while ((systemfile=(TSystemFile*)next())) {
      fname = systemfile->GetName();
      //continue;
      filename = filename_old;
      if (!systemfile->IsDirectory() && fname.Contains(filename+".root")) {
	if(fname.Contains("Bpt_TW") && ResultFile.Contains("Bpb")){
	  //cout<<"excluding "<<fname<<endl;
	  continue;
	}
	else if(fname.Contains("Bpb_TW") && ResultFile.Contains("Bpt")){
	  continue;
	}
	vector<string>  fname_split;
	string fname_string = fname.Data();
	boost::split(fname_split,fname_string,boost::is_any_of("."));
	if(!filename.Contains("DATA"))
	  filename = fname_split[3];
	//cout<<fname<<" "<<filename<<endl;
	//cout<<Prefix<<endl;
	TFile* file  = new TFile(dirname+fname,"READ");
	TFile* file2  = new TFile(ResultFile+".root","UPDATE");
	if((my_dir.Contains("up") ||my_dir.Contains("Up") || my_dir.Contains("UP")) &&  !filename.Contains("DATA")){
	  string s = my_dir.Data();
	  vector<string> my_names;
	  boost::split(my_names,s,boost::is_any_of("/"));
	  string target = my_names[my_names.size()-2];
	  boost::replace_all(target,"_up","");
	  TH1F* h1 = (TH1F*)file->Get(histoname);
	  TString histname = h1->GetName();
	  h1->SetName(Prefix+"__"+filename+"__"+target+"__plus");
	  h1->Write();

	}
	else if((my_dir.Contains("down") || my_dir.Contains("Down") || my_dir.Contains("DOWN"))&&  !filename.Contains("DATA")){
	  string s = my_dir.Data();
	  vector<string> my_names;
	  boost::split(my_names,s,boost::is_any_of("/"));
	  string target = my_names[my_names.size()-2];
	  boost::replace_all(target,"_down","");
	  TH1F* h1 = (TH1F*)file->Get(histoname);
	  TString histname = h1->GetName();
	  h1->SetName(Prefix+"__"+filename+"__"+target+"__minus");
	  h1->Write();
	}
	else if(!histoname.Contains("/") ){
	  if(!filename.Contains("DATA")){
	    pair<TH1F*,TH1F*> scale = make_envelop_hist(file,histoname,Prefix+"__"+filename,"scale");
	    if(scale.first->GetEntries() !=0 || scale.second->GetEntries() !=0){
	      scale.first->Write();
	      scale.second->Write();
	    }
	    pair<TH1F*,TH1F*> pdf = make_rms_hist(file,histoname,Prefix+"__"+filename,"pdf");
	    if(pdf.first->GetEntries() !=0 || pdf.second->GetEntries() !=0){
	      pdf.first->Write();
	      pdf.second->Write();
	    }
	  }
	}
	else{
	  TH1F* h1 = (TH1F*)file->Get(histoname);
	  //if(!fname.Contains("Bpb_TW")) h1->Scale(0.25);
	  //h1->Scale(15);
	  if(!RenameDataHist.empty())
	    filename = RenameDataHist.c_str();
	  TString histname = h1->GetName();
	  h1->SetName(Prefix+"__"+filename);
	  h1->Write();
	}
	file->Close();
	file2->Close();
      }
    }
  }
}

pair<TH1F*,TH1F*> make_rms_hist(TFile* file, TString folder, TString Prefix, TString type){
  //cout<<"entering "<<folder<<" to calculate "<<type<<" uncer"<<endl;
  TDirectory* tdir = (TDirectory*) file->Get(folder);
  TIter next(tdir->GetListOfKeys());
  TKey *key;
  int nhist=0;  
 
  //load histograms
  vector<TH1F*> uncer_histo;
  while ((key = (TKey*)next())) {
    TString hist_name(key->GetName());
    if(!hist_name.Contains(type))
      continue;
    nhist++;
    uncer_histo.push_back((TH1F*)tdir->Get(key->GetName()));
    //cout<<"loaded histogram: "<<key->GetName()<<endl;
    /*
    cout << "Key number " << nhist << endl;
    cout << "Key name "<<key->GetName()<<endl;
    cout << "Classname " <<key->GetClassName() << endl;
    cout << "Title " <<key->GetTitle() << endl; 
    */
  }
  //create th1f*

  //cout<<"RMS Histograms found "<<nhist<<endl;


  TString nominal_name = folder;
  nominal_name.ReplaceAll("UncerHists","HypHists/mass_hyp");
  TH1F* nominal = (TH1F*)file->Get(nominal_name);
  TH1F* rms = (TH1F*)uncer_histo[0]->Clone();
  TH1F* up  = (TH1F*)file->Get(nominal_name);
  up->SetName(Prefix+"__"+type+"__plus");
  TH1F* down  = (TH1F*)file->Get(nominal_name);
  down->SetName(Prefix+"__"+type+"__minus");

  //find rms
  //check all histograms have same binning
  int numberOfBins = uncer_histo[0]->GetNcells();
  for(auto & hist : uncer_histo){
    if(numberOfBins != hist->GetNcells()){
      cout<<"number of cells is not equal. Aborting"<<endl;
      assert(0==1);
    }
  }
  
  for(int i = 0; i<numberOfBins; i++){
    double mean_square = 0;
    double histo_number = 0;
    for(auto & hist : uncer_histo){
      histo_number+=1;
      mean_square += (hist->GetBinContent(i)-nominal->GetBinContent(i))*(hist->GetBinContent(i)-nominal->GetBinContent(i));
    }
    rms->SetBinContent(i,sqrt(mean_square/histo_number));
  }
  up->Add(rms);
  down->Add(rms,-1);

  pair<TH1F*,TH1F*> result;
  result.first = up;
  result.second = down;

  return result;

}
pair<TH1F*,TH1F*> make_envelop_hist(TFile* file, TString folder, TString Prefix, TString type){
  //cout<<"entering "<<folder<<" to calculate "<<type<<" uncer"<<endl;
  TDirectory* tdir = (TDirectory*) file->Get(folder);
  TIter next(tdir->GetListOfKeys());
  TKey *key;
  int nhist=0;  
 
  //load histograms
  vector<TH1F*> uncer_histo;
  while ((key = (TKey*)next())) {
    
    TString hist_name(key->GetName());
    if(!hist_name.Contains(type))
      continue;
    nhist++;
    uncer_histo.push_back((TH1F*)tdir->Get(key->GetName()));
    //cout<<"loaded histogram: "<<key->GetName()<<endl;
   /*
    cout << "Key number " << nhist << endl;
    cout << "Key name "<<key->GetName()<<endl;
    cout << "Classname " <<key->GetClassName() << endl;
    cout << "Title " <<key->GetTitle() << endl; 
    */
  }

  //cout<<"Envelop Histograms found "<<nhist<<endl;

  //create result th1f*
  TH1F* up = (TH1F*)uncer_histo[0]->Clone();
  up->SetName(Prefix+"__"+type+"__plus");
  TH1F* down = (TH1F*)uncer_histo[0]->Clone();
  down->SetName(Prefix+"__"+type+"__minus");
  //find envelop
  //check all histograms have same binning
  int numberOfBins = uncer_histo[0]->GetNcells();
  for(auto & hist : uncer_histo){
    if(numberOfBins != hist->GetNcells()){
      cout<<"number of cells is not equal. Aborting"<<endl;
      assert(0==1);
    }
  }
  for(int i = 0; i<numberOfBins; i++){
    double max =-9999999999999;
    double min = 9999999999999;
    for(auto & hist : uncer_histo){
      double content = hist->GetBinContent(i);
      if(content > max ) max = content;
      if(content < min ) min = content;
    }
    if(max!=0.0)
      up->SetBinContent(i,max);
    if(min!=0.0)
      down->SetBinContent(i,min);
  }
  pair<TH1F*,TH1F*> result;
  result.first = up;
  result.second = down;
  return result;

}
