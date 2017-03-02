#include <iostream>

#include "TreeHists.h"
#include "simplePlots.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>



int main(){
  string resultfile = "plots/width_comparison.ps";
  string CMSSW = "8_0_24_patch1";
  string folder= "MuSigSel_v1/";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.";
  
  string eta = "2.0";
  string energy = "0.";
  string chi2_central_string = "TopTagDis.mass==-1 &&((abs(Chi2Dis.forwardJet.eta()) <" +eta+") || Chi2Dis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "TopTagDis.mass==-1 && (abs(Chi2Dis.forwardJet.eta()) >="+eta+") && Chi2Dis.forwardJet.E()>="+energy;
  string toptag_central_string = "TopTagDis.mass>-1 &&((abs(TopTagDis.forwardJet.eta()) <" +eta+") || TopTagDis.forwardJet.E()<" +energy+")"; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "TopTagDis.mass>-1 && (abs(TopTagDis.forwardJet.eta()) >="+eta+") && TopTagDis.forwardJet.E()>="+energy;


  vector<vector<string>> container={{"BprimeT-800_LH","BprimeT-800_Width-10p_LH","BprimeT-800_Width-20p_LH","BprimeT-800_Width-30p_LH"},
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

  
  vector<vector<string>> names_con={{"B+t M(800)","10%","20%","30%"},
				    {"B+t M(1000)","10%","20%","30%"},
				    {"B+t M(1200)","10%","20%","30%"},
				    {"B+t M(1400)","10%","20%","30%"},
				    {"B+t M(1600)","10%","20%","30%"},

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

  TreeHists treehists(resultfile);
  treehists.switch_ratio();
  treehists.SetTree("AnalysisTree");
  treehists.SetLegend(0.6, 0.3, 0.86, 0.86);

  for(unsigned int m=0; m<container.size();++m){
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

    treehists.Draw("Chi2Dis.mass","weight*("+chi2_forward_string+")","30,500,3000","forward X^{2} B mass [GeV]");	    
    treehists.Draw("TopTagDis.mass","weight*("+toptag_forward_string+")","30,500,3000","forward toptag B mass [GeV]");

    treehists.clearFiles();
  }
  return 0;
}
