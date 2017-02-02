#include <iostream>
#include <algorithm>


#include "TreeHists.h"
#include "simplePlots.h"
#include "TF1.h"
//#include "filefinder.h"

#include <boost/algorithm/string.hpp> // include Boost, a C++ library


using namespace std;

int main(){
  string CMSSW = "8_0_20";
  string folder ="MuSelection_v13";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.";
  //vector<string> filenames ={"BprimeB-800_LH","BprimeB-900_LH","BprimeB-1000_LH","BprimeB-1100_LH","BprimeB-1200_LH","BprimeB-1300_LH","BprimeB-1400_LH","BprimeB-1500_LH","BprimeB-1600_LH","BprimeB-1700_LH","BprimeB-1800_LH","BprimeB-800_RH","BprimeB-900_RH","BprimeB-1000_RH","BprimeB-1100_RH","BprimeB-1200_RH","BprimeB-1300_RH","BprimeB-1400_RH","BprimeB-1500_RH","BprimeB-1600_RH","BprimeB-1700_RH","BprimeB-1800_RH"};
  vector<string> filenames ={"BprimeT-800_LH","BprimeT-900_LH","BprimeT-1000_LH","BprimeT-1100_LH","BprimeT-1200_LH","BprimeT-1300_LH","BprimeT-1400_LH","BprimeT-1500_LH","BprimeT-1600_LH","BprimeT-1700_LH","BprimeT-1800_LH","BprimeT-800_RH","BprimeT-900_RH","BprimeT-1000_RH","BprimeT-1100_RH","BprimeT-1200_RH","BprimeT-1300_RH","BprimeT-1400_RH","BprimeT-1500_RH","BprimeT-1600_RH","BprimeT-1700_RH","BprimeT-1800_RH"};

  
  vector<string> nicks;
  for(auto name : filenames){
    string copy = name;
    boost::replace_all(copy, "primeT-", "+t M(");
    boost::replace_all(copy, "primeB-", "+b M(");
    boost::replace_all(copy, "_", ") ");
    //cout<<copy<<endl;
    nicks.push_back(copy);
  }

  TreeHists treehists("plots/dummy.ps");
  treehists.SetTree("AnalysisTree");
  for(auto name : filenames)
    treehists.addFile(dir+name+".root");
 
  string helper_chooser = "BestFit.chi<1000 && BestFit.chi>-1 &&";
  std::vector<TH1F*> mass_toplep = treehists.return_hists("BestFit.mass"      ,"weight*("+helper_chooser+" BestFit.recoTyp ==21)","150,500,3000","mass [GeV]");
  std::vector<TH1F*> top_toplep  = treehists.return_hists("BestFit.topLep.M()","weight*("+helper_chooser+" BestFit.recoTyp ==21)","50,100,300"  ,"mass [GeV]");
  std::vector<TH1F*> w_toplep    = treehists.return_hists("BestFit.wHad.M()"  ,"weight*("+helper_chooser+" BestFit.recoTyp ==21)","50,50,180"   ,"mass [GeV]");
  std::vector<TH1F*> mass_tophad = treehists.return_hists("BestFit.mass"      ,"weight*("+helper_chooser+" BestFit.recoTyp ==22)","150,500,3000","mass [GeV]");
  std::vector<TH1F*> top_tophad  = treehists.return_hists("BestFit.topHad.M()","weight*("+helper_chooser+" BestFit.recoTyp ==22)","50,100,300"  ,"mass [GeV]");
  std::vector<TH1F*> w_tophad    = treehists.return_hists("BestFit.wLep.M()"  ,"weight*("+helper_chooser+" BestFit.recoTyp ==22)","50,50,180"   ,"mass [GeV]");


  vector<double> fitmean_toplep;
  vector<double> fitmean_tophad;
  vector<double> fitmean_whad;

  vector<double> fitrms_toplep;
  vector<double> fitrms_tophad;
  vector<double> fitrms_whad;


  TF1* gaus = new TF1("gaus","gaus");
  simplePlots recofit("plots/recofit.ps");
  recofit.switch_ratio(false);
  recofit.set_histYTitle("NA");
  recofit.set_title(" ");
  for(unsigned int i=0; i<mass_toplep.size();++i){
    recofit.set_XTitle("B mass [GeV]");
    recofit.loadHists(mass_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.plotHists(2,false);
    recofit.clearAll();
    
    recofit.loadHists(mass_tophad[i],nicks[i]+" top_{had}","PE");
    recofit.plotHists(2,false);
    recofit.clearAll();

    //Do the Fits
    //cout<<"mean "<<gaus->GetParameter(1)<<endl;
    //cout<<"rms "<<gaus->GetParameter(2)<<endl;;

    recofit.set_XTitle("top mass [GeV]");
    top_toplep[i]->Fit(gaus,"","",130,210);
    fitmean_toplep.push_back(gaus->GetParameter(1));
    fitrms_toplep.push_back(gaus->GetParameter(2));
    recofit.loadHists(top_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();

    top_tophad[i]->Fit(gaus,"","",160,190);
    fitmean_tophad.push_back(gaus->GetParameter(1));
    fitrms_tophad.push_back(gaus->GetParameter(2));
    recofit.loadHists(top_tophad[i],nicks[i]+" top_{had}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();

    recofit.set_XTitle("W mass [GeV]");
    recofit.loadHists(w_tophad[i],nicks[i]+" top_{had}","PE");
    recofit.plotHists(2,false);
    recofit.clearAll();

    w_toplep[i]->Fit(gaus,"","",60,100);
    fitmean_whad.push_back(gaus->GetParameter(1));
    fitrms_whad.push_back(gaus->GetParameter(2));
    recofit.loadHists(w_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();
  }

  double N = fitmean_toplep.size();
  double sum_rms_toplep=0, sum_rms_tophad=0, sum_rms_whad=0;
  double rec_rms_toplep=0, rec_rms_tophad=0, rec_rms_whad=0;
  double average_toplep_mean=0, average_tophad_mean=0, average_whad_mean=0;
  double average_toplep_rms=0, average_tophad_rms=0, average_whad_rms=0;

  for(unsigned int i=0;i<N;++i){
    average_toplep_mean += fitmean_toplep[i];
    average_tophad_mean += fitmean_tophad[i];
    average_whad_mean += fitmean_whad[i];

    average_toplep_rms += fitrms_toplep[i];
    average_tophad_rms += fitrms_tophad[i];
    average_whad_rms += fitrms_whad[i];

    sum_rms_toplep += fitrms_toplep[i];
    sum_rms_tophad += fitrms_tophad[i];
    sum_rms_whad += fitrms_whad[i];

    rec_rms_toplep += 1/(fitrms_toplep[i]*fitrms_toplep[i]);
    rec_rms_tophad += 1/(fitrms_tophad[i]*fitrms_tophad[i]);
    rec_rms_whad +=1/(fitrms_whad[i]*fitrms_whad[i]);
  }
  
  fitmean_toplep[0]*=fitrms_toplep[0];
  fitmean_tophad[0]*=fitrms_tophad[0];
  fitmean_whad[0]*=fitrms_whad[0];

  for(unsigned int i=1;i<N;++i){
    //mean
    fitmean_toplep[0]+=fitmean_toplep[i]*fitrms_toplep[i];
    fitmean_tophad[0]+=fitmean_tophad[i]*fitrms_tophad[i];
    fitmean_whad[0]+=fitmean_whad[i]*fitrms_whad[i];
  }

  cout<<"Mean "<<" RMS "<<endl;
  cout<<"========================="<<endl;
  cout<<"weighted mean "<<endl;
  cout<<"top lep "<<fitmean_toplep[0]/sum_rms_toplep<<" +- "<< sqrt(1/rec_rms_toplep) <<endl;
  cout<<"top had "<<fitmean_tophad[0]/sum_rms_tophad<<" +- "<< sqrt(1/rec_rms_tophad) <<endl;
  cout<<"w had "<<fitmean_whad[0]/sum_rms_whad<<" +- "<< sqrt(1/rec_rms_whad) <<endl;
  cout<<"========================="<<endl;
  cout<<"average "<<endl;
  cout<<"top lep "<<average_toplep_mean/N<<" +- "<< average_toplep_rms/N  <<endl;
  cout<<"top had "<<average_tophad_mean/N<<" +- "<< average_tophad_rms/N <<endl;
  cout<<"w had "<<average_whad_mean/N<<" +- "<< average_whad_rms/N <<endl;

  return 0;
}
