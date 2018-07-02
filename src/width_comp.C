#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>



int main(){
  string resultfile = "plots/width_comparisonMu/";
  string CMSSW = "8_0_24_patch1";
  string folder= "MuSigSel/";//"MuSigSel/"; //"EleSigSel_v1/";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.";
  
  string eta = "2.4";
  string chi2_central_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "TopTagDis.mass>-1 && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "TopTagDis.mass>-1 && abs(TopTagDis.forwardJet.eta()) >="+eta;


  vector<vector<string>> container={{"BprimeT-800_LH","BprimeT-1000_LH","BprimeT-1200_LH","BprimeT-1400_LH","BprimeT-1600_LH"},
				    {"BprimeT-800_RH","BprimeT-1000_RH","BprimeT-1200_RH","BprimeT-1400_RH","BprimeT-1600_RH"},
				    {"BprimeB-800_LH","BprimeB-1000_LH","BprimeB-1200_LH","BprimeB-1400_LH","BprimeB-1600_LH"},
				    {"BprimeB-800_RH","BprimeB-1000_RH","BprimeB-1200_RH","BprimeB-1400_RH","BprimeB-1600_RH"},
				    
                                    {"BprimeT-800_LH","BprimeT-800_Width-10p_LH","BprimeT-800_Width-20p_LH","BprimeT-800_Width-30p_LH"},
                                    {"BprimeT-800_RH","BprimeT-800_Width-10p_RH","BprimeT-800_Width-20p_RH","BprimeT-800_Width-30p_RH"},
				    {"BprimeT-1000_LH","BprimeT-1000_Width-10p_LH","BprimeT-1000_Width-20p_LH","BprimeT-1000_Width-30p_LH"},
				    {"BprimeT-1200_LH","BprimeT-1200_Width-10p_LH","BprimeT-1200_Width-20p_LH","BprimeT-1200_Width-30p_LH"},
				    {"BprimeT-1400_LH","BprimeT-1400_Width-10p_LH","BprimeT-1400_Width-20p_LH","BprimeT-1400_Width-30p_LH"},
				    {"BprimeT-1600_LH","BprimeT-1600_Width-10p_LH","BprimeT-1600_Width-20p_LH","BprimeT-1600_Width-30p_LH"},

				    {"BprimeB-800_LH","BprimeB-800_Width-10p_LH","BprimeB-800_Width-20p_LH","BprimeB-800_Width-30p_LH"},
				    {"BprimeB-1000_LH","BprimeB-1000_Width-10p_LH","BprimeB-1000_Width-20p_LH","BprimeB-1000_Width-30p_LH"},
				    {"BprimeB-1200_LH","BprimeB-1200_Width-10p_LH","BprimeB-1200_Width-20p_LH","BprimeB-1200_Width-30p_LH"},
				    {"BprimeB-1400_LH","BprimeB-1400_Width-10p_LH","BprimeB-1400_Width-20p_LH","BprimeB-1400_Width-30p_LH"},
				    {"BprimeB-1600_LH","BprimeB-1600_Width-10p_LH","BprimeB-1600_Width-20p_LH","BprimeB-1600_Width-30p_LH"},

				    {"BprimeT-800_Width-10p_LH","BprimeT-1000_Width-10p_LH","BprimeT-1200_Width-10p_LH","BprimeT-1400_Width-10p_LH","BprimeT-1600_Width-10p_LH"},
				    {"BprimeB-800_Width-10p_LH","BprimeB-1000_Width-10p_LH","BprimeB-1200_Width-10p_LH","BprimeB-1400_Width-10p_LH","BprimeB-1600_Width-10p_LH"},
				    
				    {"X53-800_LH","X53-1000_LH","X53-1200_LH","X53-1400_LH","X53-1600_LH"},
				    {"X53-800_LH","BprimeT-800_LH","X53-1000_LH","BprimeT-1000_LH"},
				    {"X53-1200_LH","BprimeT-1200_LH","X53-1400_LH","BprimeT-1400_LH"}
  };

  
  vector<vector<string>> names_con={{"B+t M(800) LH","B+t M(1000) LH","B+t M(1200) LH","B+t M(1400) LH","B+t M(1600) LH"},
				    {"B+t M(800) RH","B+t M(1000) RH","B+t M(1200) RH","B+t M(1400) RH","B+t M(1600) RH"},
				    {"B+b M(800) LH","B+b M(1000) LH","B+b M(1200) LH","B+b M(1400) LH","B+b M(1600) LH"},
				    {"B+b M(800) RH","B+b M(1000) RH","B+b M(1200) RH","B+b M(1400) RH","B+b M(1600) RH"},
				    
				    {"B+t M(800) LH","10%","20%","30%"},
				    {"B+t M(800) RH","10%","20%","30%"},
				    {"B+t M(1000) LH","10%","20%","30%"},
				    {"B+t M(1200) LH","10%","20%","30%"},
				    {"B+t M(1400) LH","10%","20%","30%"},
				    {"B+t M(1600) LH","10%","20%","30%"},

				    {"B+b M(800)","10%","20%","30%"},
				    {"B+b M(1000)","10%","20%","30%"},
				    {"B+b M(1200)","10%","20%","30%"},
				    {"B+b M(1400)","10%","20%","30%"},
				    {"B+b M(1600)","10%","20%","30%"},
				    
				    {"B+t M(800) 10%","B+t M(1000) 10%","B+t M(1200) 10%","B+t M(1400) 10%","B+t M(1600) 10%"},
				    {"B+b M(800) 10%","B+b M(1000) 10%","B+b M(1200) 10%","B+b M(1400) 10%","B+b M(1600) 10%"},
				    {"X53 M(800)","X53 M(1000)","X53 M(1200)","X53 M(1400)","X53 M(1600)"},
				    {"X53 M(800)","B+t M(800)","X53 M(1000)","B+t M(1000)"},
				    {"X53 M(1200)","B+t M(1200)","X53 M(1400)","B+t M(1400)"}
				    


  };

  TLatex *cms_text = new TLatex(0.9, 0.95,"#mu channel 35.9 fb^{-1} (13 TeV)");
  cms_text->SetNDC();
  cms_text->SetTextAlign(33);
  cms_text->SetTextFont(42);
  cms_text->SetTextSize(0.04);

  TLatex *preliminary_text = new TLatex(0.1, 0.95,"CMS #it{Simulation}");
  preliminary_text->SetNDC();
  preliminary_text->SetTextAlign(13);
  preliminary_text->SetTextFont(42);
  preliminary_text->SetTextSize(0.045);
 
  TLatex *chi2_text = new TLatex(0.8, 0.3,"X^{2} reconstruction");
  chi2_text->SetNDC();
  chi2_text->SetTextAlign(33);
  chi2_text->SetTextFont(42);
  chi2_text->SetTextSize(0.05);
  
  TLatex *toptag_text = new TLatex(0.8, 0.3,"t-tag reconstruction");
  toptag_text->SetNDC();
  toptag_text->SetTextAlign(33);
  toptag_text->SetTextFont(42);
  toptag_text->SetTextSize(0.05);
  
  TreeHists treehists(resultfile,true);
  treehists.switch_ratio();
  treehists.SetTree("AnalysisTree");
  treehists.SetLegend(0.5, 0.3, 0.8, 0.86);

  for(unsigned int m=0; m<container.size();++m){
    //if(m>2)continue;
    if(m==10)treehists.SetLegend(0.5, 0.3, 0.8, 0.8);
    vector<string> samples= container[m];
    vector<string> names = names_con[m];
    unsigned int col=2;
    for(unsigned int i =0; i<samples.size();++i){
      if(col==5 || col==10 || col==8)col++;
      cout<<"loading File: "<<dir+samples[i]+".root"<<" nick: "<<names[i]<<endl;
      treehists.addFile(dir+samples[i]+".root","hist",col,-1,false,names[i]);
      col++;
    }

    //treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+")","30,500,3000","forward X^{2} B mass [GeV]");	    
    //treehists.Draw("TopTagDis.mass","weight*("+toptag_forward_string+")","30,500,3000","forward toptag B mass [GeV]");

    treehists.clearText();
    treehists.addText(cms_text);
    treehists.addText(preliminary_text);
    treehists.addText(chi2_text);
    // Draw(std::string variable, std::string draw_option="", std::string binning="", std::string x_axis="", std::string y_axis="Events", bool legend=true, std::string data_draw_option="", std::string plot_name="");
    
    
    
    treehists.Draw("Chi2Dis.mass","weight","40,300,3500","m_{reco} [GeV]", "Events", true, "", "signal_chi2_mass");	    
    /*/
    treehists.Draw(deltaPhistring("Chi2Dis.wHad","Chi2Dis.topLep"),"weight*(Chi2Dis.recoTyp==11)","40,0,3.5","#Delta #phi(W_{had},t_{lep})", "Events", true, "");	    
    treehists.Draw(deltaPhistring("Chi2Dis.wLep","Chi2Dis.topHad"),"weight*(Chi2Dis.recoTyp==12)","40,0,3.5","#Delta #phi(W_{lep},t_{had})", "Events", true, "");
    
    treehists.Draw(deltaEtastring("Chi2Dis.wHad","Chi2Dis.topLep"),"weight*(Chi2Dis.recoTyp==11)","40,0,3.5","#Delta #eta(W_{had},t_{lep})", "Events", true, "");	    
    treehists.Draw(deltaEtastring("Chi2Dis.wLep","Chi2Dis.topHad"),"weight*(Chi2Dis.recoTyp==12)","40,0,3.5","#Delta #eta(W_{lep},t_{had})", "Events", true, "");

    treehists.Draw("cos("+deltaPhistring("Chi2Dis.wHad","Chi2Dis.topLep")+")","weight*(Chi2Dis.recoTyp==11)","40,-1,1","cos(#Delta #phi(W_{had},t_{lep}))", "Events", true, "");	    
    treehists.Draw("cos("+deltaPhistring("Chi2Dis.wLep","Chi2Dis.topHad")+")","weight*(Chi2Dis.recoTyp==12)","40,-1,1","cos(#Delta #phi(W_{lep},t_{had}))", "Events", true, "");

    
    treehists.Draw(deltaRstring("Chi2Dis.wHad","Chi2Dis.topLep"),"weight*(Chi2Dis.recoTyp==11)","40,0,3.5","#Delta R(W_{had},t_{lep})", "Events", true, "");	    
    treehists.Draw(deltaRstring("Chi2Dis.wLep","Chi2Dis.topHad"),"weight*(Chi2Dis.recoTyp==12)","40,0,3.5","#Delta R(W_{lep},t_{had})", "Events", true, "");
    /*/ 
  
    treehists.clearText();
    treehists.addText(cms_text);
    treehists.addText(preliminary_text);
    treehists.addText(toptag_text);
    treehists.Draw("TopTagDis.mass","weight","40,300,3500","m_{reco} [GeV]", "Events", true, "", "signal_ttag_mass");
    //treehists.Draw("cos("+deltaPhistring("TopTagDis.wLep","TopTagDis.topHad")+")","weight*(Chi2Dis.recoTyp==12)","40,-1,1","cos(#Delta #phi(W_{lep},t_{had}))", "Events", true, "");
    treehists.clearFiles();
  }
  return 0;
}
