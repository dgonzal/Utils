#include <iostream>
#include <algorithm>


#include "TreeHists.h"
#include "simplePlots.h"
#include "TF1.h"
//#include "filefinder.h"

#include <boost/algorithm/string.hpp> // include Boost, a C++ library


using namespace std;

int main(){
  string CMSSW = "8_0_24_patch1";
  string folder ="MuSigSel";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.";
  string output ="plots/recofitMu.ps";
  bool single = false;
  
  //vector<string> filenames ={"BprimeB-800_LH","BprimeB-900_LH","BprimeB-1000_LH","BprimeB-1100_LH","BprimeB-1200_LH","BprimeB-1300_LH","BprimeB-1400_LH","BprimeB-1500_LH","BprimeB-1600_LH","BprimeB-1700_LH","BprimeB-1800_LH","BprimeB-800_RH","BprimeB-900_RH","BprimeB-1000_RH","BprimeB-1100_RH","BprimeB-1200_RH","BprimeB-1300_RH","BprimeB-1400_RH","BprimeB-1500_RH","BprimeB-1600_RH","BprimeB-1700_RH","BprimeB-1800_RH"};
  //vector<string> filenames ={"BprimeT-800_LH","BprimeT-900_LH","BprimeT-1000_LH","BprimeT-1100_LH","BprimeT-1200_LH","BprimeT-1300_LH","BprimeT-1400_LH","BprimeT-1500_LH","BprimeT-1600_LH","BprimeT-1700_LH","BprimeT-1800_LH","BprimeT-800_RH","BprimeT-900_RH","BprimeT-1000_RH","BprimeT-1100_RH","BprimeT-1200_RH","BprimeT-1300_RH","BprimeT-1400_RH","BprimeT-1500_RH","BprimeT-1600_RH","BprimeT-1700_RH","BprimeT-1800_RH"};
  //vector<string> filenames ={"X53-800_LH","X53-900_LH","X53-1000_LH","X53-1100_LH","X53-1200_LH","X53-1300_LH","X53-1400_LH","X53-1500_LH","X53-1600_LH"};
  vector<string> filenames ={"BprimeB-800_LH","BprimeB-900_LH","BprimeB-1000_LH","BprimeB-1100_LH","BprimeB-1200_LH","BprimeB-1300_LH","BprimeB-1400_LH","BprimeB-1500_LH","BprimeB-1600_LH","BprimeB-1700_LH","BprimeB-1800_LH","BprimeB-800_RH","BprimeB-900_RH","BprimeB-1000_RH","BprimeB-1100_RH","BprimeB-1200_RH","BprimeB-1300_RH","BprimeB-1400_RH","BprimeB-1500_RH","BprimeB-1600_RH","BprimeB-1700_RH","BprimeB-1800_RH","BprimeT-800_LH","BprimeT-900_LH","BprimeT-1000_LH","BprimeT-1100_LH","BprimeT-1200_LH","BprimeT-1300_LH","BprimeT-1400_LH","BprimeT-1500_LH","BprimeT-1600_LH","BprimeT-1700_LH","BprimeT-1800_LH","BprimeT-800_RH","BprimeT-900_RH","BprimeT-1000_RH","BprimeT-1100_RH","BprimeT-1200_RH","BprimeT-1300_RH","BprimeT-1400_RH","BprimeT-1500_RH","BprimeT-1600_RH","BprimeT-1700_RH","BprimeT-1800_RH"};
  
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
  int i =0;
  for(auto name : filenames){
    if(i==0)treehists.addFile(dir+name+".root","PE", i+2, -1 ,false, nicks[i]);
    else treehists.addFile(dir+name+".root","same",i+2, -1 ,false, nicks[i]);
    i++;
  }
  string helper_chooser = "BestFit.chi<1000 && BestFit.chi>-1 &&";
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

  string factor = "((BestFit.num_top+BestFit.num_whad)==3)*";

  std::vector<TH1F*> mass_toplep  = treehists.return_hists("BestFit.mass"      ,factor+"weight*("+helper_chooser+" BestFit.recoTyp ==21)","50,500,3000" ,"mass [GeV]");
  std::vector<TH1F*> top_toplep   = treehists.return_hists("BestFit.topLep.M()",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==21)","20,100,300"  ,"mass [GeV]");
  std::vector<TH1F*> w_toplep     = treehists.return_hists("BestFit.wHad.M()"  ,factor+"weight*("+helper_chooser+" BestFit.recoTyp ==21)","20,50,180"   ,"mass [GeV]");
  std::vector<TH1F*> dR_toplep    = treehists.return_hists("sqrt("+deltaRstring("BestFit.wHad","BestFit.topLep")+")",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  std::vector<TH1F*> dRtop_toplep = treehists.return_hists("topDR","weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  std::vector<TH1F*> dRw_toplep   = treehists.return_hists("topDR","weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  std::vector<TH1F*> dPhi_toplep  = treehists.return_hists("sqrt("+deltaPhistring("BestFit.wHad","BestFit.topLep")+")",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
  std::vector<TH1F*> dEta_toplep  = treehists.return_hists("sqrt("+deltaEtastring("BestFit.wHad","BestFit.topLep")+")",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==21)","30,0,4"   ,"");
     
  std::vector<TH1F*> mass_tophad = treehists.return_hists("BestFit.mass"      ,factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","50,500,3000" ,"mass [GeV]");
  std::vector<TH1F*> top_tophad  = treehists.return_hists("BestFit.topHad.M()",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","20,100,300"  ,"mass [GeV]");
  std::vector<TH1F*> w_tophad    = treehists.return_hists("BestFit.wLep.M()"  ,factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","20,50,180"   ,"mass [GeV]");
  std::vector<TH1F*> dR_tophad    = treehists.return_hists("sqrt("+deltaRstring("BestFit.wLep","BestFit.topHad")+")",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","30,0,4"   ,"");
  std::vector<TH1F*> dRtop_tophad    = treehists.return_hists("topDR",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","30,0,4"   ,"");
  std::vector<TH1F*> dRw_tophad    = treehists.return_hists("topDR",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","30,0,4"   ,"");
  std::vector<TH1F*> dPhi_tophad    = treehists.return_hists("sqrt("+deltaPhistring("BestFit.wLep","BestFit.topHad")+")",factor+"weight*("+helper_chooser+" BestFit.recoTyp ==22)","30,0,4"   ,"");
  
  vector<double> fitmean_toplep;
  vector<double> fitmean_tophad;
  vector<double> fitmean_whad;

  vector<double> fitrms_toplep;
  vector<double> fitrms_tophad;
  vector<double> fitrms_whad;


  TF1* gaus = new TF1("gaus","gaus");
  simplePlots recofit(output,single);
  recofit.switch_ratio(false);
  recofit.set_histYTitle("AU");
  recofit.set_title(" ");
  for(unsigned int i=0; i<mass_toplep.size();++i){
    if(mass_toplep[i]->GetEntries()<50) continue;

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
    top_toplep[i]->Fit(gaus,"","",120,210);
    fitmean_toplep.push_back(gaus->GetParameter(1));
    fitrms_toplep.push_back(gaus->GetParameter(2));
    recofit.loadHists(top_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();

    top_tophad[i]->Fit(gaus,"","",120,210);
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

    w_toplep[i]->Fit(gaus,"","",65,100);
    fitmean_whad.push_back(gaus->GetParameter(1));
    fitrms_whad.push_back(gaus->GetParameter(2));
    recofit.loadHists(w_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();
    
    recofit.set_XTitle("#Delta R(top,W)");
    //dR_toplep[i]->Fit(gaus,"","",2.8,3.3);
    recofit.loadHists(dR_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();
    
    //dR_tophad[i]->Fit(gaus,"","",2.8,3.3);
    recofit.loadHists(dR_tophad[i],nicks[i]+" top_{had}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();
    
    recofit.set_XTitle("#Delta #Phi (top,W)");
    //dPhi_tophad[i]->Fit(gaus,"","",2,3.14);
    recofit.loadHists(dPhi_tophad[i],nicks[i]+" top_{had}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();
    
    //dPhi_toplep[i]->Fit(gaus,"","",2,3.14);
    recofit.loadHists(dPhi_toplep[i],nicks[i]+" top_{lep}","PE");
    recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
    recofit.plotHists(2,false);
    recofit.clearAll();

    
    recofit.set_XTitle("#Delta #eta (top,W)");
    //dEta_toplep[i]->Fit(gaus,"","",2,3.14);
    recofit.loadHists(dPhi_toplep[i],nicks[i]+" top_{lep}","PE");
    //recofit.addLegendEntry("#mu "+to_string(gaus->GetParameter(1)));
    //recofit.addLegendEntry("#sigma "+to_string(gaus->GetParameter(2)));
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

    sum_rms_toplep += 1/fitrms_toplep[i];
    sum_rms_tophad += 1/fitrms_tophad[i];
    sum_rms_whad += 1/fitrms_whad[i];

    rec_rms_toplep += 1/(fitrms_toplep[i]*fitrms_toplep[i]);
    rec_rms_tophad += 1/(fitrms_tophad[i]*fitrms_tophad[i]);
    rec_rms_whad +=1/(fitrms_whad[i]*fitrms_whad[i]);
  }
  
  fitmean_toplep[0]/=fitrms_toplep[0];
  fitmean_tophad[0]/=fitrms_tophad[0];
  fitmean_whad[0]/=fitrms_whad[0];

  for(unsigned int i=1;i<N;++i){
    //mean
    fitmean_toplep[0]+=fitmean_toplep[i]/fitrms_toplep[i];
    fitmean_tophad[0]+=fitmean_tophad[i]/fitrms_tophad[i];
    fitmean_whad[0]+=fitmean_whad[i]/fitrms_whad[i];
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
