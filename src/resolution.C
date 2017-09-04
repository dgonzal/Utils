#include <iostream>
#include <algorithm>


#include "TreeHists.h"
#include "simplePlots.h"
#include "TF1.h"
//#include "filefinder.h"

#include <boost/algorithm/string.hpp> // include Boost, a C++ library


using namespace std;

//This is specific to the naming conventions I'm using in this script like code
bool check_name_difference(string oldname, string newname){
  std::vector<std::string> splitted_old;
  boost::split(splitted_old, oldname, boost::is_any_of("\t "));
  std::vector<std::string> splitted_new;
  boost::split(splitted_new, newname, boost::is_any_of("\t ")); 

  /*/
  for(unsigned int i=0; i<splitted_old.size();++i)
    cout<<splitted_old[i] <<" "<<splitted_new[i] <<endl;
  /*/
  return splitted_new[0].compare(splitted_old[0])==0 && splitted_old[splitted_old.size()-1].compare(splitted_new[splitted_new.size()-1])==0;
}


int main(){
  string CMSSW = "8_0_24_patch1";
  string folder = "MuSigSel";//"MuSigSel";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.";
  string output ="plots/resolutionMu/";
  bool single = true;
  
  //vector<string> filenames ={"BprimeB-800_LH","BprimeB-900_LH","BprimeB-1000_LH","BprimeB-1100_LH","BprimeB-1200_LH","BprimeB-1300_LH","BprimeB-1400_LH","BprimeB-1500_LH","BprimeB-1600_LH","BprimeB-1700_LH","BprimeB-1800_LH","BprimeB-800_RH","BprimeB-900_RH","BprimeB-1000_RH","BprimeB-1100_RH","BprimeB-1200_RH","BprimeB-1300_RH","BprimeB-1400_RH","BprimeB-1500_RH","BprimeB-1600_RH","BprimeB-1700_RH","BprimeB-1800_RH"};
  //vector<string> filenames ={"BprimeT-800_LH","BprimeT-900_LH","BprimeT-1000_LH","BprimeT-1100_LH","BprimeT-1200_LH","BprimeT-1300_LH","BprimeT-1400_LH","BprimeT-1500_LH","BprimeT-1600_LH","BprimeT-1700_LH","BprimeT-1800_LH","BprimeT-800_RH","BprimeT-900_RH","BprimeT-1000_RH","BprimeT-1100_RH","BprimeT-1200_RH","BprimeT-1300_RH","BprimeT-1400_RH","BprimeT-1500_RH","BprimeT-1600_RH","BprimeT-1700_RH","BprimeT-1800_RH"};
  //vector<string> filenames ={"X53-800_LH","X53-900_LH","X53-1000_LH","X53-1100_LH","X53-1200_LH","X53-1300_LH","X53-1400_LH","X53-1500_LH","X53-1600_LH"};
  
  vector<string> filenames ={"BprimeB-800_LH","BprimeB-900_LH","BprimeB-1000_LH","BprimeB-1100_LH","BprimeB-1200_LH","BprimeB-1300_LH","BprimeB-1400_LH","BprimeB-1500_LH","BprimeB-1600_LH","BprimeB-1700_LH","BprimeB-1800_LH","BprimeB-800_RH","BprimeB-900_RH","BprimeB-1000_RH","BprimeB-1100_RH","BprimeB-1200_RH","BprimeB-1300_RH","BprimeB-1400_RH","BprimeB-1500_RH","BprimeB-1600_RH","BprimeB-1700_RH","BprimeB-1800_RH","BprimeT-800_LH","BprimeT-900_LH","BprimeT-1000_LH","BprimeT-1100_LH","BprimeT-1200_LH","BprimeT-1300_LH","BprimeT-1400_LH","BprimeT-1500_LH","BprimeT-1600_LH","BprimeT-1700_LH","BprimeT-1800_LH","BprimeT-800_RH","BprimeT-900_RH","BprimeT-1000_RH","BprimeT-1100_RH","BprimeT-1200_RH","BprimeT-1300_RH","BprimeT-1400_RH","BprimeT-1500_RH","BprimeT-1600_RH","BprimeT-1700_RH","BprimeT-1800_RH","X53-800_LH","X53-900_LH","X53-1000_LH","X53-1100_LH","X53-1200_LH","X53-1300_LH","X53-1400_LH","X53-1500_LH","X53-1600_LH"};
  
  vector<string> nicks;
  for(auto name : filenames){
    string copy = name;
    boost::replace_all(copy, "primeT-", "+t M(");
    boost::replace_all(copy, "primeB-", "+b M(");
    boost::replace_all(copy, "_", ") ");
    boost::replace_all(copy, "X53-", "X_{53}+t M(");

    //cout<<copy<<endl;
    nicks.push_back(copy);
  }

  TreeHists treehists("plots/dummy.ps");
  treehists.SetTree("AnalysisTree");
  int i =0;
  for(auto name : filenames){
    if(i==0)treehists.addFile(dir+name+".root","PE", i+2, -1 ,false, nicks[i]);
    else treehists.addFile(dir+name+".root","same",i+2, -1 ,false, nicks[i]);
    i++;
  }
  treehists.switch_ratio(false);
  /*/
  treehists.Draw("BestFit.topHad.M()","weight*("+helper_chooser+" BestFit.recoTyp ==22 && fabs(BestFit.topHad.eta())<1.4)","20,100,300"  ,"mass [GeV]");
  treehists.Draw("BestFit.topHad.eta()","weight*("+helper_chooser+" BestFit.recoTyp ==22 )","50,-4,4"  ,"mass [GeV]");
  treehists.Draw("BestFit.topLep.M()","weight*("+helper_chooser+" BestFit.recoTyp ==21 )","20,100,300"  ,"mass [GeV]");
  treehists.Draw(deltaRstring("BestFit.topHad","BprimeGen.topHad"),"weight*("+helper_chooser+" BestFit.recoTyp ==22 )","50,0,2"  ,"");
  
  treehists.Draw(deltaRstring("BestFit.topHad","BprimeGen.topHad"),"weight*("+helper_chooser+" BestFit.recoTyp ==22 && BestFit.topHad.M()<150)","50,0,2"  ,"");
  treehists.Draw("BestFit.mass"          ,"weight*("+helper_chooser+" BestFit.recoTyp ==22 && BestFit.topHad.M()<150)","50,500,3000"  ,"");
  treehists.Draw("BestFit.topHad.phi()"  ,"weight*("+helper_chooser+" BestFit.recoTyp ==22 && BestFit.topHad.M()<150)","50,-4,4"  ,"");
  treehists.Draw("BestFit.topHad.eta()","weight*("+helper_chooser+" BestFit.recoTyp ==22 && BestFit.topHad.M()<150)","50,-4,4"  ,"");
 
  treehists.Draw("sqrt("+deltaRstring("BestFit.wHad","BestFit.topLep")+")","weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  treehists.Draw("sqrt("+deltaPhistring("BestFit.wHad","BestFit.topLep")+")","weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  treehists.Draw("sqrt("+deltaEtastring("BestFit.wHad","BestFit.topLep")+")","weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  /*/

  string binning = "70,-1,4";
  string sel_string = "TopTagDis.mass==-1|| TopTagDis.topHad.pt()<400";

  std::vector<TH1F*> chi2_mass    = treehists.return_hists("(Chi2Dis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*("+sel_string+")", binning,"VLQ mass [GeV]");
  sel_string = "TopTagDis.mass>0 && TopTagDis.topHad.pt()>400";
  std::vector<TH1F*> toptag_mass  = treehists.return_hists("(TopTagDis.mass-BprimeGen.bprime.M())/BprimeGen.bprime.M()","weight*("+sel_string+")",binning ,"VLQ mass [GeV]");
  
  vector<double> fitmean_toptag_mass;
  vector<double> fitmean_chi2_mass;

  vector<double> fitrms_toptag_mass;
  vector<double> fitrms_chi2_mass;


  TF1* gaus = new TF1("gaus","gaus");
  simplePlots recofit(output,single);
  recofit.set_XTitle("(m_{rec}-m_{gen})/m_{gen}");
  recofit.setLegend(0.6, 0.3, 0.86, 0.86);
  recofit.switch_ratio(false);
  recofit.set_histYTitle("AU");
  recofit.set_title(" X^{2} reconstruction ");
  string oldname="";
  for(unsigned int i=0; i<chi2_mass.size();++i){
    string plot_command = "same";
    if(i==0){
      plot_command = "PE";
      oldname =nicks[i];
    }
    else{
      if(!check_name_difference(oldname,nicks[i])){
	recofit.plotHists(2,false);
	recofit.clearAll();
      }
      oldname=nicks[i];
    }
    //chi2_mass[i]->GetXaxis()->SetTitleOffset(0.4);
    recofit.loadHists(chi2_mass[i],nicks[i],plot_command);    
    chi2_mass[i]->Fit(gaus,"0","",-.3,.3);
    fitmean_chi2_mass.push_back(gaus->GetParameter(1));
    fitrms_chi2_mass.push_back(gaus->GetParameter(2));
  }
  recofit.plotHists(2,false);
  recofit.clearAll();
  
  recofit.set_title(" t-tag reconstruction ");
  for(unsigned int i=0; i<toptag_mass.size();++i){
    //if(toptag_mass[i]->GetEntries() <400) continue;
    string plot_command = "same";
    if(i==0){
      plot_command = "PE";
      oldname =nicks[i];
    }
    else{
      if(!check_name_difference(oldname,nicks[i])){
	recofit.plotHists(2,false);
	recofit.clearAll();
      }
      oldname=nicks[i];
    }
    //toptag_mass[i]->GetYaxis()->SetTitleOffset(0.4);
    recofit.loadHists(toptag_mass[i],nicks[i],plot_command);    
    toptag_mass[i]->Fit(gaus,"0","",-.3,.3);
    fitmean_toptag_mass.push_back(gaus->GetParameter(1));
    fitrms_toptag_mass.push_back(gaus->GetParameter(2));
  }
  recofit.plotHists(2,false);
  recofit.clearAll();

  double average_chi2_rms   = 0;
  double average_toptag_rms = 0;
  double average_chi2_mean  = 0;
  double average_toptag_mean= 0;
  
  cout<<"======================================"<<endl;
  cout<<"Fit results"<<endl;
  for(unsigned int i =0; i<fitmean_chi2_mass.size();++i){
    cout<<"chi2 "<<nicks[i]<<" "<<fitmean_chi2_mass[i]<<" +- "<<fitrms_chi2_mass[i]<<endl;
    average_chi2_rms+=fitrms_chi2_mass[i];
    average_chi2_mean+=fitmean_chi2_mass[i];
  }
  cout<<"======================================"<<endl;
  for(unsigned int i =0; i<fitmean_toptag_mass.size();++i){
    cout<<"toptag "<<nicks[i]<<" "<<fitmean_toptag_mass[i]<<" +- "<<fitrms_toptag_mass[i]<<endl;
    average_toptag_rms+=fitrms_toptag_mass[i];
    average_toptag_mean+=fitmean_toptag_mass[i];
  }
  cout<<"medium chi2 "<<average_chi2_mean/chi2_mass.size()<<" +- "<<average_chi2_rms/chi2_mass.size()<<endl;
  cout<<"medium toptag "<<average_toptag_mean/toptag_mass.size()<<" +- "<<average_toptag_rms/toptag_mass.size()<<endl;

  
  
  return 0;
}

