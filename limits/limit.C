#include <vector>

void create_file(const char *dirname, TString filename, TString histoname, TString Prefix);
pair<TH1F*,TH1F*> make_envelop_hist(TFile* file, TString folder, TString Prefix, TString type);
pair<TH1F*,TH1F*> make_rms_hist(TFile* file, TString folder, TString Prefix, TString type);
void limit(const char *dirname="/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_15_patch1/src/UHH2/VLQToTopAndLepton/config/"){

  //dirname2 = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/";
  TString directories[] = {"Selection_v16/","EleSelection_v1/"};
  TString dirPrefix[] = {"Mu","Ele"};

  TString names[] = {"SingleTsChannel","SingleTtChannel","SingleTWAntitop","SingleTWTop","ZJetsM50toInf","TTJets","WJets","Bpt_TW_800_LH_25ns","Bpt_TW_1000_LH_25ns","Bpt_TW_1200_LH_25ns","Bpt_TW_800_RH_25ns","Bpt_TW_1000_RH_25ns","Bpt_TW_1200_RH_25ns","QCD","DATA"};
  //TString names[] ={"Bpt_TW_800_LH_25ns"};
  //TString names[] = {"TTJets","Bpb_TW_800_LH_25ns","Bpb_TW_800_RH_25ns","Bpb_TW_1200_LH_25ns","Bpb_TW_1200_RH_25ns"};//,"QCD"
  //TString histograms[] = {"Chi2_AntiBTag_Chi2_BprimeHypHists/mass_hyp","Chi2_BTag_Chi2_BprimeHypHists/mass_hyp","Chi2_2_BTags_Chi2_BprimeHypHists/mass_hyp"};
  //TString histograms[] = {"Chi2_AntiHEPTopTag_Chi2_BprimeHypHists/mass_hyp","Chi2_HEPTopTag_Chi2_BprimeHypHists/mass_hyp"};
  //TString histograms[] = {"Chi2_AntiTopTag_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_2_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","CMSReco_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_BTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_2_BTag_Central_Chi2_BprimeHypHists/mass_hyp","WTagReco_BTag_WTagReco_BprimeHypHists/mass_hyp"};//!!!,"WTagReco_BprimeHypHists/mass_hyp"

  //TString histograms[] = {"Chi2_AntiBTag_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Chi2_BprimeHypHists/mass_hyp","CMSReco_BprimeHypHists/mass_hyp","WTagReco_BprimeHypHists/mass_hyp"};

  //TString histograms[] = {"Chi2_AntiBTag_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Chi2_BprimeHypHists/mass_hyp"};//!!!

  TString histograms[] = {"Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp","TopTagReco_Forward_TopTagReco_BprimeHypHists/mass_hyp","TopTagReco_Central_TopTagReco_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Forward_Chi2_BprimeUncerHists","Chi2_1_BTag_Forward_Chi2_BprimeUncerHists","Chi2_2plus_BTags_Forward_Chi2_BprimeUncerHists","Chi2_AntiBTag_Central_Chi2_BprimeUncerHists","Chi2_1_BTag_Central_Chi2_BprimeUncerHists","Chi2_2plus_BTags_Central_Chi2_BprimeUncerHists","TopTagReco_Forward_TopTagReco_BprimeUncerHists","TopTagReco_Central_TopTagReco_BprimeUncerHists"};//!!!


  //TString histograms[] = {"Chi2_AntiTopTag_AntiBTag_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_BTag_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_2_BTag_Chi2_BprimeHypHists/mass_hyp","CMSReco_AntiBTag_CMSReco_BprimeHypHists/mass_hyp","CMSReco_BTag_CMSReco_BprimeHypHists/mass_hyp","CMSReco_2_BTags_CMSReco_BprimeHypHists/mass_hyp"};
  //TString histograms[] = {"CMSReco_AntiBTag_CMSReco_BprimeHypHists/mass_hyp","CMSReco_BTag_CMSReco_BprimeHypHists/mass_hyp","CMSReco_2_BTags_CMSReco_BprimeHypHists/mass_hyp"};
  //TString histograms[] = {"Chi2_AntiTopTag_Chi2_BprimeHypHists/mass_hyp","CMSReco_BprimeHypHists/mass_hyp"};
  //TString histograms[] = {"Chi2_AntiTopTag_AntiBTag_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_BTag_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiTopTag_2_BTag_Chi2_BprimeHypHists/mass_hyp","CMSReco_BprimeHypHists/mass_hyp"};
  //TString histograms[] = {"Chi2_BprimeUncerHists"};
  //TString histograms[] = {"Chi2_Forward_Chi2_BprimeHypHists/mass_hyp"};
  //TString Prefix[] = {""};
  //TString histograms[] = {"Chi2_AntiBTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Central_Chi2_BprimeHypHists/mass_hyp","Chi2_AntiBTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_1_BTag_Forward_Chi2_BprimeHypHists/mass_hyp","Chi2_2plus_BTags_Forward_Chi2_BprimeHypHists/mass_hyp","CMSReco_BprimeHypHists/mass_hyp","WTagReco_BprimeHypHists/mass_hyp"};//!!!
  
  TString Prefix[] = {"AntiTopTagAntiBTagForward","AntiTopTag1BTagForward","AntiTopTag2BTagsForward","AntiTopTagAntiBTagCentral","AntiTopTag1BTagCentral","AntiTopTag2BTagsCentral","TopTagForward","TopTagCentral","AntiTopTagAntiBTagForward","AntiTopTag1BTagForward","AntiTopTag2BTagsForward","AntiTopTagAntiBTagCentral","AntiTopTag1BTagCentral","AntiTopTag2BTagsCentral","TopTagForward","TopTagCentral"};


  //TString histograms[] = {"RecoBprime/mass_Bprime"};
  TFile* file2  = new TFile("RecoBp.root","RECREATE");
  file2->Close();
  
 
  for(int p =0; p < sizeof(names)/sizeof(TString); ++p){
    for(int m =0; m< sizeof(directories)/sizeof(TString); ++m){
      for(int i = 0; i< sizeof(histograms)/sizeof(TString); ++i){
	//if(p<3)
	  create_file(dirname+directories[m], names[p],histograms[i], "Bprime"+Prefix[i]+dirPrefix[m]);
	//else
	//  create_file(dirname2+directories[m], names[p],histograms[i], "Bprime"+Prefix[i]);
      }
      cout<<names[p]<<" --> Done"<<endl;
    }
  }
}
 
