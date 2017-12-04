#include "TreeHists.h"
#include "effiPlots.h"
#include "simplePlots.h"
#include "Utils.h"
#include "eletriggerresult.h"

#include "TRandom3.h"

#include <iostream>
#include <fstream>

using namespace std;

double gausRandom(double sigma=1, double mean=0)
{
   // this or any other 'hashing/randomizing' algorithm.
   TRandom3 engine(1); // setting the seed.
   return engine.Gaus(mean,sigma);
}

string stochastic_smear(string jet){  
  string sf = "((fabs("+jet+".eta())<=0.5)*1.109 +(fabs("+jet+".eta())>0.5 && fabs("+jet+".eta())<=0.8)*1.138 + (fabs("+jet+".eta())>0.8 && fabs("+jet+".eta())<=1.1)*1.114+ (fabs("+jet+".eta())>1.1 && fabs("+jet+".eta())<=1.3)*1.123 + (fabs("+jet+".eta())>1.3 && fabs("+jet+".eta())<=1.7)*1.084 + (fabs("+jet+".eta())>1.7 && fabs("+jet+".eta())<=1.9)*1.082 + (fabs("+jet+".eta())>1.9 && fabs("+jet+".eta())<=2.1)*1.14 + (fabs("+jet+".eta())>2.1 && fabs("+jet+".eta())<=2.3)*1.067 + (fabs("+jet+".eta())>2.3 && fabs("+jet+".eta())<=2.5)*1.177)";
  return "(1+fabs(sqrt(-2*TMath::Log(rndm))*TMath::Cos(2*TMath::Pi()*rndm)*0.12)*sqrt("+sf+"*"+sf+"-1))";
}

