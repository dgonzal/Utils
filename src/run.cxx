//main routine call everything else from here.
//so i do not have to change my makefile and everything compiles

#include "slicePlots.h"
#include "simplePlots.h"
#include "effiPlots.h"



#include <iostream>


using namespace std;

int main(){

  effiPlots vlqEffi("plots/test.ps");
  vlqEffi.addFile("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_2_0_pre4/src/UHH2/VLQToTopAndLepton/config/Output/uhh2.AnalysisModuleRunner.MC.BpJ_TW_800.root");
  vlqEffi.loadHists("muonChannel_MuonHists/pt","muonTrigger_muonChannel_MuonHists/pt");
  vlqEffi.loadHists("eleChannel_ElectronHists/pt","eleChannel_Trigger_eleChannel_ElectronHists/pt");
  vlqEffi.plotEffi();

  

  
 

  cout<<"hello world, I'm done"<<endl;
  return 0;
}