void create_file(const char *dirname, TString filename, TString histoname, TString Prefix){
  
  TSystemDirectory dir(dirname, dirname);
  TList *fileslist = dir.GetListOfFiles();
  if (fileslist) {
    TSystemFile *systemfile;
    TString fname;
    TIter next(fileslist);
    while ((systemfile=(TSystemFile*)next())) {
      fname = systemfile->GetName();
      //cout<<fname<<endl;
      if (!systemfile->IsDirectory() && fname.Contains(filename+".root")) {
	cout<<Prefix<<endl;
	TFile* file  = new TFile(dirname+fname,"READ");
	TFile* file2  = new TFile("RecoBp.root","UPDATE");
	if(!histoname.Contains("/") ){
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
	  TString histname = h1->GetName();
	  h1->SetName(Prefix+"__"+filename);
	  h1->Write();
	  //part for the uncertainties
	}
	file->Close();
	file2->Close();
      }
    }
  }
}

pair<TH1F*,TH1F*> make_rms_hist(TFile* file, TString folder, TString Prefix, TString type){
  cout<<"entering "<<folder<<" to calculate "<<type<<" uncer"<<endl;
  TDirectory* tdir = (TDirectory*) file->Get(folder);
  TIter next(tdir->GetListOfKeys());
  TKey *key;
  int nhist=0;  
 
  //load histograms
  vector<TH1F*> uncer_histo;
  while ((key = (TKey*)next())) {
    nhist++;
    TString hist_name(key->GetName());
    if(!hist_name.Contains(type))
      continue;
    uncer_histo.push_back((TH1F*)tdir->Get(key->GetName()));
    cout<<"loaded histogram: "<<key->GetName()<<endl;
    /*
    cout << "Key number " << nhist << endl;
    cout << "Key name "<<key->GetName()<<endl;
    cout << "Classname " <<key->GetClassName() << endl;
    cout << "Title " <<key->GetTitle() << endl; 
    */
  }
  //create th1f*

  
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
  cout<<"entering "<<folder<<" to calculate "<<type<<" uncer"<<endl;
  TDirectory* tdir = (TDirectory*) file->Get(folder);
  TIter next(tdir->GetListOfKeys());
  TKey *key;
  int nhist=0;  
 
  //load histograms
  vector<TH1F*> uncer_histo;
  while ((key = (TKey*)next())) {
    nhist++;
    TString hist_name(key->GetName());
    if(!hist_name.Contains(type))
      continue;
    uncer_histo.push_back((TH1F*)tdir->Get(key->GetName()));
    cout<<"loaded histogram: "<<key->GetName()<<endl;
    /*
    cout << "Key number " << nhist << endl;
    cout << "Key name "<<key->GetName()<<endl;
    cout << "Classname " <<key->GetClassName() << endl;
    cout << "Title " <<key->GetTitle() << endl; 
    */
  }
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