int main(){

  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/VLQToTopAndLepton/config/MuSel_wtag_unc/";
  string resultfile = "plots/wtag_unc/mu_distribution.ps";
  string effiresult = "plots/muonscalefactor/effi_highpt/";

  bool single = false;
  
  string factors = "";//"("+eletriggerscale()+"*(1-isRealData)+isRealData)*";
  //factors = "(((fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<0.2)*0.97+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=0.2 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<1)*0.98+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=1 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<1.6)*0.967+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=1.6 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<1.8)*0.983+(fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=1.8 && fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])<2.2)*0.974+ (fabs(slimmedMuonsUSER.slimmedMuonsUSER.m_eta[0])>=2.2)*0.956)*(1-isRealData)+isRealData)*";
  cout<<"factors applied "<<factors<<endl;

  std::string chirality = "LH";
  std::string production = "T";
  std::string lower_case = "b";
  std::vector<std::string> masses = {"800","900","1000","1100","1200","1300","1400","1500","1600","1700","1800"};
  
  TreeHists treehists(resultfile,single);
  int i =0;
  for(auto mass : masses){
    treehists.addFile(dir+"/uhh2.AnalysisModuleRunner.MC.Bprime"+production+"-"+mass+"_"+chirality+".root","hist",i+1,-1,false,"B+"+lower_case+" M("+mass+") "+chirality,0,1);
    i++;
  }
  
  treehists.set_ratiolimits(1.69,0.39);
  treehists.SetTree("AnalysisTree");
  treehists.switch_ratio();
 
  TLatex *lumi_text = new TLatex(3.5, 24,"35.8 fb^{-1} (13 TeV)"); //CMS Preliminary
  lumi_text->SetNDC();
  lumi_text->SetTextAlign(33);
  lumi_text->SetX(0.9);
  lumi_text->SetTextFont(42);
  lumi_text->SetTextSize(0.04);
  lumi_text->SetY(.95);
  treehists.addText(lumi_text);
  TLatex *preliminary_text = new TLatex(3.5, 24,"CMS #it{Preliminary}");
  preliminary_text->SetNDC();
  preliminary_text->SetTextAlign(33);
  preliminary_text->SetX(0.5);
  preliminary_text->SetTextFont(42);
  preliminary_text->SetTextSize(0.045);
  preliminary_text->SetY(.95);
  treehists.addText(preliminary_text);

  treehists.Draw("WTagDis.wHad.pt()","weight*(TopTagDis.mass>-1&& WTagDis.wHad.M()<110)","100,170,1500","W-tag p_{T} [GeV]");
  treehists.Draw("WTagDis.wHad.M()","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,105","W-tag mass [GeV]");
  treehists.Draw("WTagDis.wHad.M()","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,95","W-tag mass [GeV]");

  treehists.Draw("WTagDis.wHad.M()",stochastic_smear("WTagDis.wHad")+"*weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,105","W-tag mass [GeV]");
  //treehists.Draw("fabs(sqrt(-2*TMath::Log(rndm))*TMath::Cos(2*TMath::Pi()*rndm)*0.12)","","100,0,1","random numbers");
  
  std::vector<TH1F*> wtag_loose = treehists.return_hists("WTagDis.wHad.M()","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,105","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_tight = treehists.return_hists("WTagDis.wHad.M()","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,95","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_loose_smear = treehists.return_hists("WTagDis.wHad.M()",stochastic_smear("WTagDis.wHad")+"*weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,105","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_tight_smear = treehists.return_hists("WTagDis.wHad.M()",stochastic_smear("WTagDis.wHad")+"*weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,95","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_loose_shift_up = treehists.return_hists("WTagDis.wHad.M()+0.0094","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,105","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_tight_shift_up = treehists.return_hists("WTagDis.wHad.M()+0.0094","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,95","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_loose_shift_down = treehists.return_hists("WTagDis.wHad.M()-0.0094","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,105","W-tag mass [GeV]");
  std::vector<TH1F*> wtag_tight_shift_down = treehists.return_hists("WTagDis.wHad.M()-0.0094","weight*(TopTagDis.mass>-1 && WTagDis.wHad.M()<110)","10,65,95","W-tag mass [GeV]");
  for(unsigned int i=0; i< wtag_loose.size(); i++){
    double nominal_eff_i = wtag_tight[i]->GetSumOfWeights()/wtag_loose[i]->GetSumOfWeights();
    double smear_eff_i = wtag_tight_smear[i]->GetSumOfWeights()/wtag_loose_smear[i]->GetSumOfWeights();
    double shift_up_i = wtag_tight_shift_up[i]->GetSumOfWeights()/wtag_loose_shift_up[i]->GetSumOfWeights();
    double shift_down_i = wtag_tight_shift_down[i]->GetSumOfWeights()/wtag_loose_shift_down[i]->GetSumOfWeights();
    cout<<"mass "<<masses[i]  << "  "<<nominal_eff_i<<endl;
    cout<<"mass smear "<<masses[i]  << "  "<<smear_eff_i<<" +- "<<nominal_eff_i-smear_eff_i<<endl;
    cout<<"mass shift up "<<masses[i]  << "  "<<shift_up_i<<" +- "<<nominal_eff_i-shift_up_i<<endl;
    cout<<"mass shift down "<<masses[i]  << "  "<<shift_down_i<<" +- "<<nominal_eff_i-shift_down_i<<endl;
    if(i>0){
      wtag_loose[0]->Add(wtag_loose[i]);
      wtag_tight[0]->Add(wtag_tight[i]);
      wtag_loose_smear[0]->Add(wtag_loose[i]);
      wtag_tight_smear[0]->Add(wtag_tight[i]);
      wtag_loose_shift_up[0]->Add(wtag_loose[i]);
      wtag_tight_shift_up[0]->Add(wtag_tight[i]);
      wtag_loose_shift_down[0]->Add(wtag_loose[i]);
      wtag_tight_shift_down[0]->Add(wtag_tight[i]);
    }
  }
  cout<<"<================>"<<endl;
  double nominal_eff = wtag_tight[0]->GetSumOfWeights()/wtag_loose[0]->GetSumOfWeights();
  double smear_eff = wtag_tight_smear[0]->GetSumOfWeights()/wtag_loose_smear[0]->GetSumOfWeights();
  double shift_up = wtag_tight_shift_up[0]->GetSumOfWeights()/wtag_loose_shift_up[0]->GetSumOfWeights();
  double shift_down = wtag_tight_shift_down[0]->GetSumOfWeights()/wtag_loose_shift_down[0]->GetSumOfWeights();
  cout<<"sum "<<nominal_eff<<endl;
  cout<<"sum smear "<<smear_eff<<" +- "<<nominal_eff-smear_eff<<endl;
  cout<<"sum shift up "<<shift_up<<" +- "<<nominal_eff-shift_up<<endl;
  cout<<"sum shift down "<<shift_down<<" +- "<<nominal_eff-shift_down <<endl;

  
  return 0;

}
