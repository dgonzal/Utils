#include <iostream>

#include "TreeRootHist.h"
#include "simplePlots.h"

#include <boost/range/iterator_range.hpp>
#include <experimental/filesystem>

int main(){
  string resultfile = "plots/paper/";
  string CMSSW = "8_0_24_patch1";
  string folder= "MuSigSel/";//"MuSigSel/"; //"EleSigSel_v1/";
  string dir = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_"+CMSSW+"/src/UHH2/VLQToTopAndLepton/config/"+folder+"/uhh2.AnalysisModuleRunner.MC.";
  
  string eta = "2.4";
  string chi2_central_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string chi2_forward_string = "TopTagDis.mass==-1 && abs(Chi2Dis.forwardJet.eta()) >="+eta;
  string toptag_central_string = "TopTagDis.mass>-1 && abs(TopTagDis.forwardJet.eta()) <" +eta; //"||Chi2Dis.jetiso >="+jetiso+
  string toptag_forward_string = "TopTagDis.mass>-1 && abs(TopTagDis.forwardJet.eta()) >="+eta;
  std::string channel = "Mu";
  

  vector<string> container={"BprimeT-800_LH","BprimeT-1000_LH","BprimeT-1200_LH","BprimeT-1400_LH","BprimeT-1600_LH",
			    "BprimeT-800_RH","BprimeT-1000_RH","BprimeT-1200_RH","BprimeT-1400_RH","BprimeT-1600_RH",
			    "BprimeB-800_LH","BprimeB-1000_LH","BprimeB-1200_LH","BprimeB-1400_LH","BprimeB-1600_LH",
			    "BprimeB-800_RH","BprimeB-1000_RH","BprimeB-1200_RH","BprimeB-1400_RH","BprimeB-1600_RH",
				    
			    "BprimeT-800_Width-10p_LH","BprimeT-800_Width-20p_LH","BprimeT-800_Width-30p_LH",
			    "BprimeT-1000_Width-10p_LH","BprimeT-1000_Width-20p_LH","BprimeT-1000_Width-30p_LH",
			    "BprimeT-1200_Width-10p_LH","BprimeT-1200_Width-20p_LH","BprimeT-1200_Width-30p_LH",
			    "BprimeT-1400_Width-10p_LH","BprimeT-1400_Width-20p_LH","BprimeT-1400_Width-30p_LH",
			    "BprimeT-1600_Width-10p_LH","BprimeT-1600_Width-20p_LH","BprimeT-1600_Width-30p_LH",
			    
			    "BprimeB-800_Width-10p_LH","BprimeB-800_Width-20p_LH","BprimeB-800_Width-30p_LH",
			    "BprimeB-1000_Width-10p_LH","BprimeB-1000_Width-20p_LH","BprimeB-1000_Width-30p_LH",
			    "BprimeB-1200_Width-10p_LH","BprimeB-1200_Width-20p_LH","BprimeB-1200_Width-30p_LH",
			    "BprimeB-1400_Width-10p_LH","BprimeB-1400_Width-20p_LH","BprimeB-1400_Width-30p_LH",
			    "BprimeB-1600_Width-10p_LH","BprimeB-1600_Width-20p_LH","BprimeB-1600_Width-30p_LH",
			    			    
			    "X53-800_LH","X53-1000_LH","X53-1200_LH","X53-1400_LH","X53-1600_LH"
  };

  
  vector<string> names_con={"B+t M(800) LH","B+t M(1000) LH","B+t M(1200) LH","B+t M(1400) LH","B+t M(1600) LH",
			    "B+t M(800) RH","B+t M(1000) RH","B+t M(1200) RH","B+t M(1400) RH","B+t M(1600) RH",
			    "B+b M(800) LH","B+b M(1000) LH","B+b M(1200) LH","B+b M(1400) LH","B+b M(1600) LH",
			    "B+b M(800) RH","B+b M(1000) RH","B+b M(1200) RH","B+b M(1400) RH","B+b M(1600) RH",
			    
			    "B+t M(800)  LH 10%","B+t M(800)  LH 20%","B+t M(800)  LH 30%",
			    "B+t M(1000) LH 10%","B+t M(1000) LH 20%","B+t M(1000) LH 30%",
			    "B+t M(1200) LH 10%","B+t M(1200) LH 20%","B+t M(1200) LH 30%",
			    "B+t M(1400) LH 10%","B+t M(1400) LH 20%","B+t M(1400) LH 30%",
			    "B+t M(1600) LH 10%","B+t M(1600) LH 20%","B+t M(1600) LH 30%",

			    "B+b M(800)  LH 10%","B+b M(800)  LH 20%","B+b M(800)  LH 30%",
			    "B+b M(1000) LH 10%","B+b M(1000) LH 20%","B+b M(1000) LH 30%",
			    "B+b M(1200) LH 10%","B+b M(1200) LH 20%","B+b M(1200) LH 30%",
			    "B+b M(1400) LH 10%","B+b M(1400) LH 20%","B+b M(1400) LH 30%",
			    "B+b M(1600) LH 10%","B+b M(1600) LH 20%","B+b M(1600) LH 30%",
			    
			    "X53 M(800)  LH","X53 M(1000) LH","X53 M(1200) LH","X53 M(1400) LH","X53 M(1600) LH",

  };  
  TreeRootHist treehists(resultfile,true);
  treehists.SetLegend(0.5, 0.3, 0.8, 0.86);
  for(unsigned int i=0; i<container.size();i++){
    unsigned int col=2;
    if(col==5 || col==10 || col==8)col++;
    cout<<"loading File: "<<dir+container[i]+".root"<<" nick: "<<names_con[i]<<endl;
    treehists.addFile(dir+container[i]+".root","hist",col,-1,false,names_con[i]);
    col++;
  }
  treehists.Draw_ROOT("Chi2Dis.mass","weight","40,0,3500","m_{reco} [GeV]", "Events", true, "", channel+"_signal_chi2_mass__");	    
  treehists.Draw_ROOT("TopTagDis.mass","weight","40,0,3500","m_{reco} [GeV]", "Events", true, "", channel+"_signal_ttag_mass__");
  return 0;
}
